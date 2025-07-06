/*
	@file	Stamina.cpp
	@brief	スタミナ管理クラス 
*/
#include "pch.h"
#include "Stamina.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/yamadaLib/Resources.h"
#include <cassert>
#include <Game/Screen.h>


/// <summary>
/// コンストラクタ
/// </summary>
Stamina::Stamina()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_staminaGuage{ MAX_STAMINA_GUAGE },
	m_frameCounter{ 0.0f }
{
}

/// <summary>
/// デストラクタ
/// </summary>
Stamina::~Stamina()
{
	// do nothing.
}


/// <summary>
/// 初期化
/// </summary>
void Stamina::Initialize()
{
	//テクスチャ画像を設定する
	m_texture = yamadalib::Resources::GetInstance()->GetTexture("Stamina");
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Stamina::Update(float elapsedTime)
{
	//スタミナの自然回復
	NaturalStaminaRecovery(elapsedTime);
}

/// <summary>
/// スタミナの自然回復
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Stamina::NaturalStaminaRecovery(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// 1フレームあたりのスタミナ回復量
	const float RECOVERY_AMOUNT_PER_FRAME = MAX_STAMINA_GUAGE / 700.0f;

	// スタミナゲージが最大値を超えないようにする
	if (m_staminaGuage >= MAX_STAMINA_GUAGE) return;

	// 各フレームでスタミナを少しずつ回復
	m_staminaGuage += RECOVERY_AMOUNT_PER_FRAME;
}

/// <summary>
/// 回避するスタミナがあるか
/// </summary>
/// <returns></returns>
bool Stamina::IsAvoidance()
{
	if (m_staminaGuage >= CONSUME_STAMINA) return true;
	return false;
}

/// <summary>
/// スタミナを消費する
/// </summary>
void Stamina::ConsumeStamina()
{
	//スタミナを消費する
	m_staminaGuage -= CONSUME_STAMINA;
}


