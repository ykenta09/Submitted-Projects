/*
	@file	CustomModelRenderer.h
	@brief	�J�X�^�����f�������_�[�N���X
*/
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Libraries/MyLib/RenderTexture.h"
#include "Libraries/MyLib/DepthStencil.h"

#ifndef CUBE_MAP_SHADOW_DEFINED
#define CUBE_MAP_SHADOW_DEFINED

class CommonResources;
namespace yamadalib { class Graphics; } // ���O���錾

class CubeMapShadow
{
public:
   // �V���h�E�}�b�v�̃T�C�Y
   static const int SHADOWMAP_SIZE = 512;

   // ���C�g�̒萔�o�b�t�@�p�\����
   struct cbLight
   {
	  DirectX::XMMATRIX lightViewProjection;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
	  DirectX::XMVECTOR lightPosition;		// ���C�g�̈ʒu
	  DirectX::XMVECTOR lightDirection;		// ���C�g�̕���
	  DirectX::XMVECTOR lightAmbient;			// ���C�g�̊���
   };

public:
	//�R���X�g���N�^
    CubeMapShadow(CommonResources* resources);
   //�f�X�g���N�^
	~CubeMapShadow();


	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �L���[�u�}�b�v���X�V�i�����ʒu����6�����ɕ`��j
	void UpdateCubeMap(
	   ID3D11DeviceContext* context,
	   const DirectX::XMFLOAT3& lightPos,
	   float lightRadius,
	   const DirectX::Model& model);

	void LoadShader(ID3D11Device* device);

	void CreateConstanBuffer(ID3D11Device* device);

	void CreateShadowMapObject(ID3D11Device* device);

	void Release();

	void LoadResources();

private:
   void InitializeResources();


private:
   // ���ʃ��\�[�X
   CommonResources* m_commonResources;
   //�O���t�B�b�N�X
   yamadalib::Graphics* m_graphics;

   // �萔�o�b�t�@�ւ̃|�C���^
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

   // ���_�V�F�[�_
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;

   // �s�N�Z���V�F�[�_
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;

   // �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
   std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

   // �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
   std::unique_ptr<DepthStencil> m_shadowMapDS;

   // ���_�V�F�[�_(�V���h�E�}�b�v�p�j
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vsShadowMap;

   // �s�N�Z���V�F�[�_�i�V���h�E�}�b�v�p�j
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psShadowMap;

   // �T���v��
   Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;


   // ���C�g�̈ʒu
   DirectX::SimpleMath::Vector3 m_lightPosition;

   // ���C�g�̉�]
   DirectX::SimpleMath::Quaternion m_lightQuaternion;

   //ID3D11Device* m_device;
   //ID3D11DeviceContext* m_context;


   // �r���[�s��
   DirectX::SimpleMath::Matrix m_view;

   // �ˉe�s��
   DirectX::SimpleMath::Matrix m_proj;


   //���ϐ�
   DirectX::Model* m_wallModel;

   // ���C�g�̉�p�F���C�g���猩�����i���f���J�����̉�p
   float m_lightTheta;


   DirectX::SimpleMath::Vector3 m_wallPosition;

};
#endif // CUBE_MAP_SHADOW_DEFINED

