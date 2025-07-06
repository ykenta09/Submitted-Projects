#include "pch.h"
#include "CubeMapShadow.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include <Libraries/Microsoft/ReadData.h>
#include <d3dcompiler.h>

//���w�b�_�[
#include "Libraries/yamadaLib/Resources.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="device">�f�o�C�X</param>
/// <param name="size">�L���[�u�}�b�v�̃T�C�Y</param>
CubeMapShadow::CubeMapShadow(CommonResources* resources)
   :
   m_commonResources(resources)
{
   InitializeResources();
}

CubeMapShadow::~CubeMapShadow()
{
   Release();
}

/// <summary>
/// ���\�[�X�̏�����
/// </summary>
void CubeMapShadow::InitializeResources()
{
   //���C�g�̈ʒu�i���j
   m_lightPosition = DirectX::SimpleMath::Vector3(5.0f, 5.0f, 0.0f);
   //���C�g�̉�]
   m_lightQuaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
      DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(45.0f), 0.0f);

   //���C�g�̊p�x
   m_lightTheta = 90.0f;

   m_wallPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

   auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
   LoadShader(device);
   CreateConstanBuffer(device);
   CreateShadowMapObject(device);
}

void CubeMapShadow::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
   auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
   auto states = m_commonResources->GetCommonStates();

   // ----------------------------------------------------------- //
   // ���C�g��Ԃ̃r���[�s��Ǝˉe�s����쐬����
   // ----------------------------------------------------------- //

   /* �菇�S */

   //���C�g�̌����Ă�������𒲐�����
   const DirectX::SimpleMath::Vector3 lightDir = DirectX::SimpleMath::Vector3::Transform(
	  DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightQuaternion);

   //���C�g��Ԃ̃r���[�s����v�Z����
   const DirectX::SimpleMath::Matrix lightView = DirectX::SimpleMath::Matrix::CreateLookAt(
	  m_lightPosition,
	  m_lightPosition + lightDir,
	  DirectX::SimpleMath::Vector3::UnitY
   );

   //���C�g��Ԃ̎ˉe�s����v�Z����
   const DirectX::SimpleMath::Matrix lightProjection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
	  DirectX::XMConvertToRadians(m_lightTheta),
	  1.0f,
	  0.1f,
	  100.0f
   );

   // ----------------------------------------------------------- //
   // �萔�o�b�t�@���X�V����
   // ----------------------------------------------------------- //

   /* �菇�T */

   //GPU���g�p���郊�\�[�X�̃�������CPU����A�N�Z�X����ۂɗ��p����\����
   D3D11_MAPPED_SUBRESOURCE mappedResource;

   //�萔�o�b�t�@���}�b�v����
   context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

   //�萔�o�b�t�@�ɑ���f�[�^����������
   cbLight* cb = static_cast<cbLight*>(mappedResource.pData);
   const DirectX::SimpleMath::Matrix viewProj = lightView * lightProjection;
   cb->lightViewProjection = viewProj;
   cb->lightPosition = m_lightPosition;
   cb->lightDirection = lightDir;
   cb->lightAmbient = DirectX::SimpleMath::Color(0.2f, 0.2f, 0.2f);

   //�}�b�v����������
   context->Unmap(m_constantBuffer.Get(), 0);

   // ----------------------------------------------------------- //
   // Pass1 �`��^�[�Q�b�g��؂�ւ��āA�V���h�E�}�b�v���쐬����
   // ----------------------------------------------------------- //

   /* �菇�U */

   //�K�v�ȁ���view���擾����
   auto rtv = m_shadowMapRT->GetRenderTargetView();
   //auto srv = m_shadowMapRT->GetShaderResourceView();
   auto dsv = m_shadowMapDS->GetDepthStencilView();

   //�V���h�E�}�b�v�p��RTV��DSV���N���A����
   context->ClearRenderTargetView(rtv, DirectX::Colors::White);
   context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

   //RTV��DSV���V���h�E�}�b�v�p�ɕύX����
   context->OMSetRenderTargets(1, &rtv, dsv);

   //�r���[�|�[�g���V���h�E�}�b�v�p�ɕύX����
   const D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
   context->RSSetViewports(1, &vp);

   // ----------------------------------------------------------- //
   // �e�ɂȂ郂�f����`�悷��
   // ----------------------------------------------------------- //

   /* �菇�V */
   DirectX::SimpleMath::Matrix world;

   //�����g�[���X
   world = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(7.0f, 10.0f, 1.0f)) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(m_wallPosition);
   m_wallModel->Draw(context, *states, world, lightView, lightProjection, false, [&]
	  {
		 context->VSSetShader(m_vsShadowMap.Get(), nullptr, 0);
		 context->PSSetShader(m_psShadowMap.Get(), nullptr, 0);
	  });

   //�����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
   auto renderTarget = m_commonResources->GetDeviceResources()->GetRenderTargetView();
   auto depthStencil = m_commonResources->GetDeviceResources()->GetDepthStencilView();

   context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
   context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
   context->OMSetRenderTargets(1, &renderTarget, depthStencil);
   auto const viewport = m_commonResources->GetDeviceResources()->GetScreenViewport();
   context->RSSetViewports(1, &viewport);

   // === �ʏ�`�� ===
   m_wallModel->Draw(context, *states, world, view, proj, false, [&]()
	  {

	  });

}

