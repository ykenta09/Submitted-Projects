/*
	@file	Stamina.cpp
	@brief	�X�^�~�i�Ǘ��N���X 
*/
#include "pch.h"
#include "Stamina.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/yamadaLib/Resources.h"
#include <cassert>
#include <Game/Screen.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
Stamina::Stamina()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_staminaGuage{ MAX_STAMINA_GUAGE },
	m_frameCounter{ 0.0f }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Stamina::~Stamina()
{
	// do nothing.
}


/// <summary>
/// ������
/// </summary>
void Stamina::Initialize()
{
	//�e�N�X�`���摜��ݒ肷��
	m_texture = yamadalib::Resources::GetInstance()->GetTexture("Stamina");
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Stamina::Update(float elapsedTime)
{
	//�X�^�~�i�̎��R��
	NaturalStaminaRecovery(elapsedTime);
}

/// <summary>
/// �X�^�~�i�̎��R��
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Stamina::NaturalStaminaRecovery(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// 1�t���[��������̃X�^�~�i�񕜗�
	const float RECOVERY_AMOUNT_PER_FRAME = MAX_STAMINA_GUAGE / 700.0f;

	// �X�^�~�i�Q�[�W���ő�l�𒴂��Ȃ��悤�ɂ���
	if (m_staminaGuage >= MAX_STAMINA_GUAGE) return;

	// �e�t���[���ŃX�^�~�i����������
	m_staminaGuage += RECOVERY_AMOUNT_PER_FRAME;
}

/// <summary>
/// �������X�^�~�i�����邩
/// </summary>
/// <returns></returns>
bool Stamina::IsAvoidance()
{
	if (m_staminaGuage >= CONSUME_STAMINA) return true;
	return false;
}

/// <summary>
/// �X�^�~�i�������
/// </summary>
void Stamina::ConsumeStamina()
{
	//�X�^�~�i�������
	m_staminaGuage -= CONSUME_STAMINA;
}


