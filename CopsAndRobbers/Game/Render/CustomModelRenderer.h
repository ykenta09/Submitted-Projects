/*
	@file	CustomModelRenderer.h
	@brief	�J�X�^�����f�������_�[�N���X
*/
#pragma once
#include <Game/Interface/ICamera.h>
#include "CubeMapShadow.h"
#include  "Game/FrustumCuller.h"

#ifndef CUSTOM_MODEL_RENDERER_DEFINED
#define CUSTOM_MODEL_RENDERER_DEFINED

class CommonResources;
class CameraManager;
namespace yamadalib { class Graphics; } // ���O���錾

class CustomModelRenderer
{
public:
   static constexpr	 DirectX::SimpleMath::Vector4 FOG_COLOR = { 0.1f, 0.1f, 0.2f, 0.3f };	 //�t�H�O�̐F
   static constexpr	 float FOG_START = { 5.0f };		 //�t�H�O�̊J�n����
   static constexpr	 float FOG_END = { 25.0f };			 //�t�H�O�̏I������
   static constexpr int MAX_POINT_LIGHT = { 5 };		 //�V�F�[�_�[�ɑ��郉�C�g�̍ő吔
public:
   //�}�e���A���o�b�t�@
   struct MatrixBuffer
   {
	  DirectX::SimpleMath::Matrix World;
	  DirectX::SimpleMath::Matrix View;
	  DirectX::SimpleMath::Matrix Projection;
   };

   //���C�g�o�b�t�@
   struct LightBuffer
   {
	  DirectX::SimpleMath::Vector3 LightDirection; //���[���h��ԃ��C�g����
	  float padding;
	  DirectX::SimpleMath::Vector4 LightDiffuseColor;	 //���C�g�̃f�B�t���[�Y�F
	  DirectX::SimpleMath::Vector4 AmbientColor;		 //�A���r�G���g�F
   };

   //�|�C���g���C�g�̍\����
   struct PointLight
   {
	  DirectX::SimpleMath::Vector3 position = { 0.0f, 0.0f, 0.0f};
	  float range = 0.0f;
	  DirectX::SimpleMath::Vector3 color = { 0.0f, 0.0f, 0.0f };
	  float intensity = 1.5f;
   };

   //�|�C���g���C�g�o�b�t�@
   struct PointLightBuffer
   {
	  PointLight pointLights[MAX_POINT_LIGHT];
	  int numLights;
	  float padding[3];
   };
   //�t�H�O�o�b�t�@
   struct FogBuffer
   {
	  DirectX::SimpleMath::Vector4 FogColor;
	  float FogStart;
	  float FogEnd;
	  float padding[2];
	  DirectX::SimpleMath::Vector3 CameraPosition;
	  float padding2;
   };

   //UV�o�b�t�@
   struct UVScaleBuffeer
   {
	  DirectX::SimpleMath::Vector2 uvScale;
	  float padding[2];
   };

   //�J������ݒ肷��
   void SetCamera(ICamera* camera) { m_camera = camera; }

   CubeMapShadow* GetCubeMap() { return m_cubeMapShadow.get(); }
public:
	//�R���X�g���N�^
    CustomModelRenderer(CommonResources* resources, yamadalib::Graphics* graphics);
	 //�f�X�g���N�^
	~CustomModelRenderer();

	//���f���`��
	void Draw(const DirectX::Model& model, ID3D11ShaderResourceView* texture, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale = DirectX::SimpleMath::Vector2::One);

	//�s��E�o�b�t�@�̍X�V
	void UpdateConstantBuffers(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale);

	//�V�F�[�_�[��ǂݍ��݁A����
	void LoadShaders(ID3D11Device* device);

	//�o�b�t�@�𐶐�����
	void CreateBuffer(ID3D11Device* device);

	//���C�g�o�b�t�@�̃p�����[�^��ݒ肷��
	void SetLightParameters(const DirectX::SimpleMath::Vector3& direction, const DirectX::SimpleMath::Vector4& diffuse, const DirectX::SimpleMath::Vector4& ambient);

	//�|�C���g���C�g�̃p�����[�^��ݒ肷��
	void SetPointLightParameters(const PointLight& lights);

	//�|�C���g���C�g���N���A����
	void SetClosestLights(const DirectX::SimpleMath::Vector3& modelPos, int maxLights);

	//�|�C���g���C�g�̃p�����[�^���N���A����
	void ClearPointLightsParameter();

	//�S�Ẵ|�C���g���C�g���N���A����
	void ClearAllPointLights();

	void UpdateShadowMap(const DirectX::Model& model);
private:
   // ���ʃ��\�[�X
   CommonResources* m_commonResources;
   //�O���t�B�b�N�X
   yamadalib::Graphics* m_graphics;
   //�J����
   ICamera* m_camera;

   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;		 //���_�V�F�[�_�[
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;			 //�s�N�Z���V�F�[�_�[
   Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;			 //���̓��C�A�E�g
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;				 //�}�e���A���o�b�t�@
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;				 //���C�g�o�b�t�@
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_pointLightBuffer;			 //�|�C���g���C�g�o�b�t�@
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_fogBuffer;				 //�t�H�O�o�b�t�@
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_uvScaleBuffer;			 //uv�o�b�t�@

   LightBuffer m_lightParams;				 // ���C�g�p�����[�^�ێ��p
   PointLightBuffer m_pointLightParams;		 //�|�C���g���C�g�p�����[�^
   std::vector<PointLight> m_allPointLights; //�S�Ẵ|�C���g���C�g�z��

   std::unique_ptr<FrustumCuller> m_frustum;	//������J�����O


   std::unique_ptr<CubeMapShadow> m_cubeMapShadow;
};
#endif // CUSTOM_MODEL_RENDERER_DEFINED

