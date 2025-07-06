#include "pch.h"
#include "Game/Render/CustomModelRenderer.h"
#include "Game/CommonResources.h"
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#include "Libraries/yamadaLib/Resources.h"   
#include "Libraries/yamadaLib/Graphics.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="graphics">�O���t�B�b�N�X</param>
CustomModelRenderer::CustomModelRenderer(CommonResources* resources, yamadalib::Graphics* graphics)
{
   m_commonResources = resources;
   m_graphics = graphics;
   // �V�F�[�_�[�Ɠ��̓��C�A�E�g�̐���
   LoadShaders(m_commonResources->GetDeviceResources()->GetD3DDevice());
   //�|�C���g���C�g���N���A����
   ClearPointLightsParameter();

   //������𐶐�
   m_frustum = std::make_unique<FrustumCuller>();

   m_cubeMapShadow = std::make_unique<CubeMapShadow>(m_commonResources);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CustomModelRenderer::~CustomModelRenderer()
{
   ClearPointLightsParameter();
}

/// <summary>
/// ���f����`�悷��
/// </summary>
/// <param name="model">���f��</param>
/// <param name="texture">���f���̃e�N�X�`��</param>
/// <param name="world">�s��</param>
void CustomModelRenderer::Draw(const DirectX::Model& model, ID3D11ShaderResourceView* texture, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale)
{
   //�`�惂�f���̒��S���W
   DirectX::SimpleMath::Vector3 modelCenter = world.Translation();
   //�|�C���g���C�g�̃\�[�g
   SetClosestLights(modelCenter, MAX_POINT_LIGHT);

   // �f�o�C�X�R���e�L�X�g�E�f�o�C�X�E�R�����X�e�[�g���擾
   ID3D11DeviceContext* context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
   auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
   DirectX::CommonStates* states = yamadalib::Graphics::GetInstance()->GetCommonStates();

   // �u�����h�E�[�x�E���X�^���C�U�X�e�[�g���Z�b�g
   context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
   context->OMSetDepthStencilState(nullptr, 0);
   context->RSSetState(states->CullCounterClockwise());

   // �[�x�X�e���V���X�e�[�g�𖾎��I�ɍ쐬�E�Z�b�g�i�[�x�e�X�g�L���E�������ݗL���E��r�֐�LESS�j
   D3D11_DEPTH_STENCIL_DESC depthDesc = {};
   depthDesc.DepthEnable = TRUE;
   depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
   depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
   Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
   device->CreateDepthStencilState(&depthDesc, &depthState);
   context->OMSetDepthStencilState(depthState.Get(), 0);

   // �t�H�O�p�萔�o�b�t�@���쐬�E�X�V
   UpdateConstantBuffers(context, world, uvScale);

   // �V�F�[�_�[�E���̓��C�A�E�g���Z�b�g
   context->IASetInputLayout(m_inputLayout.Get());
   context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
   context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

   // �e�N�X�`���iSRV�j���s�N�Z���V�F�[�_�[�ɃZ�b�g
   assert(texture != nullptr);
   ID3D11ShaderResourceView* srvs[1] = {
      texture/*,
      m_cubeMapShadow->GetSRV()*/
   };
   context->PSSetShaderResources(0, 1, srvs);
   // �T���v���[�X�e�[�g���s�N�Z���V�F�[�_�[�ɃZ�b�g
   ID3D11SamplerState* samplers[1] = {
      states->LinearWrap()/*,
      m_cubeMapShadow->GetShadowSampler(),
      m_cubeMapShadow->GetDebugSampler()*/
   };
   context->PSSetSamplers(0, 1, samplers);

   // �萔�o�b�t�@���V�F�[�_�[�ɃZ�b�g
   context->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());
   context->PSSetConstantBuffers(1, 1, m_fogBuffer.GetAddressOf());
   context->PSSetConstantBuffers(2, 1, m_uvScaleBuffer.GetAddressOf());
   context->PSSetConstantBuffers(3, 1, m_lightBuffer.GetAddressOf());
   context->PSSetConstantBuffers(4, 1, m_pointLightBuffer.GetAddressOf());
   //�|���S����`�悷��
   context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   // ���f���̊e���b�V���E�p�[�g���Ƃɒ��_/�C���f�b�N�X�o�b�t�@���Z�b�g���`��
   for (const auto& mesh : model.meshes)
   {
      for (const auto& part : mesh->meshParts)
      {
         UINT stride = part->vertexStride;
         UINT offset = 0;
         context->IASetVertexBuffers(0, 1, part->vertexBuffer.GetAddressOf(), &stride, &offset);
         context->IASetIndexBuffer(part->indexBuffer.Get(), part->indexFormat, 0);
         //�`��
         context->DrawIndexed(part->indexCount, part->startIndex, part->vertexOffset);
      }
   }

   // �e�N�X�`���X���b�g���N���A�iSRV�����h�~�j
   ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
   context->PSSetShaderResources(0, 2, nullSRV);
}

