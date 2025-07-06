/*
	@file	MissionMessageUI.cpp
	@brief  �~�b�V�������b�Z�[�WUI�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/UI/PlayScene/MissionMessageUI/MissionMessageUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/GameManager/GameManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MissionMessageUI::MissionMessageUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_isMovingUp(false),
    m_isVisible(true),
    m_isGameFinishDraw(false),
    m_isMovingBack(false),
    m_isTextTextreVisible(false),
    m_isTargetVisible(true),
    m_isStartReset(false),
    m_isEndReset(false),
    m_pauseTime(0.0f),
    m_delyTime(1.5f),
    m_textTexturePosition(MISSION_START_POSITION),
    m_textBackPosition(MISSION_START_BLACK_POSITION),
    m_transitionElapsedTime(TRANSITION_ELAPED_TIME),
    m_targetTextBackPosition(TARGET_BLACK_POSITION),
    m_drawTargetTimer(DRAW_TARGET_TIME),
    m_drawResultTimer(DRAW_RESULT_TIME)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MissionMessageUI::~MissionMessageUI()
{
   Finalize();
}


/// <summary>
/// ����������
/// </summary>
void MissionMessageUI::Initialize()
{
   //���\�[�X��ǂݍ���
   LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void MissionMessageUI::Update(const float& elapsedTime)
{
   //�o�ߎ��Ԃ��i�[
   m_deltaTime = elapsedTime;

   //�Q�[���X�^�[�g���̂݃~�b�V�����ڕW��\��
   if (GameManager::GetInstance()->GetGameStart())
   {
      MoveMissionTargetUI(elapsedTime);
   }
   //�Q�[�����n�܂�����~�b�V�����X�^�[�g��\��
   if (GameManager::GetInstance()->GetGamePlay())
   {
      if (!m_isStartReset)
      {
         //�~�b�V����UI�̐i���x�����Z�b�g����
         ResetMissionUIProgress();
         m_isStartReset = true;
      }
      // UI���\������Ă��Ȃ��ꍇ�͏������I��
      if (!m_isVisible) return;
      //�~�b�V����UI�̓���
      MoveMissionUI(elapsedTime);
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void MissionMessageUI::Render()
{
   //�~�b�V�����ڕW��`�悷��
   DrawMissionTarget();
   //�~�b�V�����X�^�[�g
   DrawMissionStart();

   //�~�b�V��������
   if (GameManager::GetInstance()->GetGameClear())
   {
      DrawResultUI(true);
   }
   //�~�b�V�������s
   if (GameManager::GetInstance()->GetGameOver())
   {
      DrawResultUI(false);
   }
}

/// <summary>
/// ��n������
/// </summary>
void MissionMessageUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// �~�b�V�����X�^�[�g��`�悷��
/// </summary>
void MissionMessageUI::DrawMissionStart()
{
   // UI����\���̏ꍇ�͕`�悵�Ȃ�
   if (!m_isVisible)
      return;

	//�e�N�X�`����`�悷��
	m_graphics->DrawTexture(m_missionBack, m_textBackPosition, nullptr);
	m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + MISSION_START_TEXT_POSITION, &MISSION_RECT);
    m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + MISSION_START_TEXT_POSITION + TEXT_SPACE_POSITION, &START_RECT);
}

/// <summary>
/// �~�b�V�����ڕW��`�悷��
/// </summary>
void MissionMessageUI::DrawMissionTarget()
{
   //UI������ɕ`��I�������܂��̓Q�[�����n�܂�����`�悵�Ȃ�
   if (m_isTextTextreVisible) return;
   if (!GameManager::GetInstance()->GetGameStart()) return;
   //�e�N�X�`����`�悷��
   m_graphics->DrawTexture(m_missionBack, m_targetTextBackPosition, nullptr);
   if (m_isTargetVisible) return;
   m_graphics->DrawTexture(m_missionTextTexture, TARGET_TEXT_POSITION, nullptr);
}


/// <summary>
/// �~�b�V����UI�̓���
/// </summary>
void MissionMessageUI::MoveMissionUI(const float& elapsedTime)
{
   // �A�j���[�V�����ҋ@���Ԃ�����΁A�J�E���g�_�E�����ďI��
   if (UpdatePauseTime(elapsedTime)) return;

   // �C�[�W���O�i�s�x���X�V
   float progressRate = UpdateTransitionProgress(elapsedTime);
   float easedProgress = EaseOut(progressRate); // �C�[�W���O��K�p

   // UI�̈ʒu���X�V
   UpdatePosition(easedProgress);

   // �A�j���[�V�����I�����̏���
   // �i�s�x��1.0�ȏ�ɂȂ����ꍇ�́A�A�j���[�V�������I��
   if (easedProgress >= 1.0f)
   {
      FinishTransition();
   }
}

/// <summary>
/// �~�b�V�����ڕW�̈ړ�����
/// </summary>
void MissionMessageUI::MoveMissionTargetUI(const float& elapsedTime)
{
   //�`����B���Ă���Ȃ珈�����Ȃ�
   if (m_isTextTextreVisible) return;

   //�f�B���C�^�C���𖞂����Ă��Ȃ��Ȃ珈�����Ȃ�
   m_delyTime -= elapsedTime;
   m_drawTargetTimer -= elapsedTime;
   if (m_delyTime >= 0.0f) return;

   // �C�[�W���O�i�s�x���X�V
   float progressRate = UpdateTransitionProgress(elapsedTime);
   float easedProgress = EaseOut(progressRate); // �C�[�W���O��K�p

   if (m_isMovingBack)
   {
      m_targetTextBackPosition = DirectX::SimpleMath::Vector2::Lerp(TARGET_END_POSITION, TARGET_START_POSITION, easedProgress);
   }
   else
   {
      m_targetTextBackPosition = DirectX::SimpleMath::Vector2::Lerp(TARGET_START_POSITION, TARGET_END_POSITION, easedProgress);
   }

   if (m_drawTargetTimer <= 0)
   {
      m_isTargetVisible = true;
   }
   // �i�s�x��1.0�ȏ�ɂȂ����ꍇ�́A�A�j���[�V�������I��
   if (m_isMovingBack && easedProgress >= 1.0f)
   {
      m_isTextTextreVisible = true;
   }
   if (!m_isMovingBack && easedProgress >= 1.0f)
   {
      m_isTargetVisible = false;
      m_isMovingBack = true;
      m_delyTime = 2.0f;
      m_drawTargetTimer = DRAW_TARGET_TIME;
   }
}

/// <summary>
/// �A�j���[�V�����ҋ@���Ԃ��X�V����
/// </summary>
/// <returns></returns>
bool MissionMessageUI::UpdatePauseTime(const float& elapsedTime)
{
   if (m_pauseTime > 0.0f)
   {
      m_pauseTime -= elapsedTime; // �t���[�����Ɏ��Ԃ�i�߂�
      return true; // �ҋ@��
   }
   return false; // �ҋ@�I��
}


/// <summary>
/// �C�[�W���O�i�s�x���X�V����
/// </summary>
/// <returns></returns>
float MissionMessageUI::UpdateTransitionProgress(const float& elapsedTime)
{
   m_transitionElapsedTime += elapsedTime; // �t���[�����Ɍo�ߎ��Ԃ�i�߂�
   float progressRate = m_transitionElapsedTime / TRANSITION_DURATION;

   // �i�s�x��1.0�𒴂����ꍇ�A1.0�ɐ���
   if (progressRate >= 1.0f)
   {
      progressRate = 1.0f;
      m_transitionElapsedTime = 0.0f;
   }

   return progressRate;
}


/// <summary>
/// �A�j���[�V�����̏I���������s��
/// </summary>
void MissionMessageUI::FinishTransition()
{
   // UI����ɖ߂����ꍇ�A1.5�b�ҋ@
   if (!m_isMovingUp)
      m_pauseTime = STANBY_TIME;
   else
      m_isVisible = false;

   m_isMovingUp = !m_isMovingUp; // �㉺�̓�����؂�ւ�

}

/// <summary>
/// �~�b�V����UI�̐i���x�����Z�b�g����
/// </summary>
void MissionMessageUI::ResetMissionUIProgress()
{
   m_transitionElapsedTime = 0.0f;
}

/// <summary>
/// UI�̈ʒu���X�V����
/// </summary>
/// <param name="progressRate">�i�s�x</param>
void MissionMessageUI::UpdatePosition(float progressRate)
{
   if (m_isMovingUp)
   {
      // ��ɖ߂� (�ʒu���X�V)
      m_textTexturePosition = DirectX::SimpleMath::Vector2::Lerp(MISSION_END_POSITION, MISSION_START_POSITION, progressRate);
      m_textBackPosition = m_textTexturePosition - TEXT_BACKGROUND_OFFSET; // �w�i�ʒu�𒲐�
   }
   else
   {
      // �������Ɉړ�
      m_textTexturePosition = DirectX::SimpleMath::Vector2::Lerp(MISSION_START_POSITION, MISSION_END_POSITION, progressRate);
      m_textBackPosition = m_textTexturePosition - TEXT_BACKGROUND_OFFSET; // �w�i�ʒu�𒲐�
   }
}

/// <summary>
/// ���\�[�X��ǂݍ���
/// </summary>
void MissionMessageUI::LoadResources()
{
   //UI�e�N�X�`����ǂݍ���
   m_missionBack = yamadalib::Resources::GetInstance()->GetTexture("TextBack");
   m_textTexture = yamadalib::Resources::GetInstance()->GetTexture("Text");
   m_missionTextTexture = yamadalib::Resources::GetInstance()->GetTexture("MissionText");
}


/// <summary>
/// �~�b�V�������ʂ�UI��`�悷��
/// </summary>
void MissionMessageUI::DrawResultUI(bool isClear)
{
   //�i�s�x����x���Z�b�g����
   if (!m_isEndReset)
   {
      ResetMissionUIProgress();
      m_isEndReset = true;
   }
   m_drawResultTimer -= m_deltaTime;

   if (m_drawResultTimer <= 0.0f)
   {
      m_isGameFinishDraw = true;
   }

   //�e�N�X�`����`�悷��
   m_graphics->DrawTexture(m_missionBack, m_textBackPosition, nullptr);
   m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + DRAW_MISSION_OFFSET, &MISSION_RECT);
   if(isClear)    //�~�b�V��������
      m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + DRAW_MISSION_COMPLETE_OFFSET, &COMPLETE_RECT);
   else           //�~�b�V�������s
      m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + DRAW_MISSION_FAILED_OFFSET, &FAILED_RECT);

   if (m_isGameFinishDraw) return;
   //UI�𓮂���
   MoveMissionUI(m_deltaTime);
}

/// <summary>
/// �C�[�W���O��K�p����֐�
/// </summary>
/// <param name="progressRate">�i�s�x</param>
/// <returns></returns>
float MissionMessageUI::EaseOut(float progressRate)
{
   return 1 - powf(1 - progressRate, 3); // 3���֐���EaseOut
}
