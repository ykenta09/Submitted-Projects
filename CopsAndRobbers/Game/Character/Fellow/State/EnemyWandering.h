/*
	@file	EnemyWandering.h
	@brief	敵の徘徊状態クラス
*/
#ifndef ENEMY_WANDERING_DEFINED
#define ENEMY_WANDERING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// 前方宣言
class CommonResources;
class GraphScene;
class Enemy;


class EnemyWandering : public IState
{
public:
	void SetGraphDate(GraphScene* graph, int number);
	//敵の処理座標を取得する
	const DirectX::SimpleMath::Vector3& GetInitialPosition() { return m_patrolRouteMap[m_initalNumber]; }
	//目標地点を取得する
	DirectX::SimpleMath::Vector3& GetWanderTarget() { return m_wanderTarget; }
	//移動先の頂点番号を取得する
	int GetRootNumber() { return m_currentRootNumber;  }
	//現在の頂点番号を取得する
	int GetCurrentNumber();
public:
	//コンストラクタ
	EnemyWandering(Enemy* enemy);
	//デストラクタ
	~EnemyWandering();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
	//受け取った目標地点に移動する
	void  Move(const DirectX::SimpleMath::Vector3& target, const float& elapsedTime);
	//巡回経路の更新
	void UpDaatePatrolRoute();
	//巡回座標に到着したか
	bool IsArriveDestination();
	//巡回ルートを初期化する
	void InitialPatrolRoute();
	//移動割合を計算する
	void MovementRatio();
private:
	//敵
	Enemy* m_enemy;
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフデータ
	GraphScene* m_graphScene;
	//グラフィックス　デバック用
	yamadalib::Graphics* m_graphics;

	//ターゲットの座標
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//敵の巡回ルートマップ
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRouteMap;
	//敵の個別のルートマップ
	std::unordered_map<int, Graph::PatrolRoute> m_patrolAdjacencyList;
	//敵が通ったルートマップ
	std::vector<int> m_passedRoutNumber;
	//移動目標の経路番号
	int m_currentRootNumber;
	//移動元の経路番号
	int m_previousRootNumber;
	//検索した経路を見つけることができたか
	bool m_noFoundRoute;
	//敵番号
	int m_enemyNumber;
	//敵の初期座標の頂点番号
	int m_initalNumber;
	// 巡回の目標地点
	DirectX::SimpleMath::Vector3 m_wanderTarget;
	//移動の進行度
	float m_moveProgress;
};
#endif // ENEMY_WANDERING_DEFINED
