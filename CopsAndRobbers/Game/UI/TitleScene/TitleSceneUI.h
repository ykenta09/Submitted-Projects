/*
	@file	TitleSceneUI.h
	@brief	タイトルシーンUIクラス
*/
#pragma once
#ifndef TITLE_SCENE_UI_DEFINED
#define TITLE_SCENE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Game/Interface/IUIGroup.h"


// 前方宣言
class CommonResources;
class Stamina;

class TitleSceneUI : public IUIGroup
{
public:
public:
	//時間を取得する
	float GetTimer() { return m_timer; }

public:
    //コンストラクタ
    TitleSceneUI(CommonResources* resoureces);
	//デストラクタ
	~TitleSceneUI();

	//初期化する
	void Initialize() override;
	//更新する
	void Update(const float& elapsedTime, const float& playTime = 0) override;
	//描画する
	void Render() override;
	//後始末する
	void Finalize();
	//リソースを読み込む
	void LoadResources();

private:
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス
	yamadalib::Graphics* m_graphics;

	//時間
	float m_timer;
	//時間の描画座標
	DirectX::SimpleMath::Vector2 m_timerPosition;
	//スタミナゲージ描画座標
	DirectX::SimpleMath::Vector2 m_staminaPosition;
	//操作方法の描画座標
	DirectX::SimpleMath::Vector2 m_controlsPosition;
};
#endif // TITLE_SCENE_UI_DEFINED
