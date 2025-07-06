/*
	@file	ResultScene.cpp
	@brief	リザルトシーンクラス
*/
#include "pch.h"
#include "ResultScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/BlackBoard.h"
#include "Libraries/yamadaLib/Resources.h"
#include <cassert>

/// <summary>
/// コンストラクタ
/// </summary>
ResultScene::ResultScene()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_commonResources{},
	m_spriteFont{},
    m_resultTexture{},
    m_alpha(1.0f),
	m_isChangeScene{},
    m_clearTimeTextPosition(CLEAR_TIME_TEXT_POSITION)
{
}

/// <summary>
/// デストラクタ
/// </summary>
ResultScene::~ResultScene()
{
	// do nothing.
}

/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
void ResultScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	//クリアタイムを取得する
	m_clearTime = m_commonResources->GetBlackBoard()->GetTime();

	//リソースを読み込む
	LoadResources();

	//円形フェードを作成
	m_circularFade = std::make_unique<CircularFade>();
	m_circularFade->Create(m_commonResources->GetDeviceResources());
	m_circularFade->SetTime(1.0f);
	m_circularFade->SetFadeOut(true);

	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	// ステージ番号をリセットする
	m_commonResources->GetBlackBoard()->SetStageNumber(0);
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void ResultScene::Update(float elapsedTime)
{
	// 宣言をしたが、実際は使用していない変数
	UNREFERENCED_PARAMETER(elapsedTime);

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// スペースキーが押されたら
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
	FadeTexture(elapsedTime);
}


/// <summary>
/// 描画する
/// </summary>
void ResultScene::Render()
{
    //コモンステートを取得する
	auto states = m_commonResources->GetCommonStates();
	//スプライトバッチで画像を描画
	m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	m_graphics->GetSpriteBatch()->Draw(m_backTexture.Get(), BACK_TEXTURE_POSITION);
	m_graphics->GetSpriteBatch()->Draw(m_resultBoardTexture.Get(), RESULT_BOARD_POSITION);
	m_graphics->GetSpriteBatch()->Draw(m_resultTexture.Get(), TEXT_POSITION);
	m_graphics->GetSpriteBatch()->Draw(m_pushKeyTextTexture.Get(), PUSHKEY_TEXT_POSITION, nullptr, DirectX::Colors::White * m_alpha);
	m_graphics->GetSpriteBatch()->End();
	//クリアタイムを描画する
	DrawClearTime();

	if (!m_circularFade->GetFade())
	{
	   m_circularFade->Render();
	}
}

/// <summary>
/// 後始末する
/// </summary>
void ResultScene::Finalize()
{
}

/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns></returns>
IScene::SceneID ResultScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}


/// <summary>
/// リソースを読み込む
/// </summary>
void ResultScene::LoadResources()
{
	m_resultTexture = yamadalib::Resources::GetInstance()->GetTexture("ResultText");
	m_backTexture = yamadalib::Resources::GetInstance()->GetTexture("ResultBack");
	m_resultBoardTexture = yamadalib::Resources::GetInstance()->GetTexture("ResultBoard");
	m_pushKeyTextTexture = yamadalib::Resources::GetInstance()->GetTexture("PushKey");
	m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("BackTimer");
	m_clearTimeTextTexture = yamadalib::Resources::GetInstance()->GetTexture("ClearTimer");
}

/// <summary>
/// クリアタイム
/// </summary>
void ResultScene::DrawClearTime()
{
   // スプライトバッチの開始
	m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_graphics->GetCommonStates()->NonPremultiplied());

	// 整数秒に変換
	int totalSeconds = static_cast<int>(m_clearTime);

	// 分と秒を計算
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	// 現在の描画位置を初期化
	DirectX::SimpleMath::Vector2 drawPosition = TIMER_POSITION;

	// 数字を描画するための位置を設定
	auto drawDigit = [&](int digit) {
		// RECTの設定
		RECT rect;
		rect.top = 0;
		rect.bottom = 128;  // 高さを設定
		rect.left = static_cast<LONG>(DRAW_TIME_OFSET * digit);  // 現在の数字の左端
		rect.right = rect.left + DRAW_TIME_OFSET;  // 現在の数字の右端

		// テクスチャの描画
		m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &rect);

		// 次の数字の描画位置を右に移動
		drawPosition.x += DRAW_TIME_OFSET;  // 各数字の描画幅に合わせて移動
	};

	// 分の十の位と一の位を描画
	drawPosition.x += DRAW_TIME_OFSET;  // 左に移動して00:00形式に合わせる
	drawDigit(minutes / 10);
	drawDigit(minutes % 10);

	// 分と秒の間にコロンを描画
	drawPosition.x += DRAW_TIME_OFSET;  // コロンのスペース分移動
	m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &COLON_RECT);
	drawPosition.x += DRAW_TIME_OFSET;  // コロンのスペース分移動

	// 秒の十の位と一の位を描画
	drawDigit(seconds / 10);
	drawDigit(seconds % 10);

	//クリアタイムテキストを描画する
	m_graphics->GetSpriteBatch()->Draw(m_clearTimeTextTexture.Get(), m_clearTimeTextPosition);

	// スプライトバッチの終わり
	m_graphics->GetSpriteBatch()->End();
}



/// <summary>
/// フェード
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void ResultScene::FadeTexture(float elapsedTime)
{
   if (m_fadeIn) //フェードイン
   {
	  m_alpha += FADE_SPEED * elapsedTime;
	  if (m_alpha >= 1.0f)
	  {
		 m_alpha = 1.0f;
		 m_fadeIn = false;
	  }
   }
   else   //フェードアウト
   {
	  m_alpha -= FADE_SPEED * elapsedTime;
	  if (m_alpha <= 0.3f)
	  {
		 m_alpha = 0.3f;
		 m_fadeIn = true;
	  }
   }
}
