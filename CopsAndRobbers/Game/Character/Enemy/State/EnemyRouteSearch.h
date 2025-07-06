/*
	@file	EnemyRouteSearch.h
	@brief	敵の経路探索状態クラス
*/
#ifndef ENEMY_ROUTE_SEARCH_DEFINED
#define ENEMY_ROUTE_SEARCH_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Astar/LineOfSite.h"
#include "Game/Map/Map.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// 前方宣言
class CommonResources;
class Enemy;
class GraphScene;

class EnemyRouteSearch : public IState
{
public:
   static constexpr float TIME_TO_SEARCH = { 0.5f };		//経路探索までの時間
public:
   //グラフを取得する
   AStarGraph* GetGrid() const { return m_graph; }
   //ターゲットのセルを取得する
   CellVertex* GetTargetCell() const { return m_targetCell; }
   //最初の探索かどうか取得する
   bool GetFirstExploration() const { return m_isFirstExploration; }
   //最初の探索フラグを設定する
   void SetFirstExploration(bool flag) { m_isFirstExploration = flag; }
   int GetTargetStageNumber() const { return m_tagetStageNumber; }
   //ターゲットのステージ番号を設定する
   void SetTargetStageNumber(const int& stageNumber) { m_tagetStageNumber = stageNumber; }
   //経路探索時間をセットする
   void SetSearchTime(const float& time) { m_searchTime = time; }

public:
	//コンストラクタ
    EnemyRouteSearch(Enemy* enemy, Map* map, const int& stageNumber);
	//デストラクタ
	~EnemyRouteSearch();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();
	//Astarの準備をする
	void SetAstar(DirectX::SimpleMath::Vector3 target);
private:
    CommonResources* m_commonResources;    // 共通リソース
	Enemy* m_enemy;						   //敵
    yamadalib::Graphics* m_graphics;	   //グラフィックス　デバック用
	Map* m_map;							   //マップ
	CellVertex* m_targetCell;			   //ターゲットのセル

	AStarGraph* m_graph;		//グラフ
	std::unique_ptr<LineOfSite> m_lineOfSite;	//ラインオブサイト

	DirectX::SimpleMath::Vector3 m_targetPosition;	//ターゲットの座標

	//ターゲットンステージ番号
	int m_tagetStageNumber;

	bool m_isFirstExploration;	//始めての探索か
	float m_searchTime;		    //探索時間
	int m_stageNumber;		    //ステージ番号

};
#endif // ENEMY_ROUTE_SEARCH_DEFINED
