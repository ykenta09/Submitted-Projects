/*
	@file	StaminaUI.h
	@brief	スタミナUIクラス
*/
#pragma once
#ifndef STAMINA_UI_DEFINED
#define STAMINA_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;
class Player;

class StaminaUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 STAMINA_POSITION = DirectX::SimpleMath::Vector2(10, 70);						 //スタミナのテクスチャ座標
   static constexpr RECT STAMINA_RECT = { 0, 0, 363, 60 };			 //スタミナのテクスチャ切り取り
public:
    //コンストラクタ
    StaminaUI(Player* player);
	//デストラクタ
	~StaminaUI();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();
private:
   //リソースを読み込む
   void LoadResources();
   //スタミナUI	
   void DrawStaminaUI();

private:
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス
	yamadalib::Graphics* m_graphics;
	//キャラクターのポインタ
	Player* m_player;

	//スタミナゲージの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_staminaGauge;
};
#endif // STAMINA_UI_DEFINED
