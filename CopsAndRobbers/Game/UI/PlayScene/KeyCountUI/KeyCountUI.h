/*
	@file	KeyCountUI.h
	@brief	鍵の数表示UIクラス
*/
#pragma once
#ifndef KEY_COUNT_UI_DEFINED
#define KEY_COUNT_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;
class GameManager;

class KeyCountUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 KEY_ICON_POSITION = { 35.0f, 230.0f };	  //鍵のアイコン描画座標
   static constexpr int DIGIT_WIDTH = 57;
   static constexpr int DIGIT_HEIGHT = 98;
   static constexpr int SPACING = 5;
   static constexpr DirectX::SimpleMath::Vector2 KEY_NUM_POSITION = { 120.0f, 230.0f }; // アイコンの右横など

public:
    //コンストラクタ
    KeyCountUI();
	//デストラクタ
	~KeyCountUI();

	//初期化する
	void Initialize();
	//更新する
	void Update();
	//描画する
	void Render();
	//後始末する
	void Finalize();

private:
   //リソースを読み込む
   void LoadResources();
   //鍵の数を描画する
   void DrawKeyCount();

private:
	//グラフィックス
	yamadalib::Graphics* m_graphics;

	//スタミナゲージの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_miniMapTexture;
	//タイマーの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;

	//所持している鍵の数
	int m_keyNum;
};
#endif // KEY_COUNT_UI_DEFINED
