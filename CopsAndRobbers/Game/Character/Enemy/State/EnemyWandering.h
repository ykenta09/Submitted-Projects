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
#include "Libraries/yamadaLib/GameParameter.h"

// 前方宣言
class CommonResources;
class GraphScene;
class Enemy;

class EnemyWandering : public IState
{
public:
    //グラフのデータを受け取る
    void SetGraphDate(std::vector<DirectX::SimpleMath::Vector3>& patrolMap, std::unordered_map<int, GameParameters::PatrolRoute> adjacenyList, int initialNumber);
	//敵の処理座標を取得する
	const DirectX::SimpleMath::Vector3& GetInitialPosition() { return m_patrolRouteMap[m_initalNumber]; }
	//巡回マップを取得する
	const std::vector<DirectX::SimpleMath::Vector3> GetPatrolMap() { return m_patrolRouteMap; }
	//目標地点を取得する
	DirectX::SimpleMath::Vector3& GetWanderTarget() { return m_wanderTarget; }
	//移動先の頂点番号を取得する
	int GetRootNumber() { return m_currentRootNumber;  }
public:
	//コンストラクタ
	EnemyWandering(Enemy* enemy);
	//デストラクタ
	~EnemyWandering();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後始末する
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
	Enemy* m_enemy;							 //敵
	CommonResources* m_commonResources;		 // 共通リソース
	GraphScene* m_graphScene;				 //グラフデータ
	yamadalib::Graphics* m_graphics;		 //グラフィックス　デバック用

	// === 巡回経路管理 ===
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRouteMap;						//巡回ルートマップ
	std::unordered_map<int, GameParameters::PatrolRoute> m_patrolAdjacencyList;		//個別のルートマップ
	std::vector<int> m_passedRoutNumber;											//通ったルートマッ
	int m_currentRootNumber;	 //移動目標の経路番号
	int m_previousRootNumber;	 //移動元の経路番号
	int m_initalNumber;			 //敵の初期座標の頂点番号
	bool m_noFoundRoute;		 //検索した経路を見つけることができたか

	// === 移動状態 ===
	DirectX::SimpleMath::Vector3 m_wanderTarget;	// 巡回の目標地点
	float m_moveProgress;						    //移動の進行度

	int m_enemyNumber;			 //敵番号
};
#endif // ENEMY_WANDERING_DEFINED
