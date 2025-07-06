/*
	@file	Shadow.h
	@brief	影クラス
*/
#pragma once
#ifndef SHADOW_DEFINED
#define SHADOW_DEFINED

#include "Libraries/yamadaLib/Graphics.h"

//影クラス
class Shadow
{	
public:
	//コンストラクタ
	Shadow();
	//デストラクタ
	~Shadow();

	//初期化する
	void Initialize();
	//描画する
	void Render(
	const DirectX::SimpleMath::Vector3& position,
	float radius = 0.9f);
	//後処理をする
	void Finalize();
private:
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//グラフィックス
	yamadalib::Graphics* m_graphics;
};
#endif // SHADOW_DEFINED