/// <summary>
/// �V�F�[�_�����[�h����
/// </summary>
/// <param name="device">�f�o�C�X</param>
void CubeMapShadow::LoadShader(ID3D11Device* device)
{
   // �V�F�[�_��ǂݍ��ނ��߂�blob
   std::vector<uint8_t> blob;

   // �V���h�E�}�b�v�p���_�V�F�[�_�����[�h����
   blob = DX::ReadData(L"Resources/Shaders/ShadowMap/ShadowMapDepthVS.cso");
   DX::ThrowIfFailed(
      device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_vsShadowMap.ReleaseAndGetAddressOf())
   );

   // �V���h�E�}�b�v�p�s�N�Z���V�F�[�_�����[�h����
   blob = DX::ReadData(L"Resources/Shaders/ShadowMap/ShadowMapDepthPS.cso");
   DX::ThrowIfFailed(
      device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_psShadowMap.ReleaseAndGetAddressOf())
   );

}

/// <summary>
/// �萔�o�b�t�@�����
/// </summary>
/// <param name="device">�f�o�C�X</param>
void CubeMapShadow::CreateConstanBuffer(ID3D11Device* device)
{
   //�萔�o�b�t�@�p�̃o�b�t�@�I�u�W�F�N�g�����
   D3D11_BUFFER_DESC bufferDesc{};
   bufferDesc.ByteWidth = static_cast<UINT>(sizeof(cbLight));
   bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
   bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   DX::ThrowIfFailed(
      device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf())
   );
}

/// <summary>
/// �V���h�E�}�b�v�p�̊e��I�u�W�F�N�g���쐬����
/// </summary>
/// <param name="device">�f�o�C�X</param>
void CubeMapShadow::CreateShadowMapObject(ID3D11Device* device)
{
   //�V���h�E�}�b�v�̑傫�����w�肷��
   const RECT rectShadow = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

   //�����_�[�e�N�X�`�����쐬����
   m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
   m_shadowMapRT->SetDevice(device);
   m_shadowMapRT->SetWindow(rectShadow);

   //�[�x�X�e���V�����쐬����
   m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
   m_shadowMapDS->SetDevice(device);
   m_shadowMapDS->SetWindow(rectShadow);

   //�V���h�E�}�b�v�p�̃T���v���[���쐬����
   D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
   samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
   samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
   DX::ThrowIfFailed(
      device->CreateSamplerState(&samplerDesc, m_shadowMapSampler.ReleaseAndGetAddressOf())
   );
}

void CubeMapShadow::Release()
{
}

void CubeMapShadow::LoadResources()
{
   m_wallModel = yamadalib::Resources::GetInstance()->GetModel("Wall_01");
}

/// <summary>
/// �L���[�u�}�b�v���X�V(6�ʕ������_�����O)
/// </summary>
/// <param name="context"></param>
/// <param name="lightPos"></param>
/// <param name="lightRadius"></param>
/// <param name="model"></param>
//void CubeMapShadow::UpdateCubeMap(
//   ID3D11DeviceContext* context,
//   const DirectX::XMFLOAT3& lightPos,
//   float lightRadius,
//   const DirectX::Model& model)
//{
//   //�V���h�E�}�b�v�p��DSC(�[�x�X�e���V���r���[)��ʓr�p��
//   auto dsv = m_shadowMapDS->GetDepthStencilView();
//   auto rtv = m_shadowMapRT->GetRenderTargetView();
//
//   //�V���h�E�}�b�v�`��p�X
//   context->ClearRenderTargetView(rtv, DirectX::Colors::White);
//   context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
//   context->OMSetRenderTargets(1, &rtv, dsv);
//
//   D3D11_VIEWPORT vp = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
//   context->RSSetViewports(1, &vp);
//
//   context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
//   context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);)
//
//   // 6�����̃r���[�E�v���W�F�N�V�����s����쐬
//
//   // ���f���`��i���b�V�����j
//      for (const auto& mesh : model.meshes)
//      {
//         for (const auto& part : mesh->meshParts)
//         {
//            UINT stride = part->vertexStride;
//            UINT offset = 0;
//            context->IASetVertexBuffers(0, 1, part->vertexBuffer.GetAddressOf(), &stride, &offset);
//            context->IASetIndexBuffer(part->indexBuffer.Get(), part->indexFormat, 0);
//            context->DrawIndexed(part->indexCount, part->startIndex, part->vertexOffset);
//         }
//      }
//
//
//   //// ����DSV�ɖ߂�
//   //context->OMSetRenderTargets(0, nullptr, oldDSV);
//   //if (oldDSV) oldDSV->Release();
//}
