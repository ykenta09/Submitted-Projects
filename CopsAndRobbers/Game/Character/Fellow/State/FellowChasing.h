/*
	@file	FellowChasing.h
	@brief	仲間の追跡状態クラス
*/
#ifndef FELLOW_CHASING_DEFINED
#define FELLOW_CHASING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;
class Fellow;

class FellowChasing : public IState
{
public:
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_targetPosition = target; }
	bool GeteGoalFlag() { return m_isGoal; }
	void SetGoalFlag(bool flag) { m_isGoal = flag; }
public:
	//コンストラクタ
	FellowChasing(Fellow* fellow);
	//デストラクタ
	~FellowChasing();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
	void Move(const float& elapsedTime);
	void TargetMove(const DirectX::SimpleMath::Vector3& target, const float& elapsedTime);
	//探索
	void Search();
	//受け取った目標地点に移動する
private:
	//仲間
	Fellow* m_fellow;
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス　デバック用
	yamadalib::Graphics* m_graphics;

	//ターゲットの座標
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//スケール
	DirectX::SimpleMath::Vector3 m_scale;
	//境界ボックス
	DirectX::BoundingBox m_boundingBox;
	//目的地に達したか
	bool m_isArrived;

	//ゴールしたかどうか
	bool m_isGoal;
};
#endif // FELLOW_CHASING_DEFINED
