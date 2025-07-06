/*
	@file	UIManager.cpp
	@brief	UI�}�l�[�W���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/UI/UIManager.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Character/Player/Stamina.h"



/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resoureces">���ʃ��\�[�X</param>
UIManager::UIManager(CommonResources* resoureces)
   :
   m_commonResources(resoureces),
   m_graphics(yamadalib::Graphics::GetInstance())
{
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resoureces">���ʃ��\�[�X</param>
/// <param name="player">�v���C���[</param>
/// <param name="fellow">����</param>
/// <param name="map">�}�b�v</param>
UIManager::UIManager(CommonResources* resoureces, Player* player, Fellow* fellow, Map* map)
	:
    m_commonResources(resoureces),
    m_player(player),
    m_fellow(fellow),
    m_map(map),
	m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIManager::~UIManager()
{
   Finalize();
}


/// <summary>
/// ����������
/// </summary>
/// <param name="scene"></param>
void UIManager::Initialize(SceneType scene)
{
   m_titleScene = std::make_unique<TitleSceneUI>(m_commonResources);
   m_playScene = std::make_unique<PlaySceneUI>(m_commonResources, m_player, m_fellow, m_map);
   m_resultScene = std::make_unique<ResultSceneUI>(m_commonResources);
   
   //���݂̃X�e�[�W�ɐ؂�ւ���
   ChangeScene(scene);
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime">�v���C����</param>
void UIManager::Update(const float& elapsedTime, const float& playTime)
{
   //���݂̃V�[��UI���X�V����
   if (m_currentUIGroup) m_currentUIGroup->Update(elapsedTime, playTime);
}


/// <summary>
/// �`�悷��
/// </summary>
void UIManager::Render()
{
   //���݂̃V�[��UI��`�悷��
   if (m_currentUIGroup) m_currentUIGroup->Render();
}


/// <summary>
/// ��n������
/// </summary>
void UIManager::Finalize()
{
	// do nothing.
}


/// <summary>
/// �`�悷��UI��؂�ւ���
/// </summary>
/// <param name="scene">���݂̃V�[��</param>
void UIManager::ChangeScene(SceneType scene)
{
   switch (scene)
   {
   case UIManager::TITLE:
	  m_currentUIGroup = m_titleScene.get();
	  break;
   case UIManager::PLAY:
	  m_currentUIGroup = m_playScene.get();
	  break;
   case UIManager::RESULT:
	  m_currentUIGroup = m_resultScene.get();
	  break;
   }
   m_currentUIGroup->Initialize();
}

