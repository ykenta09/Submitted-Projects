/*
	@file	MiniMapUI.h
	@brief	�~�j�}�b�vUI�N���X
*/
#pragma once
#ifndef MINIMAP_UI_DEFINED
#define MINIMAP_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Libraries/MyLib/RenderTexture.h"

// �O���錾
class CommonResources;

class MiniMapUI
{
public:	 //�萔
   static constexpr DirectX::SimpleMath::Vector3 MINIMAP_MASK_VERTEX = { 4.48f, 1.37f, 0.0f };	    //�~�j�}�b�v�}�X�N�̒��_���W
   static constexpr DirectX::SimpleMath::Vector3 MINIMAP_MASK_SCALE = { 0.177f, 0.32f, 1.0f };	    //�~�j�}�b�v�}�X�N�̃X�P�[��
   static constexpr DirectX::SimpleMath::Vector2 PLAYER_FELLOW_ICON_SCALE = { 0.8f, 0.8f };			//�~�j�}�b�v�̃L�����N�^�[�A�C�R���̃T�C�Y
   static constexpr DirectX::SimpleMath::Vector2 BACK_TEXTURE_POSITION = { 1150.0f, 200.0f };	    //�~�j�}�b�v�̔w�i�e�N�X�`���̕`����W
   static constexpr DirectX::SimpleMath::Vector2 PLAYER_TEXTURE_POSITION = { 0.0f, 0.0f };			//�v���C���[�e�N�X�`���̏������W
   static constexpr DirectX::SimpleMath::Vector2 MINIMAP_SCALE = { 0.8f, 0.8f };					//�~�j�}�b�v�̃X�P�[��
   static constexpr DirectX::SimpleMath::Vector2 RENDER_TEXTURE_SIZE = { 310.0f, 300.0f };			//�����_�[�e�N�X�`���̃T�C�Y
   static constexpr DirectX::SimpleMath::Vector2 CIRCLE_POSITION_OFSET = { 120.0f, 120.0f };	    //�~�`�e�N�X�`���̍��W�␳

   static constexpr float MAP_SPACE = 2.5f;														   //�}�b�v�̕`��X�P�[��
   static constexpr float MAP_SCALE_X_DIVISOR = 13.0f;											   //�}�b�v�I�u�W�F�N�g�̃X�P�[���␳
   static constexpr float MAP_SCALE_Z_DIVISOR = 14.0f;											   //�}�b�v�I�u�W�F�N�g�̃X�P�[���␳
   static constexpr float GOAL_ICON_OFSET = 10.0f;												   //�S�[���A�C�R���̍��W�␳
   static constexpr float CIRCLE_SCALE = 1.5f;													   //�~�`�e�N�X�`���̃X�P�[���␳
   static constexpr float MAX_ICON_SCALE = 5.0f;												   //�}�b�v�A�C�R���̃X�P�[���Œ�l
   static constexpr float MIN_ICON_SCALE = 0.1f;												   //�}�b�v�A�C�R���̃X�P�[���ő�l
   static constexpr RECT BACK_RECT = { 0, 0 , 313, 305 };										   //�}�b�v�w�i�̃e�N�X�`���؂蔲�����W
   static constexpr RECT OBJECT_RECT = { 317, 38 , 343, 68 };									   //�}�b�v�I�u�W�F�N�g�̃e�N�X�`���؂蔲�����W
   static constexpr RECT PLAYER_RECT = { 317, 10 , 340, 30 };									   //�v���C���[�̃e�N�X�`���؂蔲�����W
   static constexpr RECT ENEMY_RECT = { 317, 76 , 338, 106 };									   //�G�̃e�N�X�`���؂蔲�����W
   static constexpr RECT FELLOW_RECT = { 317, 102 , 343, 132 };									   //���Ԃ̃e�N�X�`���؂蔲�����W
   static constexpr RECT GOAL_RECT = { 317, 155 , 343, 175 };									   //�S�[���̃e�N�X�`���؂蔲�����W
public:
   //	�֐�
   static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:
   //�R���X�g���N�^
   MiniMapUI(CommonResources* m_commonResources, Player* player, Fellow* fellow, Map* map);
   //�f�X�g���N�^
	~MiniMapUI();

