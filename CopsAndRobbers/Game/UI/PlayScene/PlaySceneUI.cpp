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
#include "Game/UI/PlayScene/PlaySceneUI.h"
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
PlaySceneUI::PlaySceneUI(CommonResources* resoureces, Player* player, Fellow* fellow, Map* map)
	:
    m_commonResources(resoureces),
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_gameManager(GameManager::GetInstance()),
    m_player(player),
    m_fellow(fellow),
    m_map(map)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
PlaySceneUI::~PlaySceneUI()
{
}


/// <summary>
/// ����������
/// </summary>
void PlaySceneUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    LoadResources();
	//�~�j�}�b�v�𐶐��A������
	m_miniMapUI = std::make_unique<MiniMapUI>(m_commonResources, m_player, m_fellow, m_map);
	m_miniMapUI->Initialize();
	//�~�b�V�������b�Z�[�W
	m_missionMessageUI = std::make_unique<MissionMessageUI>();
	m_missionMessageUI->Initialize();
	//���̕\��UI�𐶐��A������
	m_keyCountUI = std::make_unique<KeyCountUI>();
	m_keyCountUI->Initialize();
	//�X�^�~�iUI�𐶐��A������
	m_staminaUI = std::make_unique<StaminaUI>(m_player);
	m_staminaUI->Initialize();
	//�^�C�}�[UI�𐶐��A������
	m_timerUI = std::make_unique<TimerUI>();
	m_timerUI->Initialize();
	//������@UI�𐶐��A������
	m_controlUI = std::make_unique<ControlUI>();
	m_controlUI->Initialize();
	//�M�~�b�N����UI�𐶐��A������
	m_gimmickControlUI = std::make_unique<GimmickControlUI>();
	m_gimmickControlUI->Initialize();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="playTime"></param>
void PlaySceneUI::Update(const float& elapsedTime, const float& playTime)
{
   //UI�̍X�V
   m_missionMessageUI->Update(elapsedTime);
   m_miniMapUI->Update();
   m_keyCountUI->Update();
   m_staminaUI->Update(elapsedTime);
   m_timerUI->Update(playTime);
   m_controlUI->Update();
   m_gimmickControlUI->Update();
}


/// <summary>
/// �`�悷��
/// </summary>
void PlaySceneUI::Render()
{
   //�Q�[���v���C���̂ݕ`�悷��
   if (m_gameManager->GetGamePlay())
   {
	  m_graphics->DrawTexture(m_backTexture, BLACK_TEXTURE_TOP_POSITION, nullptr, DirectX::Colors::Black);
	  m_graphics->DrawTexture(m_backTexture, BLACK_TEXTURE_BOTTOM_POSITION, nullptr, DirectX::Colors::Black);

	  //�~�j�}�b�v��`�悷��
	  m_miniMapUI->Render();
	  m_graphics->DrawTexture(m_hintTexture, HINT_POSITION, nullptr, DirectX::Colors::White);
	  //����UI
	  m_timerUI->Render();
	  //�X�^�~�iUI
	  m_staminaUI->Render();
	  m_keyCountUI->Render();
	  //������@UI��`�悷��
	  m_controlUI->Render();
	  m_gimmickControlUI->Render();
   }
   //�~�b�V�������b�Z�[�W��`�悷��
   m_missionMessageUI->Render();
}


/// <summary>
/// ��n������
/// </summary>
void PlaySceneUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// ���\�[�X���擾����
/// </summary>
void PlaySceneUI::LoadResources()
{
   m_backTexture = yamadalib::Resources::GetInstance()->GetTexture("TextBack");
   m_hintTexture = yamadalib::Resources::GetInstance()->GetTexture("Hint");
}
