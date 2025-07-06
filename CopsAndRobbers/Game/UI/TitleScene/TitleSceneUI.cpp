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
#include "Game/UI/TitleScene/TitleSceneUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Stamina.h"



/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resoureces">共通リソース</param>
TitleSceneUI::TitleSceneUI(CommonResources* resoureces)
   :
   m_commonResources(resoureces),
   m_graphics(yamadalib::Graphics::GetInstance()),
   m_timer(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
TitleSceneUI::~TitleSceneUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void TitleSceneUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime">プレイ時間</param>
void TitleSceneUI::Update(const float& elapsedTime, const float& playTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
   //ゲーム開始時間に調整(のちに修正)
   m_timer = playTime;
}


/// <summary>
/// 描画する
/// </summary>
void TitleSceneUI::Render()
{
}


/// <summary>
/// 後始末する
/// </summary>
void TitleSceneUI::Finalize()
{
	// do nothing.
}


/// <summary>
/// リソースを取得する
/// </summary>
void TitleSceneUI::LoadResources()
{
}
