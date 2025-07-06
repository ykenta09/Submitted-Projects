/*
	@file	KeyCountUI.cpp
	@brief	鍵の数表示UIクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/KeyCountUI/KeyCountUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Player/Stamina.h"
#include "Game/GameManager/GameManager.h"
#include "Libraries/MyLib/BlackBoard.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resoureces">共通リソース</param>
/// <param name="player">プレイヤー</param>
/// <param name="fellow">仲間</param>
/// <param name="map">マップ</param>
KeyCountUI::KeyCountUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance())
{
}


/// <summary>
/// デストラクタ
/// </summary>
KeyCountUI::~KeyCountUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void KeyCountUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
void KeyCountUI::Update()
{
   //所持している鍵の数を取得
   m_keyNum = GameManager::GetInstance()->GetCommonResources()->GetBlackBoard()->GetHasKeyNum();
}


/// <summary>
/// 描画する
/// </summary>
void KeyCountUI::Render()
{
   //持っている鍵の数を描画する
   DrawKeyCount();
}


/// <summary>
/// 後始末する
/// </summary>
void KeyCountUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// リソースを取得する
/// </summary>
void KeyCountUI::LoadResources()
{
   m_miniMapTexture = yamadalib::Resources::GetInstance()->GetTexture("KeyIcon");
   m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("Timer");
}

/// <summary>
/// 持っている鍵の数を描画する
/// </summary>
void KeyCountUI::DrawKeyCount()
{
   //鍵アイコンを描画
   m_graphics->DrawTexture(m_miniMapTexture, KEY_ICON_POSITION, nullptr, DirectX::Colors::White);

   // 数字の描画位置
   DirectX::SimpleMath::Vector2 drawPosition = KEY_NUM_POSITION;

   // 2桁まで対応（例：最大99個）
   int keyNum = m_keyNum;
   int digits[2] = { 0, 0 };
   digits[0] = keyNum / 10; // 十の位
   digits[1] = keyNum % 10; // 一の位

   for (int i = 0; i < 2; ++i)
   {
      // 1桁目が0なら1桁表示にする場合はここでcontinueしてもOK
      if (i == 0 && digits[0] == 0) {
         // 1桁目が0の場合は描画しない（1桁表示）
         continue;
      }

      RECT rect;
      rect.top = 0;
      rect.bottom = DIGIT_HEIGHT;
      rect.left = DIGIT_WIDTH * digits[i];
      rect.right = rect.left + DIGIT_WIDTH;

      m_graphics->DrawTexture(m_timerTexture, drawPosition, &rect, DirectX::Colors::White);

      drawPosition.x += DIGIT_WIDTH + SPACING;
   }
}
