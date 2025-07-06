/*
	@file	PlayerIdle.cpp
	@brief	プレイヤーの待機状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerIdle.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">プレイヤー本体のポインタ</param>
PlayerIdle::PlayerIdle(Player* player)
	:
	m_player( player )
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerIdle::~PlayerIdle()
{
    //後始末をする
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void PlayerIdle::Initialize()
{
}

/// <summary>
///  更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerIdle::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

/// <summary>
/// 描画する
/// </summary>
void PlayerIdle::Render()
{
}

/// <summary>
/// 後始末する
/// </summary>
void PlayerIdle::Finalize()
{
}