/// <summary>
/// �萔�o�b�t�@�i�s��E�t�H�O�j�̍X�V
/// </summary>
/// <param name="context">�R���e�L�X�g</param>
/// <param name="world">�s��</param>
void CustomModelRenderer::UpdateConstantBuffers(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale)
{
   // �s��o�b�t�@���X�V�i�]�u���ăV�F�[�_�[�ɓn���j
   MatrixBuffer mb = { DirectX::XMMatrixTranspose(world), DirectX::XMMatrixTranspose(m_graphics->GetViewMatrix()), DirectX::XMMatrixTranspose(m_graphics->GetProjectionMatrix())};
   D3D11_MAPPED_SUBRESOURCE mapped;
   context->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &mb, sizeof(mb));
   context->Unmap(m_matrixBuffer.Get(), 0);

   // ���C�g�o�b�t�@���X�V
   context->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &m_lightParams, sizeof(m_lightParams));
   context->Unmap(m_lightBuffer.Get(), 0);

   // �|�C���g���C�g�o�b�t�@���X�V
   context->Map(m_pointLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &m_pointLightParams, sizeof(m_pointLightParams));
   context->Unmap(m_pointLightBuffer.Get(), 0);

   // �t�H�O�o�b�t�@���X�V
   FogBuffer fb = { FOG_COLOR, FOG_START, FOG_END, {0.0f, 0.0f}, m_camera->GetEyePosition() };
   context->Map(m_fogBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &fb, sizeof(fb));
   context->Unmap(m_fogBuffer.Get(), 0);

   //uv�X�P�[���o�b�t�@�̍X�V
   UVScaleBuffeer uv = { uvScale };
   context->Map(m_uvScaleBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &uv, sizeof(uv));
   context->Unmap(m_uvScaleBuffer.Get(), 0);
}

/// <summary>
/// �V�F�[�_�[�̓ǂݍ��݁E��������
/// </summary>
/// <param name="device">�f�o�C�X</param>
void CustomModelRenderer::LoadShaders(ID3D11Device* device)
{
   Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

   // ���_�V�F�[�_�[���R���p�C��
   HRESULT hr = D3DCompileFromFile(
      L"Resources/Shaders/FogPointLight/FogPointLightVS.hlsl",
      nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
      "main",
      "vs_5_0",
      0, 0, &vsBlob, &errorBlob
   );
   if (FAILED(hr)) {
      if (errorBlob) {
         OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      }
      throw std::runtime_error("VS compile error");
   }

   // �s�N�Z���V�F�[�_�[���R���p�C��
   hr = D3DCompileFromFile(
      L"Resources/Shaders/FogPointLight/FogPointLightPS.hlsl",
      nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
      "main",
      "ps_5_0",
      0, 0, &psBlob, &errorBlob
   );
   if (FAILED(hr)) {
      if (errorBlob) {
         OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      }
      throw std::runtime_error("PS compile error");
   }

   // �V�F�[�_�[�I�u�W�F�N�g����
   device->CreateVertexShader(
      vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
   device->CreatePixelShader(
      psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

   // ���̓��C�A�E�g�����iPOSITION/NORMAL/TEXCOORD�̒��_�\���ɑΉ��j
   D3D11_INPUT_ELEMENT_DESC layout[] = {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
   };
   device->CreateInputLayout(
      layout, 4,
      vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
      m_inputLayout.GetAddressOf());

   //�o�b�t�@�[�𐶐�����
   CreateBuffer(device);
}

/// <summary>
/// �o�b�t�@�𐶐�����
/// </summary>
/// <param name="device">�f�o�C�X</param>
void CustomModelRenderer::CreateBuffer(ID3D11Device* device)
{
   // �萔�o�b�t�@�����i�s��o�b�t�@)
   D3D11_BUFFER_DESC mbDesc = {};
   mbDesc.ByteWidth = sizeof(MatrixBuffer);
   mbDesc.Usage = D3D11_USAGE_DYNAMIC;
   mbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   mbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&mbDesc, nullptr, m_matrixBuffer.GetAddressOf());

   //���C�g�o�b�t�@
   D3D11_BUFFER_DESC lbDesc = {};
   lbDesc.ByteWidth = sizeof(LightBuffer);
   lbDesc.Usage = D3D11_USAGE_DYNAMIC;
   lbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   lbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&lbDesc, nullptr, m_lightBuffer.GetAddressOf());

   //�|�C���g���C�g�o�b�t�@
   D3D11_BUFFER_DESC plDesc = {};
   plDesc.ByteWidth = sizeof(PointLightBuffer);
   plDesc.Usage = D3D11_USAGE_DYNAMIC;
   plDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   plDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&plDesc, nullptr, m_pointLightBuffer.GetAddressOf());

   //�t�H�O�o�b�t�@
   D3D11_BUFFER_DESC fbDesc = {};
   fbDesc.ByteWidth = sizeof(FogBuffer);
   fbDesc.Usage = D3D11_USAGE_DYNAMIC;
   fbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   fbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&fbDesc, nullptr, m_fogBuffer.GetAddressOf());

   //UV�o�b�t�@
   D3D11_BUFFER_DESC uvDesc = {};
   uvDesc.ByteWidth = sizeof(UVScaleBuffeer);
   uvDesc.Usage = D3D11_USAGE_DYNAMIC;
   uvDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   uvDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&uvDesc, nullptr, m_uvScaleBuffer.GetAddressOf());
}

