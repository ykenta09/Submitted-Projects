/*
	@file	MissionMessageUI.cpp
	@brief  ミッションメッセージUIクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/UI/PlayScene/MissionMessageUI/MissionMessageUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/GameManager/GameManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
MissionMessageUI::MissionMessageUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_isMovingUp(false),
    m_isVisible(true),
    m_isGameFinishDraw(false),
    m_isMovingBack(false),
    m_isTextTextreVisible(false),
    m_isTargetVisible(true),
    m_isStartReset(false),
    m_isEndReset(false),
    m_pauseTime(0.0f),
    m_delyTime(1.5f),
    m_textTexturePosition(MISSION_START_POSITION),
    m_textBackPosition(MISSION_START_BLACK_POSITION),
    m_transitionElapsedTime(TRANSITION_ELAPED_TIME),
    m_targetTextBackPosition(TARGET_BLACK_POSITION),
    m_drawTargetTimer(DRAW_TARGET_TIME),
    m_drawResultTimer(DRAW_RESULT_TIME)
{
}

/// <summary>
/// デストラクタ
/// </summary>
MissionMessageUI::~MissionMessageUI()
{
   Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void MissionMessageUI::Initialize()
{
   //リソースを読み込む
   LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void MissionMessageUI::Update(const float& elapsedTime)
{
   //経過時間を格納
   m_deltaTime = elapsedTime;

   //ゲームスタート中のみミッション目標を表示
   if (GameManager::GetInstance()->GetGameStart())
   {
      MoveMissionTargetUI(elapsedTime);
   }
   //ゲームが始まったらミッションスタートを表示
   if (GameManager::GetInstance()->GetGamePlay())
   {
      if (!m_isStartReset)
      {
         //ミッションUIの進捗度をリセットする
         ResetMissionUIProgress();
         m_isStartReset = true;
      }
      // UIが表示されていない場合は処理を終了
      if (!m_isVisible) return;
      //ミッションUIの動き
      MoveMissionUI(elapsedTime);
   }
}

/// <summary>
/// 描画する
/// </summary>
void MissionMessageUI::Render()
{
   //ミッション目標を描画する
   DrawMissionTarget();
   //ミッションスタート
   DrawMissionStart();

   //ミッション成功
   if (GameManager::GetInstance()->GetGameClear())
   {
      DrawResultUI(true);
   }
   //ミッション失敗
   if (GameManager::GetInstance()->GetGameOver())
   {
      DrawResultUI(false);
   }
}

/// <summary>
/// 後始末する
/// </summary>
void MissionMessageUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// ミッションスタートを描画する
/// </summary>
void MissionMessageUI::DrawMissionStart()
{
   // UIが非表示の場合は描画しない
   if (!m_isVisible)
      return;

	//テクスチャを描画する
	m_graphics->DrawTexture(m_missionBack, m_textBackPosition, nullptr);
	m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + MISSION_START_TEXT_POSITION, &MISSION_RECT);
    m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + MISSION_START_TEXT_POSITION + TEXT_SPACE_POSITION, &START_RECT);
}

/// <summary>
/// ミッション目標を描画する
/// </summary>
void MissionMessageUI::DrawMissionTarget()
{
   //UIが正常に描画終了したまたはゲームが始まったら描画しない
   if (m_isTextTextreVisible) return;
   if (!GameManager::GetInstance()->GetGameStart()) return;
   //テクスチャを描画する
   m_graphics->DrawTexture(m_missionBack, m_targetTextBackPosition, nullptr);
   if (m_isTargetVisible) return;
   m_graphics->DrawTexture(m_missionTextTexture, TARGET_TEXT_POSITION, nullptr);
}


/// <summary>
/// ミッションUIの動き
/// </summary>
void MissionMessageUI::MoveMissionUI(const float& elapsedTime)
{
   // アニメーション待機時間があれば、カウントダウンして終了
   if (UpdatePauseTime(elapsedTime)) return;

   // イージング進行度を更新
   float progressRate = UpdateTransitionProgress(elapsedTime);
   float easedProgress = EaseOut(progressRate); // イージングを適用

   // UIの位置を更新
   UpdatePosition(easedProgress);

   // アニメーション終了時の処理
   // 進行度が1.0以上になった場合は、アニメーションを終了
   if (easedProgress >= 1.0f)
   {
      FinishTransition();
   }
}

/// <summary>
/// ミッション目標の移動処理
/// </summary>
void MissionMessageUI::MoveMissionTargetUI(const float& elapsedTime)
{
   //描画を隠しているなら処理しない
   if (m_isTextTextreVisible) return;

   //ディレイタイムを満たしていないなら処理しない
   m_delyTime -= elapsedTime;
   m_drawTargetTimer -= elapsedTime;
   if (m_delyTime >= 0.0f) return;

   // イージング進行度を更新
   float progressRate = UpdateTransitionProgress(elapsedTime);
   float easedProgress = EaseOut(progressRate); // イージングを適用

   if (m_isMovingBack)
   {
      m_targetTextBackPosition = DirectX::SimpleMath::Vector2::Lerp(TARGET_END_POSITION, TARGET_START_POSITION, easedProgress);
   }
   else
   {
      m_targetTextBackPosition = DirectX::SimpleMath::Vector2::Lerp(TARGET_START_POSITION, TARGET_END_POSITION, easedProgress);
   }

   if (m_drawTargetTimer <= 0)
   {
      m_isTargetVisible = true;
   }
   // 進行度が1.0以上になった場合は、アニメーションを終了
   if (m_isMovingBack && easedProgress >= 1.0f)
   {
      m_isTextTextreVisible = true;
   }
   if (!m_isMovingBack && easedProgress >= 1.0f)
   {
      m_isTargetVisible = false;
      m_isMovingBack = true;
      m_delyTime = 2.0f;
      m_drawTargetTimer = DRAW_TARGET_TIME;
   }
}

/// <summary>
/// アニメーション待機時間を更新する
/// </summary>
/// <returns></returns>
bool MissionMessageUI::UpdatePauseTime(const float& elapsedTime)
{
   if (m_pauseTime > 0.0f)
   {
      m_pauseTime -= elapsedTime; // フレーム毎に時間を進める
      return true; // 待機中
   }
   return false; // 待機終了
}


/// <summary>
/// イージング進行度を更新する
/// </summary>
/// <returns></returns>
float MissionMessageUI::UpdateTransitionProgress(const float& elapsedTime)
{
   m_transitionElapsedTime += elapsedTime; // フレーム毎に経過時間を進める
   float progressRate = m_transitionElapsedTime / TRANSITION_DURATION;

   // 進行度が1.0を超えた場合、1.0に制限
   if (progressRate >= 1.0f)
   {
      progressRate = 1.0f;
      m_transitionElapsedTime = 0.0f;
   }

   return progressRate;
}


/// <summary>
/// アニメーションの終了処理を行う
/// </summary>
void MissionMessageUI::FinishTransition()
{
   // UIが上に戻った場合、1.5秒待機
   if (!m_isMovingUp)
      m_pauseTime = STANBY_TIME;
   else
      m_isVisible = false;

   m_isMovingUp = !m_isMovingUp; // 上下の動きを切り替え

}

/// <summary>
/// ミッションUIの進捗度をリセットする
/// </summary>
void MissionMessageUI::ResetMissionUIProgress()
{
   m_transitionElapsedTime = 0.0f;
}

/// <summary>
/// UIの位置を更新する
/// </summary>
/// <param name="progressRate">進行度</param>
void MissionMessageUI::UpdatePosition(float progressRate)
{
   if (m_isMovingUp)
   {
      // 上に戻る (位置を更新)
      m_textTexturePosition = DirectX::SimpleMath::Vector2::Lerp(MISSION_END_POSITION, MISSION_START_POSITION, progressRate);
      m_textBackPosition = m_textTexturePosition - TEXT_BACKGROUND_OFFSET; // 背景位置を調整
   }
   else
   {
      // 下から上に移動
      m_textTexturePosition = DirectX::SimpleMath::Vector2::Lerp(MISSION_START_POSITION, MISSION_END_POSITION, progressRate);
      m_textBackPosition = m_textTexturePosition - TEXT_BACKGROUND_OFFSET; // 背景位置を調整
   }
}

/// <summary>
/// リソースを読み込む
/// </summary>
void MissionMessageUI::LoadResources()
{
   //UIテクスチャを読み込む
   m_missionBack = yamadalib::Resources::GetInstance()->GetTexture("TextBack");
   m_textTexture = yamadalib::Resources::GetInstance()->GetTexture("Text");
   m_missionTextTexture = yamadalib::Resources::GetInstance()->GetTexture("MissionText");
}


/// <summary>
/// ミッション結果のUIを描画する
/// </summary>
void MissionMessageUI::DrawResultUI(bool isClear)
{
   //進行度を一度リセットする
   if (!m_isEndReset)
   {
      ResetMissionUIProgress();
      m_isEndReset = true;
   }
   m_drawResultTimer -= m_deltaTime;

   if (m_drawResultTimer <= 0.0f)
   {
      m_isGameFinishDraw = true;
   }

   //テクスチャを描画する
   m_graphics->DrawTexture(m_missionBack, m_textBackPosition, nullptr);
   m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + DRAW_MISSION_OFFSET, &MISSION_RECT);
   if(isClear)    //ミッション成功
      m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + DRAW_MISSION_COMPLETE_OFFSET, &COMPLETE_RECT);
   else           //ミッション失敗
      m_graphics->DrawTexture(m_textTexture, m_textTexturePosition + DRAW_MISSION_FAILED_OFFSET, &FAILED_RECT);

   if (m_isGameFinishDraw) return;
   //UIを動かす
   MoveMissionUI(m_deltaTime);
}

/// <summary>
/// イージングを適用する関数
/// </summary>
/// <param name="progressRate">進行度</param>
/// <returns></returns>
float MissionMessageUI::EaseOut(float progressRate)
{
   return 1 - powf(1 - progressRate, 3); // 3次関数のEaseOut
}
