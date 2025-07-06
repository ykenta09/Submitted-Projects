/*
	@file	KeyCountUI.cpp
	@brief	���̐��\��UI�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/KeyCountUI/KeyCountUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Stamina.h"
#include "Game/GameManager/GameManager.h"
#include "Libraries/MyLib/BlackBoard.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resoureces">���ʃ��\�[�X</param>
/// <param name="player">�v���C���[</param>
/// <param name="fellow">����</param>
/// <param name="map">�}�b�v</param>
KeyCountUI::KeyCountUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance())
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
KeyCountUI::~KeyCountUI()
{
}


/// <summary>
/// ����������
/// </summary>
void KeyCountUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
void KeyCountUI::Update()
{
   //�������Ă��錮�̐����擾
   m_keyNum = GameManager::GetInstance()->GetCommonResources()->GetBlackBoard()->GetHasKeyNum();
}


/// <summary>
/// �`�悷��
/// </summary>
void KeyCountUI::Render()
{
   //�����Ă��錮�̐���`�悷��
   DrawKeyCount();
}


/// <summary>
/// ��n������
/// </summary>
void KeyCountUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// ���\�[�X���擾����
/// </summary>
void KeyCountUI::LoadResources()
{
   m_miniMapTexture = yamadalib::Resources::GetInstance()->GetTexture("KeyIcon");
   m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("Timer");
}

/// <summary>
/// �����Ă��錮�̐���`�悷��
/// </summary>
void KeyCountUI::DrawKeyCount()
{
   //���A�C�R����`��
   m_graphics->DrawTexture(m_miniMapTexture, KEY_ICON_POSITION, nullptr, DirectX::Colors::White);

   // �����̕`��ʒu
   DirectX::SimpleMath::Vector2 drawPosition = KEY_NUM_POSITION;

   // 2���܂őΉ��i��F�ő�99�j
   int keyNum = m_keyNum;
   int digits[2] = { 0, 0 };
   digits[0] = keyNum / 10; // �\�̈�
   digits[1] = keyNum % 10; // ��̈�

   for (int i = 0; i < 2; ++i)
   {
      // 1���ڂ�0�Ȃ�1���\���ɂ���ꍇ�͂�����continue���Ă�OK
      if (i == 0 && digits[0] == 0) {
         // 1���ڂ�0�̏ꍇ�͕`�悵�Ȃ��i1���\���j
         continue;
      }

      RECT rect;
      rect.top = 0;
      rect.bottom = DIGIT_HEIGHT;
      rect.left = DIGIT_WIDTH * digits[i];
      rect.right = rect.left + DIGIT_WIDTH;

      m_graphics->DrawTexture(m_timerTexture, drawPosition, &rect, DirectX::Colors::White);

      drawPosition.x += DIGIT_WIDTH + SPACING;
   }
}
