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
#include "Game/UI/PlayScene/ControlUI/GimmickControlUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Map/Map.h"
#include "Game/Map/Base/DeviceObjectBase.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
GimmickControlUI::GimmickControlUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_controlsPosition(DirectX::SimpleMath::Vector3::Zero),
    m_isVisible(false)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
GimmickControlUI::~GimmickControlUI()
{
}


/// <summary>
/// ����������
/// </summary>
void GimmickControlUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime"></param>
void GimmickControlUI::Update()
{
   m_isVisible = false;
   //�f�o�C�X�I�u�W�F�N�g�z����擾
   auto devices =  GameManager::GetInstance()->GetMap()->GetDeviceObjects();
   for (const auto& device : devices)
   {
	  //����\�ȑ��u������΍��W���擾
	  if (device->GetInsideControlZone())
	  {
		 m_controlsPosition = device->GetPosition();
		 m_isVisible = true;
	  }
   }
}


/// <summary>
/// �`�悷��
/// </summary>
void GimmickControlUI::Render()
{
   if (!m_isVisible) return;
   //������@UI��`�悷��
   ControlsUI();
}


/// <summary>
/// ��n������
/// </summary>
void GimmickControlUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// ������@��UI
/// </summary>
void GimmickControlUI::ControlsUI()
{
   //�G�̍s��𐶐�
   DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_controlsPosition + CONTROLS_OFFSET);
   //�ړ��L�[�̕`��
   m_graphics->DrawTextureBillboard(m_controlTexture.Get(), world, false);
}


/// <summary>
/// ���\�[�X���擾����
/// </summary>
void GimmickControlUI::LoadResources()
{
   m_controlTexture = yamadalib::Resources::GetInstance()->GetTexture("GimmickControl");
}
