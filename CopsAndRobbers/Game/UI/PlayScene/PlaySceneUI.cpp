/*
	@file	PlaySceneUI.cpp
	@brief	プレイシーンのUIクラス
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
/// コンストラクタ
/// </summary>
/// <param name="resoureces">共通リソース</param>
/// <param name="player">プレイヤー</param>
/// <param name="fellow">仲間</param>
/// <param name="map">マップ</param>
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
/// デストラクタ
/// </summary>
PlaySceneUI::~PlaySceneUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void PlaySceneUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
	//ミニマップを生成、初期化
	m_miniMapUI = std::make_unique<MiniMapUI>(m_commonResources, m_player, m_fellow, m_map);
	m_miniMapUI->Initialize();
	//ミッションメッセージ
	m_missionMessageUI = std::make_unique<MissionMessageUI>();
	m_missionMessageUI->Initialize();
	//鍵の表示UIを生成、初期化
	m_keyCountUI = std::make_unique<KeyCountUI>();
	m_keyCountUI->Initialize();
	//スタミナUIを生成、初期化
	m_staminaUI = std::make_unique<StaminaUI>(m_player);
	m_staminaUI->Initialize();
	//タイマーUIを生成、初期化
	m_timerUI = std::make_unique<TimerUI>();
	m_timerUI->Initialize();
	//操作方法UIを生成、初期化
	m_controlUI = std::make_unique<ControlUI>();
	m_controlUI->Initialize();
	//ギミック操作UIを生成、初期化
	m_gimmickControlUI = std::make_unique<GimmickControlUI>();
	m_gimmickControlUI->Initialize();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime"></param>
void PlaySceneUI::Update(const float& elapsedTime, const float& playTime)
{
   //UIの更新
   m_missionMessageUI->Update(elapsedTime);
   m_miniMapUI->Update();
   m_keyCountUI->Update();
   m_staminaUI->Update(elapsedTime);
   m_timerUI->Update(playTime);
   m_controlUI->Update();
   m_gimmickControlUI->Update();
}


/// <summary>
/// 描画する
/// </summary>
void PlaySceneUI::Render()
{
   //ゲームプレイ中のみ描画する
   if (m_gameManager->GetGamePlay())
   {
	  m_graphics->DrawTexture(m_backTexture, BLACK_TEXTURE_TOP_POSITION, nullptr, DirectX::Colors::Black);
	  m_graphics->DrawTexture(m_backTexture, BLACK_TEXTURE_BOTTOM_POSITION, nullptr, DirectX::Colors::Black);

	  //ミニマップを描画する
	  m_miniMapUI->Render();
	  m_graphics->DrawTexture(m_hintTexture, HINT_POSITION, nullptr, DirectX::Colors::White);
	  //時間UI
	  m_timerUI->Render();
	  //スタミナUI
	  m_staminaUI->Render();
	  m_keyCountUI->Render();
	  //操作方法UIを描画する
	  m_controlUI->Render();
	  m_gimmickControlUI->Render();
   }
   //ミッションメッセージを描画する
   m_missionMessageUI->Render();
}


/// <summary>
/// 後始末する
/// </summary>
void PlaySceneUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// リソースを取得する
/// </summary>
void PlaySceneUI::LoadResources()
{
   m_backTexture = yamadalib::Resources::GetInstance()->GetTexture("TextBack");
   m_hintTexture = yamadalib::Resources::GetInstance()->GetTexture("Hint");
}