/// <summary>
/// ���C�g�p�����[�^��ݒ肷��
/// </summary>
/// <param name="direction">���C�g�̕���</param>
/// <param name="diffuse">���C�g�̃f�B�t���[�Y</param>
/// <param name="ambient">�A���r�G���g�J���[</param>
void CustomModelRenderer::SetLightParameters(
   const DirectX::SimpleMath::Vector3& direction,
   const DirectX::SimpleMath::Vector4& diffuse,
   const DirectX::SimpleMath::Vector4& ambient)
{
   m_lightParams.LightDirection = direction;
   m_lightParams.LightDiffuseColor = diffuse;
   m_lightParams.AmbientColor = ambient;
}


/// <summary>
/// ���f���̒��S�ʒu����߂����C�g��I��
/// </summary>
/// <param name="modelPos">���f���̒��S���W</param>
/// <param name="maxLights">�V�F�[�_�[�ɓn�����C�g�̍ő吔</param>
void CustomModelRenderer::SetClosestLights(const DirectX::SimpleMath::Vector3& modelPos, int maxLights)
{
   //�|�C���g���C�g�̃p�����[�^���N���A����
   ClearPointLightsParameter();

   // �����Ń\�[�g���邽�߂̈ꎞ�z��
   struct LightInfo
   {
      PointLight light;
      float distance;
   };
   std::vector<LightInfo> visibleLights;

   //��������쐬����
   DirectX::BoundingFrustum frustum = m_frustum->CreateViewFrustum(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

   //������J�����O�Ƌ����v�Z
   for (const auto& light : m_allPointLights)
   {
      //���C�g�̉e���͈͂��l�������o�E���f�B���O�X�t�B�A
      DirectX::BoundingSphere sphere;
      sphere.Center = light.position;
      sphere.Radius = light.range;

      //������Ƃ̌�������
      if (m_frustum->IsVisible(frustum, sphere))
      {
         float distance = (modelPos - light.position).Length();
         visibleLights.push_back({ light, distance });
      }
   }

   //�����Ń\�[�g(����)
   std::sort(visibleLights.begin(), visibleLights.end(), []
   (const LightInfo& a, const LightInfo& b) {
         return a.distance < b.distance;
      });

   //�n���|�C���g���C�g�̐��𐧌�
   int count = std::min(maxLights, (int)visibleLights.size());
   m_pointLightParams.numLights = count;
   for (int i = 0; i < count; i++)
   {
         m_pointLightParams.pointLights[i] = visibleLights[i].light;
   }
}

/// <summary>
/// �|�C���g���C�g�̃p�����[�^��ݒ肷��
/// </summary>
/// <param name="lights">�ݒ肷��|�C���g���C�g</param>
void CustomModelRenderer::SetPointLightParameters(const PointLight& light)
{
   m_allPointLights.push_back(light);
}

/// <summary>
/// �|�C���g���C�g�̃p�����[�^���N���A����
/// </summary>
void CustomModelRenderer::ClearPointLightsParameter()
{
   for (int i = 0; i < MAX_POINT_LIGHT; i++)
   {
      m_pointLightParams.pointLights[i] = PointLight();
   }
   m_pointLightParams.numLights = 0;
}

/// <summary>
/// �S�Ẵ|�C���g���C�g���N���A����
/// </summary>
void CustomModelRenderer::ClearAllPointLights()
{
   m_allPointLights.clear();
}

void CustomModelRenderer::UpdateShadowMap(const DirectX::Model& model)
{
   UNREFERENCED_PARAMETER(model);
   //auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

   //// �|�C���g���C�g���ƂɃL���[�u�}�b�v���X�V
   //for (int i = 0; i < m_pointLightParams.numLights; ++i)
   //{
   //   //���C�g���擾����
   //   const auto& light = m_pointLightParams.pointLights[i];
   //   //m_cubeMapShadow->UpdateCubeMap(
   //   //   context,
   //   //   light.position,
   //   //   light.range,
   //   //   model
   //   //);
   //}
}