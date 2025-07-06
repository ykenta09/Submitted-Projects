/*
	@file	TimerUI.cpp
	@brief	�^�C�}�[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/TimerUI/TimerUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Stamina.h"
#include "Game/GameManager/GameManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resoureces">���ʃ��\�[�X</param>
/// <param name="player">�v���C���[</param>
/// <param name="fellow">����</param>
/// <param name="map">�}�b�v</param>
TimerUI::TimerUI()
	:
    m_graphics(yamadalib::Graphics::GetInstance()),
    m_timer(0.0f),
    m_timerPosition(TIMER_POSITION)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
TimerUI::~TimerUI()
{
}


/// <summary>
/// ����������
/// </summary>
void TimerUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime"></param>
void TimerUI::Update(const float& playTime)
{
   m_timer = playTime;
}


/// <summary>
/// �`�悷��
/// </summary>
void TimerUI::Render()
{
   //����UI
   DrawTimerUI();
}


/// <summary>
/// ��n������
/// </summary>
void TimerUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// �^�C�}�[UI��`�悷��
/// </summary>
void TimerUI::DrawTimerUI()
{
   // �X�v���C�g�o�b�`�̊J�n
   m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_graphics->GetCommonStates()->NonPremultiplied());

   // �����b�ɕϊ�
   int totalSeconds = static_cast<int>(m_timer);

   // ���ƕb���v�Z
   int minutes = totalSeconds / 60;
   int seconds = totalSeconds % 60;

   // ���݂̕`��ʒu��������
   DirectX::SimpleMath::Vector2 drawPosition = m_timerPosition;

   // ������`�悷�邽�߂̈ʒu��ݒ�
   auto drawDigit = [&](int digit)
   {
	  // RECT�̐ݒ�
	  RECT rect;
	  rect.top = 0;
	  rect.bottom = DIGIT_HEIGHT;  // �����̍����ɍ��킹��
	  rect.left = static_cast<LONG>(DIGIT_WIDTH * digit);  // �����̍��[���v�Z
	  rect.right = rect.left + DIGIT_WIDTH;  // �����̉E�[��ݒ�

	  // �e�N�X�`���̕`��
	  m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &rect);

	  // ���̐����̕`��ʒu���E�Ɉړ�
	  drawPosition.x += DIGIT_WIDTH + SPACING;  // �����̕��ɍ��킹�Ĉړ����A�Ԋu�𒲐�
   };

   // ���̏\�̈ʂƈ�̈ʂ�`��
   drawPosition.x += DIGIT_WIDTH;  // ���Ɉړ�����00:00�`���ɍ��킹��i�����̕��ɍ��킹��j
   drawDigit(minutes / 10);
   drawDigit(minutes % 10);

   // ���ƕb�̊ԂɃR������`��
   drawPosition.x += COLON_WIDTH - SPACING;  // �R�����̃X�y�[�X�𒲐�
   RECT colonRect = { DIGIT_WIDTH * 10, 0, DIGIT_WIDTH * 10 + COLON_WIDTH, COLON_HEIGHT };  // �R�����̈ʒu��ݒ�
   m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &colonRect);
   drawPosition.x += COLON_HEIGHT / 2.0f;  // �R�����̃X�y�[�X���ړ�

   // �b�̏\�̈ʂƈ�̈ʂ�`��
   drawDigit(seconds / 10);
   drawDigit(seconds % 10);

   // �X�v���C�g�o�b�`�̏I���
   m_graphics->GetSpriteBatch()->End();
}


/// <summary>
/// ���\�[�X���擾����
/// </summary>
void TimerUI::LoadResources()
{
   m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("Timer");
}
