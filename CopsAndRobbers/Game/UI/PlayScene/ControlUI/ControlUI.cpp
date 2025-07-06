/*
	@file	ControlUI.cpp
	@brief	������@��UI�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/ControlUI/ControlUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/GameManager/GameManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ControlUI::ControlUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_controlsPosition(CONTROLS_POSITION)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
ControlUI::~ControlUI()
{
}


/// <summary>
/// ����������
/// </summary>
void ControlUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime"></param>
void ControlUI::Update()
{
}


/// <summary>
/// �`�悷��
/// </summary>
void ControlUI::Render()
{
   //������@UI��`�悷��
   ControlsUI();
}


/// <summary>
/// ��n������
/// </summary>
void ControlUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// ������@��UI
/// </summary>
void ControlUI::ControlsUI()
{
   //�ړ��L�[�̕`��
   m_graphics->DrawTexture(m_controlsTextTexture, m_controlsPosition, &MOVE_RECT);
   //�_�b�V���L�[�̕`��
   m_graphics->DrawTexture(m_controlsTextTexture, m_controlsPosition + SPRINT_RENDER_SPACE, &SPRINT_RECT);
   //�}�E�X����̕`��
   m_graphics->DrawTexture(m_controlsTextTexture, m_controlsPosition + MOUSE_RENDER_SPACE, &MOUSE_RECT);
}


/// <summary>
/// ���\�[�X���擾����
/// </summary>
void ControlUI::LoadResources()
{
   m_backTexture = yamadalib::Resources::GetInstance()->GetTexture("TextBack");
   m_controlsTextTexture = yamadalib::Resources::GetInstance()->GetTexture("ControlsText");
}
