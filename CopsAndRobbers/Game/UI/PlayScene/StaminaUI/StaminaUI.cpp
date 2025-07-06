/*
	@file	StaminaUI.cpp
	@brief	スタミナUIクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/StaminaUI/StaminaUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Player.h"
#include "Game/GameManager/GameManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resoureces">共通リソース</param>
/// <param name="player">プレイヤー</param>
/// <param name="fellow">仲間</param>
/// <param name="map">マップ</param>
StaminaUI::StaminaUI(Player* player)
	:
    m_graphics(yamadalib::Graphics::GetInstance()),
    m_player(player)
{
}


/// <summary>
/// デストラクタ
/// </summary>
StaminaUI::~StaminaUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void StaminaUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime"></param>
void StaminaUI::Update(const float& elapsedTime)
{
}


/// <summary>
/// 描画する
/// </summary>
void StaminaUI::Render()
{
   //スタミナUI
   DrawStaminaUI();
}


/// <summary>
/// 後始末する
/// </summary>
void StaminaUI::Finalize()
{
	// do nothing.
}


/// <summary>
/// スタミナのUIを描画する
/// </summary>
void StaminaUI::DrawStaminaUI()
{
	//スタミナを描画用に変換
	int staminaLength = static_cast<int>(m_player->GetStamina()->GetStaminaGuage());
	int staminaMax = static_cast<int>(m_player->GetStamina()->GetMaxStaminaGuage());

	RECT guageRect{ 0, 60 , 363 * staminaLength / staminaMax, 104};

	//スタミナゲージの描画
	m_graphics->DrawTexture(m_staminaGauge, STAMINA_POSITION, &STAMINA_RECT);
	//スタミナフレームの描画
	m_graphics->DrawTexture(m_staminaGauge, STAMINA_POSITION + DirectX::SimpleMath::Vector2(0.0f, 12.0f), &guageRect);
}

/// <summary>
/// リソースを取得する
/// </summary>
void StaminaUI::LoadResources()
{
   m_staminaGauge = yamadalib::Resources::GetInstance()->GetTexture("Stamina");
}
