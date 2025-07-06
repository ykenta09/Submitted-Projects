/*
	@file	AStar.h
	@brief	AStarクラス
*/
#pragma once
#ifndef ASTAR_DEFINED
#define ASTAR_DEFINED
#include <unordered_set>
#include "Game/Character/Enemy/Comparator.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Map/Map.h"

// 前方宣言
class CommonResources;

//経路探索
class AStar
{
public:
   //グラフを取得する
   AStarGraph* GetGraph() const { return m_graph; }
   //グリッドを設定する
   void SetGraph(AStarGraph* graph) { m_graph = graph; }

   //最短経路を取得する
   const std::vector<int> GetShortestPath();
   //頂点リストを取得する
   std::vector<CellVertex> GetVertexList() const { return m_vertexList; }
   //頂点がオープンリストに存在しているかどうか調べる
   const bool GetVertexInOpenList(const CellVertex& currentVertex);
   //頂点がクローズリストに存在しているかどうか
   bool GetVertexInClosedList(const CellVertex& currentVartex);
public:
   //コンストラクタ
	AStar(AStarGraph* graph, const int& stageNumber);
	//デストラクタ
   ~AStar();
   //初期化する
	void Initialize();
	//クリアする
	void Clear();
	//探索する
	bool Search(int startVertexIndex, int goalVertexIndex);
	//複数ゴール対応探索
	bool SearchToAnyGoal(int startVertexIndex, const std::vector<int>& goalVertexIndices);
	//実際に到達したゴールノード
	int GetReachedGoalNode() const { return m_goalVertex.GetVertexNumber(); }
private:
   //ユークリッド距離を計算する
   float EuclideanDistance(const CellVertex& vertex, const CellVertex& vertex1);
   //マンハッタン距離を計算する
   float ManhattanDistance(const CellVertex& vertex, const CellVertex& vertex1);
   //スタート頂点の初期化
   void InitStartVertex(CellVertex& startVertex, int parentIndex, float cost, float heuristic);
   //最も近いゴールへのヒューリスティック
   float CalcMinHeuristic(const CellVertex& from, const std::unordered_set<int>& goalSet);
private:
    //グラフ
    AStarGraph* m_graph;

	//頂点リスト
	std::vector<CellVertex> m_vertexList;
	//隣接リスト
	std::vector<std::vector<int>> m_adjacencyList;

	//スタート頂点
	CellVertex m_startVertex;
	//ゴール頂点
	CellVertex m_goalVertex;

	//オープンリスト
	std::set<CellVertex, Comparator> m_openList;
	//クローズリスト
	std::unordered_set<int> m_closedList;

	//ステージ番号
	int m_stageNumber;
};
#endif // ASTAR_DEFINED
