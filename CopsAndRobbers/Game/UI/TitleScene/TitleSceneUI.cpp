/*
	@file	PlaySceneUI.cpp
	@brief	�v���C�V�[����UI�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/TitleScene/TitleSceneUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Stamina.h"



/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resoureces">���ʃ��\�[�X</param>
TitleSceneUI::TitleSceneUI(CommonResources* resoureces)
   :
   m_commonResources(resoureces),
   m_graphics(yamadalib::Graphics::GetInstance()),
   m_timer(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleSceneUI::~TitleSceneUI()
{
}


/// <summary>
/// ����������
/// </summary>
void TitleSceneUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime">�v���C����</param>
void TitleSceneUI::Update(const float& elapsedTime, const float& playTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
   //�Q�[���J�n���Ԃɒ���(�̂��ɏC��)
   m_timer = playTime;
}


/// <summary>
/// �`�悷��
/// </summary>
void TitleSceneUI::Render()
{
}


/// <summary>
/// ��n������
/// </summary>
void TitleSceneUI::Finalize()
{
	// do nothing.
}


/// <summary>
/// ���\�[�X���擾����
/// </summary>
void TitleSceneUI::LoadResources()
{
}
