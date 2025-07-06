/*
	@file	TimerUI.h
	@brief	�^�C�}�[UI�N���X
*/
#pragma once
#ifndef TIMER_UI_DEFINED
#define TIMER_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;
class Player;
class Fellow;
class Map;
class GameManager;

class TimerUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 TIMER_POSITION = DirectX::SimpleMath::Vector2(440, 0);							 //�^�C�}�[�̃e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 HINT_POSITION = DirectX::SimpleMath::Vector2(0.0f, 150.0f);					 //�q���g�e�N�X�`���̍��W
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_TOP_POSITION = DirectX::SimpleMath::Vector2(0.0f, -150.0f);		 //��̔w�i�e�N�X�`���̍��W
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_BOTTOM_POSITION = DirectX::SimpleMath::Vector2(0.0f, 650.0f);	 //���̔w�i�e�N�X�`���̍��W
   static constexpr RECT MOVE_RECT = { 0, 0 , 634, 80 };			 //����̃e�N�X�`���؂���
   static constexpr int DIGIT_WIDTH = 57;		   //�����̕�
   static constexpr int DIGIT_HEIGHT = 98;		   //�����̍���
   static constexpr int COLON_WIDTH = 24;		   //�R�����̕�
   static constexpr int COLON_HEIGHT = 96;		   //�R�����̍���
   static constexpr int SPACING = 5;			   //�Ԋu����
public:
    //�R���X�g���N�^
    TimerUI();
	//�f�X�g���N�^
	~TimerUI();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& playTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();

private:
   //�^�C�}�[UI
   void DrawTimerUI();
   //���\�[�X��ǂݍ���
   void LoadResources();
private:
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;

	//�^�C�}�[�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;

	//����
	float m_timer;
	//���Ԃ̕`����W
	DirectX::SimpleMath::Vector2 m_timerPosition;
};
#endif // TIMER_UI_DEFINED
