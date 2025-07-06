/*
	@file	ControlUI.cpp
	@brief	操作方法のUIクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/ControlUI/ControlUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/GameManager/GameManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
ControlUI::ControlUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_controlsPosition(CONTROLS_POSITION)
{
}


/// <summary>
/// デストラクタ
/// </summary>
ControlUI::~ControlUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void ControlUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime"></param>
void ControlUI::Update()
{
}


/// <summary>
/// 描画する
/// </summary>
void ControlUI::Render()
{
   //操作方法UIを描画する
   ControlsUI();
}


/// <summary>
/// 後始末する
/// </summary>
void ControlUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// 操作方法のUI
/// </summary>
void ControlUI::ControlsUI()
{
   //移動キーの描画
   m_graphics->DrawTexture(m_controlsTextTexture, m_controlsPosition, &MOVE_RECT);
   //ダッシュキーの描画
   m_graphics->DrawTexture(m_controlsTextTexture, m_controlsPosition + SPRINT_RENDER_SPACE, &SPRINT_RECT);
   //マウス操作の描画
   m_graphics->DrawTexture(m_controlsTextTexture, m_controlsPosition + MOUSE_RENDER_SPACE, &MOUSE_RECT);
}


/// <summary>
/// リソースを取得する
/// </summary>
void ControlUI::LoadResources()
{
   m_backTexture = yamadalib::Resources::GetInstance()->GetTexture("TextBack");
   m_controlsTextTexture = yamadalib::Resources::GetInstance()->GetTexture("ControlsText");
}
