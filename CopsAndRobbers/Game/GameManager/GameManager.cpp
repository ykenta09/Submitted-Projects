/*
	@file	GameManager.cpp
	@brief	ゲームマネージャークラス
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
	  //グラフィックスのインスタンスを生成する
	  m_gameManager.reset(new GameManager(resources, map));
   }
   return m_gameManager.get();
}

/// <summary>
/// コンストラクタ
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
   //Astarグラフを作成
   m_astarGraph = std::make_unique<AStarGraph>(m_map);
}


/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager()
{
   Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void GameManager::Initialize()
{
   Reset();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void GameManager::Update(const float& elapsedTime)
{
   //キーボードの状態を取得する
   m_keyboardState = DirectX::Keyboard::Get().GetState();
   //キーボードステートトラッカーを更新する
   m_keyboradStateTracker.Update(m_keyboardState);

	//ゲーム開始時のディレイタイム処理
	m_delayTimer -= elapsedTime;

	if (!m_isGoal && m_delayTimer <= 0)
	{
	   //ゲーム時間を更新する
	   m_timer += elapsedTime;
	   m_commonResources->GetBlackBoard()->AddTime(m_timer);
	}
}


/// <summary>
/// 描画する
/// </summary>
void GameManager::Render()
{

}

/// <summary>
/// 後始末する
/// </summary>
void GameManager::Finalize()
{
   Reset();
}

/// <summary>
/// ゴールイベント
/// </summary>
void GameManager::GoalEvent()
{
}

/// <summary>
/// キーボード入力処理
/// </summary>
/// <param name="keys">入力キー</param>
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
/// インスタンスを削除する
/// </summary>
void GameManager::DestroyInstance()
{
   m_gameManager.reset();
}


/// <summary>
/// ゲームプレイ関数
/// </summary>
void GameManager::GamePlay()
{
   m_gameStart = false;
   m_gamePlay = true;
}

/// <summary>
/// ゲームクリア関数
/// </summary>
void GameManager::GameClear()
{
   m_gamePlay = false;
   m_gameClear = true;
}

/// <summary>
/// ゲームオーバー関数
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
