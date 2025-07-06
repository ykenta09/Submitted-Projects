/*
	@file	MissionMessageUI.h
	@brief	�~�b�V�������b�Z�[�WUI�N���X
*/
#pragma once
#ifndef MISSION_MESSAGE_UI_DEFINED
#define MISSION_MESSAGE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;

class MissionMessageUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 MISSION_START_POSITION = { -50.0f, -400.0f };  // �����ʒu (��ʊO�����ɐݒ�)
   static constexpr DirectX::SimpleMath::Vector2 MISSION_END_POSITION = { -50.0f, 250.0f };     // �ŏI�ʒu (�����̕`��ʒu)
   static constexpr DirectX::SimpleMath::Vector2 TARGET_START_POSITION = { -1700.0f, 360.0f };	 //�J�n�ʒu
   static constexpr DirectX::SimpleMath::Vector2 TARGET_END_POSITION = { -80.0f, 360.0f };		 //�I���ʒu

   static constexpr DirectX::SimpleMath::Vector2 MISSION_START_BLACK_POSITION = { 0.0f, -360.0f };   //�����e�N�X�`���̍��W
   static constexpr DirectX::SimpleMath::Vector2 MISSION_START_TEXT_POSITION = { 150.0f, 100.0f };  // �����ʒu (��ʊO�����ɐݒ�)
   static constexpr DirectX::SimpleMath::Vector2 TARGET_TEXT_POSITION = { 220.0f, 370.0f };		 //�~�b�V�����e�L�X�g�̍��W
   static constexpr DirectX::SimpleMath::Vector2 TARGET_BLACK_POSITION = { -2000.0f, 360.0f };		 //�~�b�V����UI�̔w�i�e�N�X�`���̍��W

   static constexpr DirectX::SimpleMath::Vector2 TEXT_SPACE_POSITION = { 600.0f, 0.0f };
   static constexpr DirectX::SimpleMath::Vector2 TEXT_BACKGROUND_OFFSET = { -50.0f, -50.0f };
   static constexpr DirectX::SimpleMath::Vector2 DRAW_MISSION_OFFSET = { 100.0f, 100.0f };
   static constexpr DirectX::SimpleMath::Vector2 DRAW_MISSION_COMPLETE_OFFSET = { 600.0f, 100.0f };
   static constexpr DirectX::SimpleMath::Vector2 DRAW_MISSION_FAILED_OFFSET = { 700.0f, 100.0f };
   static constexpr float TRANSITION_ELAPED_TIME = 0.0f;		  //�o�ߎ���
   static constexpr float TRANSITION_DURATION = 1.0f;			  //�A�j���[�V�����S�̂̎���
   static constexpr float STANBY_TIME = 1.5f;					  //�ҋ@����
   static constexpr float MISSION_MOVE_START = 0.4f;				 //�~�b�V�����̓����̊J�n����
   static constexpr float MISSION_MOVE_END = 0.6f;					 //�~�b�V�����̓����̏I������
   static constexpr float DRAW_TARGET_TIME = 1.0f;
   static constexpr float DRAW_RESULT_TIME = 2.0f;

   static constexpr RECT MISSION_RECT = { 0, 260 , 830, 400 };
   static constexpr RECT START_RECT = { 0, 380 , 810, 500 };
   static constexpr RECT COMPLETE_RECT = { 0, 510, 830, 650 };		 //�����̃e�N�X�`���؂���
   static constexpr RECT FAILED_RECT = { 0, 630, 830, 750 };		 //���s�̃e�N�X�`���؂���

public:
   //�B��Ă��邩����
   bool IsVisible() const { return m_isVisible; }
   //�Q�[���I��UI�̕`�悪�I��������
   bool GameFinishDraw() const { return m_isGameFinishDraw; }
public:
    //�R���X�g���N�^
    MissionMessageUI();
	//�f�X�g���N�^
	~MissionMessageUI();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();


	//�C�[�W���O�A�E�g
	float EaseOut(float t);
	//�A�j���[�V�����̏I���������s��
	void FinishTransition();
	//
	void ResetMissionUIProgress();
private:
   //���\�[�X��ǂݍ���
   void LoadResources();
   //�~�b�V�������ʂ�UI
   void DrawResultUI(bool isClear);
   //�~�b�V����UI�̓���
   void MoveMissionUI(const float& elapsedTime);
   //�~�b�V�����ڕW��`�悷��
   void MoveMissionTargetUI(const float& elapsedTime);
   //�~�b�V�����X�^�[�g��`�悷��
   void DrawMissionStart();
   //�~�b�V�����ڕW��`�悷��
   void DrawMissionTarget();
   //�A�j���[�V�����ҋ@���Ԃ��X�V����
   bool UpdatePauseTime(const float& elapsedTime);
   //UI�̈ʒu���X�V����
   void UpdatePosition(float t);
   //�C�[�W���O�i�s�x���X�V����
   float UpdateTransitionProgress(const float& elapsedTime);

private:
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
	//�~�b�V�����w�i�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionBack;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionTextTexture;
	//�e�N�X�`�����W
	DirectX::SimpleMath::Vector2 m_textTexturePosition;
	DirectX::SimpleMath::Vector2 m_textBackPosition;

	// === �~�b�V�����J�n ===
	bool m_isVisible;			 //UI�`��I��
	bool m_isMovingUp;			 // true: ��ɖ߂�, false: �������Ɉړ�
	float m_transitionElapsedTime; // �o�ߎ���
	float m_pauseTime;			 //�A�j���[�V�����ҋ@����
	bool m_isStartReset;

	// === �~�b�V�����I�� ===
	float m_deltaTime;		 //�o�ߎ���
	float m_drawResultTimer;	 //�~�b�V����������`�悷�鎞��
	bool m_isGameFinishDraw;	 //�Q�[���I�����b�Z�[�W�`��̏I���
	bool m_isEndReset;

	// === �~�b�V�����ڕW ===
	DirectX::SimpleMath::Vector2 m_targetTextBackPosition;
	float m_transitionDuration; // �A�j���[�V�����S�̂̎��ԁi1.5�b�j
	float m_delyTime;				   //�f�B���C�^�C��
	float m_drawTargetTimer;
	bool m_isMovingBack;
	bool m_isTextTextreVisible;		   //UI�`��I��
	bool m_isTargetVisible;
};
#endif // MISSION_MESSAGE_UI_DEFINED
