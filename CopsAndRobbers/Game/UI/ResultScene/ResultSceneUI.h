/*
	@file	ResultSceneUI.h
	@brief	リザルトシーンUIクラス
*/
#pragma once
#ifndef RESULT_SCENE_UI_DEFINED
#define RESULT_SCENE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Game/Interface/IUIGroup.h"


// 前方宣言
class CommonResources;
class Stamina;

class ResultSceneUI : public IUIGroup
{
public:
public:
	//時間を取得する
	float GetTimer() { return m_timer; }

public:
    ResultSceneUI(CommonResources* resoureces);
	~ResultSceneUI();

	void Initialize() override;
	void Update(const float& elapsedTime, const float& playTime = 0) override;
	void Render() override;
	void Finalize();

	void LoadResources();

private:
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス
	yamadalib::Graphics* m_graphics;


	//時間
	float m_timer;

};
#endif // RESULT_SCENE_UI_DEFINED
