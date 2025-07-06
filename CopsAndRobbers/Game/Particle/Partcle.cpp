//--------------------------------------------------------------------------------------
//	File: Particle.h
//
//	�p�[�e�B�N���N���X
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Game/Particle/Partcle.h"

#include "Game/Particle/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

///	<summary>
///	�C���v�b�g���C�A�E�g
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

///	<summary>
///	�R���X�g���N�^
///	</summary>
Particle::Particle()
   : m_pDR(nullptr)
   , time{ 0.0f }
{
}

///	<summary>
///	�f�X�g���N�^
///	</summary>
Particle::~Particle()
{
}

///	<summary>
///	�e�N�X�`�����\�[�X�ǂݍ��݊֐�
///	</summary>
///	<param name="path">���΃p�X(Resources/Textures/�E�E�E.png�Ȃǁj</param>
void Particle::LoadTexture(const wchar_t* path)
{
   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
   DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

   m_texture.push_back(texture);
}

///	<summary>
///	�����֐�
///	</summary>
///	<param name="pDR">���[�U�[���\�[�X�����玝���Ă���</param>
void Particle::Create(DX::DeviceResources* pDR)
{
   m_pDR = pDR;
   ID3D11Device1* device = pDR->GetD3DDevice();

   //	�V�F�[�_�[�̍쐬
   CreateShader();

   //	�v���~�e�B�u�o�b�`�̍쐬
   m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

   m_states = std::make_unique<DirectX::CommonStates>(device);

}

///	<summary>
///	Shader�쐬�����������������֐�
///	</summary>
void Particle::CreateShader()
{
   ID3D11Device1* device = m_pDR->GetD3DDevice();

   //	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
   BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
   BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
   BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

   //	�C���v�b�g���C�A�E�g�̍쐬
   device->CreateInputLayout(&INPUT_LAYOUT[0],
	  static_cast<UINT>(INPUT_LAYOUT.size()),
	  VSData.GetData(), VSData.GetSize(),
	  m_inputLayout.GetAddressOf());

   //	���_�V�F�[�_�쐬
   if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
   {//	�G���[
	  MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
	  return;
   }

   //	�W�I���g���V�F�[�_�쐬
   if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
   {//	�G���[
	  MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
	  return;
   }
   //	�s�N�Z���V�F�[�_�쐬
   if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
   {//	�G���[
	  MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
	  return;
   }

   //	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
   D3D11_BUFFER_DESC bd;
   ZeroMemory(&bd, sizeof(bd));
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof(ConstBuffer);
   bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bd.CPUAccessFlags = 0;
   device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

///	<summary>
///	�`��֐�
///	</summary>
///	<param name="view">�r���[�s��</param>
///	<param name="proj">�ˉe�s��</param>
void Particle::Render()
{
   time += 0.025f;

   if (time >= 1.0f)
   {
	  m_isFadeEnd = true;
   }

   ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();

   //	���_���(�|���S���̂S���_�̍��W���j
   DirectX::VertexPositionColorTexture vertex[4] =
   {
	   DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f,  0.0f, 0.0f), DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
   };

   //	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
   ConstBuffer cbuff;
   cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
   cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
   cbuff.time = DirectX::SimpleMath::Vector4(time, 0, 0, 0);

   //	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
   context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

   //	�V�F�[�_�[�Ƀo�b�t�@��n��
   ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
   context->VSSetConstantBuffers(0, 1, cb);
   context->GSSetConstantBuffers(0, 1, cb);
   context->PSSetConstantBuffers(0, 1, cb);

   //	�摜�p�T���v���[�̓o�^
   ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
   context->PSSetSamplers(0, 1, sampler);

   //	�������`��w��
   ID3D11BlendState* blendstate = m_states->NonPremultiplied();

   //	�������菈��
   context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

   //	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
   context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

   //	�J�����O�͍�����
   context->RSSetState(m_states->CullNone());

   //	�V�F�[�_���Z�b�g����
   context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
   context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
   context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

   //	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
   for (int i = 0; i < m_texture.size(); i++)
   {
	  context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
   }

   //	�C���v�b�g���C�A�E�g�̓o�^
   context->IASetInputLayout(m_inputLayout.Get());

   //	�|���S����`��
   m_batch->Begin();
   m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
   m_batch->End();

   //	�V�F�[�_�̓o�^���������Ă���
   context->VSSetShader(nullptr, nullptr, 0);
   context->GSSetShader(nullptr, nullptr, 0);
   context->PSSetShader(nullptr, nullptr, 0);

}
