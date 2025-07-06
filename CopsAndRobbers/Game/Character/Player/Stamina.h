/*
	@file	Stamina.h
	@brief	スタミナ管理クラス
*/
#pragma once
#ifndef STAMINA_DEFINED
#define STAMINA_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;


class Stamina
{
public:
	static constexpr float MAX_STAMINA_GUAGE = 150.0f;		//最大スタミナ量
	static constexpr float CONSUME_STAMINA = 15.0f;			//消費するスタミナ量
public:
	//スタミナゲージを取得する
	float GetStaminaGuage() { return m_staminaGuage; }
	//スタミナの最大値を取得する
	float GetMaxStaminaGuage() { return MAX_STAMINA_GUAGE; }
	//回避するスタミナがあるか
	bool IsAvoidance();
	//スタミナを消費する
	void ConsumeStamina();
public:
    //コンストラクタ
	Stamina();
	//デストラクタ
	~Stamina();
	//初期化する
	void Initialize();
	//更新する
	void Update(float elapsedTime);
	//スタミナ自然回復
	void NaturalStaminaRecovery(const float& elapsedTime);
private:
	yamadalib::Graphics* m_graphics;	//グラフィックス
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// ゲージの画像

	float m_staminaGuage;	//スタミナゲージ
	float m_frameCounter;	//フレームレートカウンター
};
#endif // STAMINA_DEFINED
