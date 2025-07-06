/*
	@file	StaminaUI.cpp
	@brief	�X�^�~�iUI�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/StaminaUI/StaminaUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Player.h"
#include "Game/GameManager/GameManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resoureces">���ʃ��\�[�X</param>
/// <param name="player">�v���C���[</param>
/// <param name="fellow">����</param>
/// <param name="map">�}�b�v</param>
StaminaUI::StaminaUI(Player* player)
	:
    m_graphics(yamadalib::Graphics::GetInstance()),
    m_player(player)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
StaminaUI::~StaminaUI()
{
}


/// <summary>
/// ����������
/// </summary>
void StaminaUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime"></param>
void StaminaUI::Update(const float& elapsedTime)
{
}


/// <summary>
/// �`�悷��
/// </summary>
void StaminaUI::Render()
{
   //�X�^�~�iUI
   DrawStaminaUI();
}


/// <summary>
/// ��n������
/// </summary>
void StaminaUI::Finalize()
{
	// do nothing.
}


/// <summary>
/// �X�^�~�i��UI��`�悷��
/// </summary>
void StaminaUI::DrawStaminaUI()
{
	//�X�^�~�i��`��p�ɕϊ�
	int staminaLength = static_cast<int>(m_player->GetStamina()->GetStaminaGuage());
	int staminaMax = static_cast<int>(m_player->GetStamina()->GetMaxStaminaGuage());

	RECT guageRect{ 0, 60 , 363 * staminaLength / staminaMax, 104};

	//�X�^�~�i�Q�[�W�̕`��
	m_graphics->DrawTexture(m_staminaGauge, STAMINA_POSITION, &STAMINA_RECT);
	//�X�^�~�i�t���[���̕`��
	m_graphics->DrawTexture(m_staminaGauge, STAMINA_POSITION + DirectX::SimpleMath::Vector2(0.0f, 12.0f), &guageRect);
}

/// <summary>
/// ���\�[�X���擾����
/// </summary>
void StaminaUI::LoadResources()
{
   m_staminaGauge = yamadalib::Resources::GetInstance()->GetTexture("Stamina");
}
