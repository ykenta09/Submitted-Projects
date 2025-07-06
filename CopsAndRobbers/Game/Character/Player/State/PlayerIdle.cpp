/*
	@file	PlayerIdle.cpp
	@brief	�v���C���[�̑ҋ@��ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerIdle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="player">�v���C���[�{�̂̃|�C���^</param>
PlayerIdle::PlayerIdle(Player* player)
	:
	m_player( player )
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerIdle::~PlayerIdle()
{
    //��n��������
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void PlayerIdle::Initialize()
{
}

/// <summary>
///  �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerIdle::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

/// <summary>
/// �`�悷��
/// </summary>
void PlayerIdle::Render()
{
}

/// <summary>
/// ��n������
/// </summary>
void PlayerIdle::Finalize()
{
}