	//����������
	void Initialize();
	//�X�V����
	void Update();
	//�`�悷��
	void Render();
	//��n������
	void Finalize();

	//�V�F�[�_�[�̍쐬
	void CreateShader();

	//���[���h���W���~�j�}�b�v���W�ɕϊ�
	DirectX::SimpleMath::Vector2 WorldToMiniMap(
	   const DirectX::SimpleMath::Vector3& worldPos,
	   const DirectX::SimpleMath::Vector3& mapCenter,
	   float mapScale);

	//�~�j�}�b�v�w�i�̕`��
	void DrawMiniMapBack();
	//�I�u�W�F�N�g�`��
	void DrawMiniMapObject();
	//�ݐσe�N�X�`�����쐬����
	void UpdateCompositionTexture();
	//�����_�[�e�N�X�`���̏�����
	void InitializeCompositionTexture(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, const D3D11_VIEWPORT& viewport);
	//�V�F�[�_�[��`�悷��
	void RenderShader();

private:
   //�R���X�g�o�b�t�@
   struct ConstBuffer
   {
	  DirectX::SimpleMath::Matrix		matWorld;	  //���[���h�s��
	  DirectX::SimpleMath::Matrix		matView;	  //�r���[�s��
	  DirectX::SimpleMath::Matrix		matProj;	  //�v���W�F�N�V�����s��
	  DirectX::SimpleMath::Vector4		Diffuse;	  //�f�B�t���[�Y
   };

private:
	CommonResources* m_commonResources;		 // ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;		 //�O���t�B�b�N�X

	//�L�����N�^�[
	Player* m_player;				//�v���C���[
	Fellow* m_fellow;				//����
	Map* m_map;						//�}�b�v 

	//UI�w�i�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_miniMapBackTexture;			//�~�j�}�b�v�w�i�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_circleTexture;				//�}�X�N�p�̉~�`�e�N�X�`��

	//�e�N�X�`���̕`����W
	DirectX::SimpleMath::Vector2 m_circlePosition;					 // �~�`�e�N�X�`���̕`��ʒu
	DirectX::SimpleMath::Vector2 m_backTexturePosition;				 //�w�i�e�N�X�`���̍��W
	DirectX::SimpleMath::Vector2 m_playerTexturePosition;			 //�v���C���[�e�N�X�`���̕`����W
	DirectX::SimpleMath::Vector2 m_fellowTexturePosition;			 //���ԃe�N�X�`���̕`����W
	DirectX::SimpleMath::Vector2 m_mapTexturePosition;

	DirectX::SimpleMath::Vector3 m_miniMapCenter;		 //�~�j�}�b�v�̒��S�ƂȂ���W

	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	D3D11_VIEWPORT m_miniMapViewpor;						   //�~�j�}�b�v�p�r���[�|�[�g
	D3D11_VIEWPORT m_mainViewport;							   //���C����ʂ̃r���[�|�[�g

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;									//�X�v���C�g�o�b�`

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;			   //���̓��C�A�E�g

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;			    //���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;			    //�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;		   	//�W�I���g���V�F�[�_

	std::unordered_map<int, std::unique_ptr<DX::RenderTexture>> m_renderTexture;	//�����_�[�e�N�X�`��
	DirectX::SimpleMath::Vector2 m_renderTextureSize;								// �����_�[�e�N�X�`���̃T�C�Y

	bool m_isCompositionTextureInitialized;		//�����_�[�e�N�X�`���̏������t���O
	int m_currentStageNumber;					//���݂̃X�e�[�W�ԍ�
};
#endif // MINIMAP_UI_DEFINED
