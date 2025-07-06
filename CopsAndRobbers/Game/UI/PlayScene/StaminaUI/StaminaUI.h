/*
	@file	StaminaUI.h
	@brief	�X�^�~�iUI�N���X
*/
#pragma once
#ifndef STAMINA_UI_DEFINED
#define STAMINA_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;
class Player;

class StaminaUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 STAMINA_POSITION = DirectX::SimpleMath::Vector2(10, 70);						 //�X�^�~�i�̃e�N�X�`�����W
   static constexpr RECT STAMINA_RECT = { 0, 0, 363, 60 };			 //�X�^�~�i�̃e�N�X�`���؂���
public:
    //�R���X�g���N�^
    StaminaUI(Player* player);
	//�f�X�g���N�^
	~StaminaUI();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();
private:
   //���\�[�X��ǂݍ���
   void LoadResources();
   //�X�^�~�iUI	
   void DrawStaminaUI();

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
	//�L�����N�^�[�̃|�C���^
	Player* m_player;

	//�X�^�~�i�Q�[�W�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_staminaGauge;
};
#endif // STAMINA_UI_DEFINED
