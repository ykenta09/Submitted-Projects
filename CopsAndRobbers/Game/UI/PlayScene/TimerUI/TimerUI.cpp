/*
	@file	TimerUI.cpp
	@brief	タイマークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/TimerUI/TimerUI.h"
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
TimerUI::TimerUI()
	:
    m_graphics(yamadalib::Graphics::GetInstance()),
    m_timer(0.0f),
    m_timerPosition(TIMER_POSITION)
{
}


/// <summary>
/// デストラクタ
/// </summary>
TimerUI::~TimerUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void TimerUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime"></param>
void TimerUI::Update(const float& playTime)
{
   m_timer = playTime;
}


/// <summary>
/// 描画する
/// </summary>
void TimerUI::Render()
{
   //時間UI
   DrawTimerUI();
}


/// <summary>
/// 後始末する
/// </summary>
void TimerUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// タイマーUIを描画する
/// </summary>
void TimerUI::DrawTimerUI()
{
   // スプライトバッチの開始
   m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_graphics->GetCommonStates()->NonPremultiplied());

   // 整数秒に変換
   int totalSeconds = static_cast<int>(m_timer);

   // 分と秒を計算
   int minutes = totalSeconds / 60;
   int seconds = totalSeconds % 60;

   // 現在の描画位置を初期化
   DirectX::SimpleMath::Vector2 drawPosition = m_timerPosition;

   // 数字を描画するための位置を設定
   auto drawDigit = [&](int digit)
   {
	  // RECTの設定
	  RECT rect;
	  rect.top = 0;
	  rect.bottom = DIGIT_HEIGHT;  // 数字の高さに合わせる
	  rect.left = static_cast<LONG>(DIGIT_WIDTH * digit);  // 数字の左端を計算
	  rect.right = rect.left + DIGIT_WIDTH;  // 数字の右端を設定

	  // テクスチャの描画
	  m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &rect);

	  // 次の数字の描画位置を右に移動
	  drawPosition.x += DIGIT_WIDTH + SPACING;  // 数字の幅に合わせて移動し、間隔を調整
   };

   // 分の十の位と一の位を描画
   drawPosition.x += DIGIT_WIDTH;  // 左に移動して00:00形式に合わせる（数字の幅に合わせる）
   drawDigit(minutes / 10);
   drawDigit(minutes % 10);

   // 分と秒の間にコロンを描画
   drawPosition.x += COLON_WIDTH - SPACING;  // コロンのスペースを調整
   RECT colonRect = { DIGIT_WIDTH * 10, 0, DIGIT_WIDTH * 10 + COLON_WIDTH, COLON_HEIGHT };  // コロンの位置を設定
   m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &colonRect);
   drawPosition.x += COLON_HEIGHT / 2.0f;  // コロンのスペース分移動

   // 秒の十の位と一の位を描画
   drawDigit(seconds / 10);
   drawDigit(seconds % 10);

   // スプライトバッチの終わり
   m_graphics->GetSpriteBatch()->End();
}


/// <summary>
/// リソースを取得する
/// </summary>
void TimerUI::LoadResources()
{
   m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("Timer");
}
