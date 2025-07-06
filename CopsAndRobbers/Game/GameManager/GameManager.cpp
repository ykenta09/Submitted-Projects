/*
	@file	GameManager.cpp
	@brief	�Q�[���}�l�[�W���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/GameManager/GameManager.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/BlackBoard.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Observer/Messenger.h"

std::unique_ptr<GameManager> GameManager::m_gameManager = nullptr;

GameManager* const GameManager::GetInstance(CommonResources* resources, Map* map)
{
   if (!m_gameManager && resources && map)
   {
	  //�O���t�B�b�N�X�̃C���X�^���X�𐶐�����
	  m_gameManager.reset(new GameManager(resources, map));
   }
   return m_gameManager.get();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resources"></param>
GameManager::GameManager(CommonResources* resources, Map* map)
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_timer(0),
	m_commonResources(resources),
    m_map(map),
	m_delayTimer(DELAY_TIME),
    m_currentStageNumber(0),
    m_gameStart(true),
    m_gamePlay(false),
    m_gameClear(false),
    m_gameOver(false),
    m_isGoal(false)
{
   //Astar�O���t���쐬
   m_astarGraph = std::make_unique<AStarGraph>(m_map);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
GameManager::~GameManager()
{
   Finalize();
}

/// <summary>
/// ����������
/// </summary>
void GameManager::Initialize()
{
   Reset();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void GameManager::Update(const float& elapsedTime)
{
   //�L�[�{�[�h�̏�Ԃ��擾����
   m_keyboardState = DirectX::Keyboard::Get().GetState();
   //�L�[�{�[�h�X�e�[�g�g���b�J�[���X�V����
   m_keyboradStateTracker.Update(m_keyboardState);

	//�Q�[���J�n���̃f�B���C�^�C������
	m_delayTimer -= elapsedTime;

	if (!m_isGoal && m_delayTimer <= 0)
	{
	   //�Q�[�����Ԃ��X�V����
	   m_timer += elapsedTime;
	   m_commonResources->GetBlackBoard()->AddTime(m_timer);
	}
}


/// <summary>
/// �`�悷��
/// </summary>
void GameManager::Render()
{

}

/// <summary>
/// ��n������
/// </summary>
void GameManager::Finalize()
{
   Reset();
}

/// <summary>
/// �S�[���C�x���g
/// </summary>
void GameManager::GoalEvent()
{
}

/// <summary>
/// �L�[�{�[�h���͏���
/// </summary>
/// <param name="keys">���̓L�[</param>
void GameManager::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
   switch (keys)
   {
   case DirectX::Keyboard::Space:
	  m_gameStart = false;
	  break;
   }
}

/// <summary>
/// �C���X�^���X���폜����
/// </summary>
void GameManager::DestroyInstance()
{
   m_gameManager.reset();
}


/// <summary>
/// �Q�[���v���C�֐�
/// </summary>
void GameManager::GamePlay()
{
   m_gameStart = false;
   m_gamePlay = true;
}

/// <summary>
/// �Q�[���N���A�֐�
/// </summary>
void GameManager::GameClear()
{
   m_gamePlay = false;
   m_gameClear = true;
}

/// <summary>
/// �Q�[���I�[�o�[�֐�
/// </summary>
void GameManager::GameOver()
{
   m_gamePlay = false;
   m_gameOver = true;
}

void GameManager::Reset()
{
   m_gameStart = true;
   m_gamePlay = false;
   m_gameClear = false;
   m_gameOver = false;
   m_isGoal = false;
} 
