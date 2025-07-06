/*
	@file	EnemyChasing.h
	@brief	敵の追跡状態クラス
*/
#ifndef ENEMY_CHASING_DEFINED
#define ENEMY_CHASING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// 前方宣言
class CommonResources;
class Enemy;


class EnemyChasing : public IState
{
public:
   static const float LOST_TIME;

public:
   //ターゲットの座標を設定する
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_targetPosition = target; }
	//ターゲットの座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_targetPosition; }
public:
	//コンストラクタ
	EnemyChasing(Enemy* enemy);
	//デストラクタ
	~EnemyChasing();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
	//受け取った目標地点に移動する
	void  Move(const float& elapsedTime);
	//ターゲットを見失う
	void LostTarget(float elapsedTime);
private:
	//敵
	Enemy* m_enemy;
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス　デバック用
	yamadalib::Graphics* m_graphics;
	//ターゲットの座標
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//見失った時間
	float m_lostTime;
	//頂点に到達したか
	bool m_isArrived;
};
#endif // ENEMY_CHASING_DEFINED
