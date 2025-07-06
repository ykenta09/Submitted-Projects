/*
	@file	UIManager.cpp
	@brief	UIマネージャークラス
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
/// コンストラクタ
/// </summary>
/// <param name="resoureces">共通リソース</param>
UIManager::UIManager(CommonResources* resoureces)
   :
   m_commonResources(resoureces),
   m_graphics(yamadalib::Graphics::GetInstance())
{
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resoureces">共通リソース</param>
/// <param name="player">プレイヤー</param>
/// <param name="fellow">仲間</param>
/// <param name="map">マップ</param>
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
/// デストラクタ
/// </summary>
UIManager::~UIManager()
{
   Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
/// <param name="scene"></param>
void UIManager::Initialize(SceneType scene)
{
   m_titleScene = std::make_unique<TitleSceneUI>(m_commonResources);
   m_playScene = std::make_unique<PlaySceneUI>(m_commonResources, m_player, m_fellow, m_map);
   m_resultScene = std::make_unique<ResultSceneUI>(m_commonResources);
   
   //現在のステージに切り替える
   ChangeScene(scene);
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime">プレイ時間</param>
void UIManager::Update(const float& elapsedTime, const float& playTime)
{
   //現在のシーンUIを更新する
   if (m_currentUIGroup) m_currentUIGroup->Update(elapsedTime, playTime);
}


/// <summary>
/// 描画する
/// </summary>
void UIManager::Render()
{
   //現在のシーンUIを描画する
   if (m_currentUIGroup) m_currentUIGroup->Render();
}


/// <summary>
/// 後始末する
/// </summary>
void UIManager::Finalize()
{
	// do nothing.
}


/// <summary>
/// 描画するUIを切り替える
/// </summary>
/// <param name="scene">現在のシーン</param>
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

