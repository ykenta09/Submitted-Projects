/*
	@file	GimmickControlUI.h
	@brief	コントロールUIクラス
*/
#pragma once
#ifndef GIMMICK_CONTROL_UI_DEFINED
#define GIMMICK_CONTROL_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;

class GimmickControlUI
{
public:
   static constexpr DirectX::SimpleMath::Vector3 CONTROLS_OFFSET = DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);						 //操作方法のテクスチャ座標
public:
    //コンストラクタ
    GimmickControlUI();
	//デストラクタ
	~GimmickControlUI();

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
	//操作方法画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_controlTexture;

	//操作方法の描画座標
	DirectX::SimpleMath::Vector3 m_controlsPosition;

	//UI表示フラグ
	bool m_isVisible;
};
#endif // GIMMICK_CONTROL_UI_DEFINED
