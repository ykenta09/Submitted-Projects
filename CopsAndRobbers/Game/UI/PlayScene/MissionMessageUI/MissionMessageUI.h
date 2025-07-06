/*
	@file	MissionMessageUI.h
	@brief	ミッションメッセージUIクラス
*/
#pragma once
#ifndef MISSION_MESSAGE_UI_DEFINED
#define MISSION_MESSAGE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;

class MissionMessageUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 MISSION_START_POSITION = { -50.0f, -400.0f };  // 初期位置 (画面外から上に設定)
   static constexpr DirectX::SimpleMath::Vector2 MISSION_END_POSITION = { -50.0f, 250.0f };     // 最終位置 (既存の描画位置)
   static constexpr DirectX::SimpleMath::Vector2 TARGET_START_POSITION = { -1700.0f, 360.0f };	 //開始位置
   static constexpr DirectX::SimpleMath::Vector2 TARGET_END_POSITION = { -80.0f, 360.0f };		 //終了位置

   static constexpr DirectX::SimpleMath::Vector2 MISSION_START_BLACK_POSITION = { 0.0f, -360.0f };   //黒いテクスチャの座標
   static constexpr DirectX::SimpleMath::Vector2 MISSION_START_TEXT_POSITION = { 150.0f, 100.0f };  // 初期位置 (画面外から上に設定)
   static constexpr DirectX::SimpleMath::Vector2 TARGET_TEXT_POSITION = { 220.0f, 370.0f };		 //ミッションテキストの座標
   static constexpr DirectX::SimpleMath::Vector2 TARGET_BLACK_POSITION = { -2000.0f, 360.0f };		 //ミッションUIの背景テクスチャの座標

   static constexpr DirectX::SimpleMath::Vector2 TEXT_SPACE_POSITION = { 600.0f, 0.0f };
   static constexpr DirectX::SimpleMath::Vector2 TEXT_BACKGROUND_OFFSET = { -50.0f, -50.0f };
   static constexpr DirectX::SimpleMath::Vector2 DRAW_MISSION_OFFSET = { 100.0f, 100.0f };
   static constexpr DirectX::SimpleMath::Vector2 DRAW_MISSION_COMPLETE_OFFSET = { 600.0f, 100.0f };
   static constexpr DirectX::SimpleMath::Vector2 DRAW_MISSION_FAILED_OFFSET = { 700.0f, 100.0f };
   static constexpr float TRANSITION_ELAPED_TIME = 0.0f;		  //経過時間
   static constexpr float TRANSITION_DURATION = 1.0f;			  //アニメーション全体の時間
   static constexpr float STANBY_TIME = 1.5f;					  //待機時間
   static constexpr float MISSION_MOVE_START = 0.4f;				 //ミッションの動きの開始時間
   static constexpr float MISSION_MOVE_END = 0.6f;					 //ミッションの動きの終了時間
   static constexpr float DRAW_TARGET_TIME = 1.0f;
   static constexpr float DRAW_RESULT_TIME = 2.0f;

   static constexpr RECT MISSION_RECT = { 0, 260 , 830, 400 };
   static constexpr RECT START_RECT = { 0, 380 , 810, 500 };
   static constexpr RECT COMPLETE_RECT = { 0, 510, 830, 650 };		 //成功のテクスチャ切り取り
   static constexpr RECT FAILED_RECT = { 0, 630, 830, 750 };		 //失敗のテクスチャ切り取り

public:
   //隠れているか堂か
   bool IsVisible() const { return m_isVisible; }
   //ゲーム終了UIの描画が終了したか
   bool GameFinishDraw() const { return m_isGameFinishDraw; }
public:
    //コンストラクタ
    MissionMessageUI();
	//デストラクタ
	~MissionMessageUI();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();


	//イージングアウト
	float EaseOut(float t);
	//アニメーションの終了処理を行う
	void FinishTransition();
	//
	void ResetMissionUIProgress();
private:
   //リソースを読み込む
   void LoadResources();
   //ミッション結果のUI
   void DrawResultUI(bool isClear);
   //ミッションUIの動き
   void MoveMissionUI(const float& elapsedTime);
   //ミッション目標を描画する
   void MoveMissionTargetUI(const float& elapsedTime);
   //ミッションスタートを描画する
   void DrawMissionStart();
   //ミッション目標を描画する
   void DrawMissionTarget();
   //アニメーション待機時間を更新する
   bool UpdatePauseTime(const float& elapsedTime);
   //UIの位置を更新する
   void UpdatePosition(float t);
   //イージング進行度を更新する
   float UpdateTransitionProgress(const float& elapsedTime);

private:
	//グラフィックス
	yamadalib::Graphics* m_graphics;
	//ミッション背景の画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionBack;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missionTextTexture;
	//テクスチャ座標
	DirectX::SimpleMath::Vector2 m_textTexturePosition;
	DirectX::SimpleMath::Vector2 m_textBackPosition;

	// === ミッション開始 ===
	bool m_isVisible;			 //UI描画終了
	bool m_isMovingUp;			 // true: 上に戻る, false: 下から上に移動
	float m_transitionElapsedTime; // 経過時間
	float m_pauseTime;			 //アニメーション待機時間
	bool m_isStartReset;

	// === ミッション終了 ===
	float m_deltaTime;		 //経過時間
	float m_drawResultTimer;	 //ミッション成功を描画する時間
	bool m_isGameFinishDraw;	 //ゲーム終了メッセージ描画の終わり
	bool m_isEndReset;

	// === ミッション目標 ===
	DirectX::SimpleMath::Vector2 m_targetTextBackPosition;
	float m_transitionDuration; // アニメーション全体の時間（1.5秒）
	float m_delyTime;				   //ディレイタイム
	float m_drawTargetTimer;
	bool m_isMovingBack;
	bool m_isTextTextreVisible;		   //UI描画終了
	bool m_isTargetVisible;
};
#endif // MISSION_MESSAGE_UI_DEFINED
