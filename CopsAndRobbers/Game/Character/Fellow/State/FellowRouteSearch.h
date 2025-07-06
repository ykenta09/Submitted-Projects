/*
	@file	FellowRouteSearch.h
	@brief	仲間の経路探索状態クラス
*/
#ifndef FELLOW_ROUTE_SEARCH_DEFINED
#define FELLOW_ROUTE_SEARCH_DEFINED
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
class Fellow;
class GraphScene;


class FellowRouteSearch : public IState
{
public:
   const int MAXGRIDSIZE = 60;
   static constexpr float TIME_TO_SEARCH = { 0.5f };		   //経路探索までの時間
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
    FellowRouteSearch(Fellow* fellow, Map* map, const int& stageNumber);
	//デストラクタ
	~FellowRouteSearch();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();

	void SetAstar(DirectX::SimpleMath::Vector3 target);
private:
	//仲間のポインタ
	Fellow* m_fellow;
	//グラフ
	AStarGraph* m_graph;
	//ラインオブサイト
	std::unique_ptr<LineOfSite> m_lineOfSite;
	//マップ
	Map* m_map;
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス　デバック用
	yamadalib::Graphics* m_graphics;
	//ターゲットの座標
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//グリッドのサイズ
	int m_gridSize;
	//ターゲットのセル
	CellVertex* m_targetCell;
	//始めての探索か
	bool m_isFirstExploration;
	//探索時間
	float m_searchTime;
	//ステージ番号
	int m_stageNumber;
	//ターゲットンステージ番号
	int m_tagetStageNumber;

	//探索が必要か
	bool m_needSearch;
};
#endif // FELLOW_ROUTE_SEARCH_DEFINED
