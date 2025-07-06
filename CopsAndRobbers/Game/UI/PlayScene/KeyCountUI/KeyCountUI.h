/*
	@file	KeyCountUI.h
	@brief	���̐��\��UI�N���X
*/
#pragma once
#ifndef KEY_COUNT_UI_DEFINED
#define KEY_COUNT_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;
class GameManager;

class KeyCountUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 KEY_ICON_POSITION = { 35.0f, 230.0f };	  //���̃A�C�R���`����W
   static constexpr int DIGIT_WIDTH = 57;
   static constexpr int DIGIT_HEIGHT = 98;
   static constexpr int SPACING = 5;
   static constexpr DirectX::SimpleMath::Vector2 KEY_NUM_POSITION = { 120.0f, 230.0f }; // �A�C�R���̉E���Ȃ�

public:
    //�R���X�g���N�^
    KeyCountUI();
	//�f�X�g���N�^
	~KeyCountUI();

	//����������
	void Initialize();
	//�X�V����
	void Update();
	//�`�悷��
	void Render();
	//��n������
	void Finalize();

private:
   //���\�[�X��ǂݍ���
   void LoadResources();
   //���̐���`�悷��
   void DrawKeyCount();

private:
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;

	//�X�^�~�i�Q�[�W�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_miniMapTexture;
	//�^�C�}�[�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;

	//�������Ă��錮�̐�
	int m_keyNum;
};
#endif // KEY_COUNT_UI_DEFINED
