/*
	@file	TimerUI.h
	@brief	タイマーUIクラス
*/
#pragma once
#ifndef TIMER_UI_DEFINED
#define TIMER_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;
class Player;
class Fellow;
class Map;
class GameManager;

class TimerUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 TIMER_POSITION = DirectX::SimpleMath::Vector2(440, 0);							 //タイマーのテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 HINT_POSITION = DirectX::SimpleMath::Vector2(0.0f, 150.0f);					 //ヒントテクスチャの座標
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_TOP_POSITION = DirectX::SimpleMath::Vector2(0.0f, -150.0f);		 //上の背景テクスチャの座標
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_BOTTOM_POSITION = DirectX::SimpleMath::Vector2(0.0f, 650.0f);	 //下の背景テクスチャの座標
   static constexpr RECT MOVE_RECT = { 0, 0 , 634, 80 };			 //操作のテクスチャ切り取り
   static constexpr int DIGIT_WIDTH = 57;		   //数字の幅
   static constexpr int DIGIT_HEIGHT = 98;		   //数字の高さ
   static constexpr int COLON_WIDTH = 24;		   //コロンの幅
   static constexpr int COLON_HEIGHT = 96;		   //コロンの高さ
   static constexpr int SPACING = 5;			   //間隔調整
public:
    //コンストラクタ
    TimerUI();
	//デストラクタ
	~TimerUI();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& playTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();

private:
   //タイマーUI
   void DrawTimerUI();
   //リソースを読み込む
   void LoadResources();
private:
	//グラフィックス
	yamadalib::Graphics* m_graphics;

	//タイマーの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;

	//時間
	float m_timer;
	//時間の描画座標
	DirectX::SimpleMath::Vector2 m_timerPosition;
};
#endif // TIMER_UI_DEFINED
