/*
	@file	ControlUI.h
	@brief	コントロールUIクラス
*/
#pragma once
#ifndef CONTROL_UI_DEFINED
#define CONTROL_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;

class ControlUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 CONTROLS_POSITION = DirectX::SimpleMath::Vector2(50, 650);						 //操作方法のテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 SPRINT_RENDER_SPACE = DirectX::SimpleMath::Vector2(350.0f, 0.0f);				 //ダッシュの描画間隔
   static constexpr DirectX::SimpleMath::Vector2 MOUSE_RENDER_SPACE = DirectX::SimpleMath::Vector2(800.0f, 0.0f);				 //マウスの描画間隔
   static constexpr RECT MOVE_RECT = { 0, 0 , 634, 80 };			 //操作のテクスチャ切り取り
   static constexpr RECT SPRINT_RECT = { 0, 65 , 624, 200 };		 //ダッシュテクスチャ切り取り
   static constexpr RECT MOUSE_RECT = { 0, 125 , 624, 200 };		 //マウス操作のテクスチャ切り取り
public:
    //コンストラクタ
    ControlUI();
	//デストラクタ
	~ControlUI();

	//初期化する
	void Initialize();
	//更新する
	void Update();
	//描画する
	void Render();
	//後始末する
	void Finalize();

private:
	//操作方法のUI
	void ControlsUI();
	//リソースを読み込む
	void LoadResources();

private:
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス
	yamadalib::Graphics* m_graphics;

	//UI背景画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	//操作方法画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_controlsTextTexture;

	//操作方法の描画座標
	DirectX::SimpleMath::Vector2 m_controlsPosition;
};
#endif // CONTROL_UI_DEFINED
