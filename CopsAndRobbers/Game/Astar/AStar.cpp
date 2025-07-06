/*
	@file	AStar.cpp
	@brief	AStarクラス
*/
#include "pch.h"
#include "Game/Astar/AStar.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/yamadaLib/Resources.h"
#include <cassert>
#include <Game/Screen.h>
#include <Libraries/MyLib/Geometry.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="graph"></param>
AStar::AStar(AStarGraph* graph, const int& stageNumber)
	:
   m_graph(graph),
   m_stageNumber(stageNumber),
   m_openList{},
   m_closedList{},
   m_startVertex(DirectX::SimpleMath::Vector3::Zero),
   m_goalVertex(DirectX::SimpleMath::Vector3::Zero)
{
}

/// <summary>
/// デストラクタ
/// </summary>
AStar::~AStar()
{
	// do nothing.
}

/// <summary>
/// 初期化する
/// </summary>
void AStar::Initialize()
{
   //クリアする
   Clear();
   if (!m_graph) return;

   //頂点リストと隣接リストのセットは初回のみ
   if (m_vertexList.empty())
   {
	  m_vertexList = m_graph->GetVertexList(m_stageNumber);
   }
   if (m_adjacencyList.empty())
   {
	  m_adjacencyList = m_graph->GetAdjaceneyList(m_stageNumber);
   }
}


/// <summary>
///  探索する
/// </summary>
/// <param name="startVertexIndex">探索開始頂点</param>
/// <param name="goalVertexIndex">探索目標頂点</param>
/// <returns></returns>
bool AStar::Search(int startVertexIndex, int goalVertexIndex)
{
   // 頂点リストと隣接リストは初期化時にセット済み（再利用）
   Clear();

   //最新の頂点リストと隣接リストを取得する
   m_vertexList = m_graph->GetVertexList(m_stageNumber);
   m_adjacencyList = m_graph->GetAdjaceneyList(m_stageNumber);

   //スタート頂点を取得する
   CellVertex startVertex = m_vertexList[startVertexIndex];
   //ゴール頂点を取得する
   CellVertex goalVertex = m_vertexList[goalVertexIndex];

   InitStartVertex(startVertex, -1, 0.0f, EuclideanDistance(startVertex, goalVertex));
   m_openList.insert(startVertex);
   m_closedList.insert(startVertex.GetVertexNumber());

   //オープンリストが空になるまで探索を繰り返す
   while (!m_openList.empty())
   {
	  //オープンリストの先頭頂点を取得する
	  CellVertex parentVertex = *m_openList.begin();
	  //オープンリスtの先頭頂点を削除する
	  m_openList.erase(m_openList.begin());
	  //親頂点の頂点番号を親頂点インデックスに設定する
	  int parentVertexIndex = parentVertex.GetVertexNumber();
	  //ゴールに到達した場合、探索を終了する
	  if (parentVertexIndex == goalVertexIndex)
	  {
		 //スタート頂点の初期値を設定する
		 m_startVertex.SetVertexNumber(startVertexIndex);
		 //ゴール頂点の初期値を設定する
		 m_goalVertex.SetVertexNumber(goalVertexIndex);
		 return true;
	  }
	  //隣接ノードを探索する(対応した番号の隣接ノードを探索する)
	  for (int neighborIndex : m_adjacencyList[parentVertexIndex])
	  {
		 //隣接頂点を取得する
 		 CellVertex& neighborVertex = m_vertexList[neighborIndex];
		 //隣接頂点がクローズドリストに存在している場合はスキップ
		 if (GetVertexInClosedList(neighborVertex)) continue;
		 //コストを計算する
		 float cost = parentVertex.GetCost() + EuclideanDistance(parentVertex, neighborVertex);
		 //オープンリストに隣接頂点が存在しない　|| コストが隣接頂点のコストより小さい場合
		 if (!GetVertexInOpenList(neighborVertex) || cost < neighborVertex.GetCost())
		 {
			InitStartVertex(neighborVertex, parentVertexIndex, cost, EuclideanDistance(neighborVertex, goalVertex));
			//オープンリストに隣接リストに隣接頂点が存在していない場合、頂点を追加する
			if (!GetVertexInOpenList(neighborVertex))
			{
			   //オープンリストに追加する
			   m_openList.insert(neighborVertex);
			}
		 }
	  }
	  //クローズドリストに親頂点番号を追加する
	  m_closedList.insert(parentVertex.GetVertexNumber());
   }
   //探索に失敗した
   return false;
}


bool AStar::SearchToAnyGoal(int startVertexIndex, const std::vector<int>& goalVertexIndices)
{
   Clear();

   //最新の頂点リストと隣接リストを取得する
   m_vertexList = m_graph->GetVertexList(m_stageNumber);
   m_adjacencyList = m_graph->GetAdjaceneyList(m_stageNumber);

   std::unordered_set<int> goalSet(goalVertexIndices.begin(), goalVertexIndices.end());

   // スタート頂点を取得する
   CellVertex startVertex = m_vertexList[startVertexIndex];
   float heuristic = CalcMinHeuristic(startVertex, goalSet);
   InitStartVertex(startVertex, -1, 0.0f, heuristic);

   m_openList.insert(startVertex);
   m_closedList.insert(startVertex.GetVertexNumber());

   while (!m_openList.empty())
   {
	  CellVertex parentVertex = *m_openList.begin();
	  m_openList.erase(m_openList.begin());
	  int parentVertexIndex = parentVertex.GetVertexNumber();

	  // ゴール群に到達したら終了
	  if (goalSet.count(parentVertexIndex))
	  {
		 m_startVertex.SetVertexNumber(startVertexIndex);
		 m_goalVertex.SetVertexNumber(parentVertexIndex);
		 return true;
	  }

	  for (int neighborIndex : m_adjacencyList[parentVertexIndex])
	  {
		 CellVertex& neighborVertex = m_vertexList[neighborIndex];
		 if (GetVertexInClosedList(neighborVertex)) continue;
		 float cost = parentVertex.GetCost() + EuclideanDistance(parentVertex, neighborVertex);

		 // 最も近いゴールへのヒューリスティック
		 heuristic = CalcMinHeuristic(neighborVertex, goalSet);

		 if (!GetVertexInOpenList(neighborVertex) || cost < neighborVertex.GetCost())
		 {
			InitStartVertex(neighborVertex, parentVertexIndex, cost, heuristic);

			if (!GetVertexInOpenList(neighborVertex))
			{
			   m_openList.insert(neighborVertex);
			}
		 }
	  }
	  m_closedList.insert(parentVertex.GetVertexNumber());
   }
   return false;
}


/// <summary>
/// 探索状態をクリアする
/// </summary>
void AStar::Clear()
{
   //オープンリストをクリアする
   m_openList.clear();
   //クローズドリストをクリアする
   m_closedList.clear();
}

/// <summary>
/// 最短経路を取得する
/// </summary>
/// <returns></returns>
const std::vector<int> AStar::GetShortestPath()
{
   std::vector<int> wayPointList;
   //ゴール頂点を現在の頂点に設定する
   CellVertex currentVertex = m_vertexList[m_goalVertex.GetVertexNumber()];
   //現在の頂点番号がスタート頂点番号と同じ間繰り返す
   while (currentVertex.GetVertexNumber() != m_startVertex.GetVertexNumber())
   {
	  //ウェイポイントに現在の頂点番号を追加する
	  wayPointList.push_back(currentVertex.GetVertexNumber());
	  //親頂点を現在の頂点に設定する
	  currentVertex = m_vertexList[currentVertex.GetParentVertexIndex()];
   }
   //ウェイポイントに現在の頂点番号を追加する
   wayPointList.push_back(currentVertex.GetVertexNumber());
   //ウェイポイント配列を反転させる
   std::reverse(wayPointList.begin(), wayPointList.end());

   return wayPointList;
}

/// <summary>
/// 頂点がオープンリストに存在しているかどうか調べる
/// </summary>
/// <param name="currentVertex">調べる頂点</param>
/// <returns></returns>
const bool AStar::GetVertexInOpenList(const CellVertex& currentVertex)
{
   return std::any_of(
	  m_openList.begin(),
	  m_openList.end(),
	  [currentVertex](const CellVertex& vertex)
	  {
		 return vertex.GetVertexNumber() == currentVertex.GetVertexNumber();
	  });
}

/// <summary>
/// 頂点がクローズドリストに存在しているかどうか調べる	
/// </summary>
/// <param name="currentVartex">調べる頂点</param>
/// <returns></returns>
bool AStar::GetVertexInClosedList(const CellVertex& currentVartex)
{
   return m_closedList.find(currentVartex.GetVertexNumber()) != m_closedList.end();
}

/// <summary>
/// ユークリッド距離を計算する
/// </summary>
/// <param name="vertex">開始頂点</param>
/// <param name="vertex1">終了頂点</param>
/// <returns></returns>
float AStar::EuclideanDistance(const CellVertex& vertex, const CellVertex& vertex1)
{
   //X軸幅を計算する
   float width = vertex.GetPosition().x - vertex1.GetPosition().x;
   //Z軸幅を計算する
   float height = vertex.GetPosition().z - vertex1.GetPosition().z;
   return sqrtf((width * width) + (height * height));
}

/// <summary>
/// マンハッタン距離を計算する
/// </summary>
/// <param name="vertex">開始頂点</param>
/// <param name="vertex1">終了頂点</param>
/// <returns></returns>
float AStar::ManhattanDistance(const CellVertex& vertex, const CellVertex& vertex1)
{
   // 幅を計算する
   float width = abs(vertex.GetPosition().x - vertex1.GetPosition().x);
   // 高さを計算する
   float height = abs(vertex.GetPosition().z - vertex1.GetPosition().z);
   // マンハッタン距離を返す
   return width + height;
}

/// <summary>
/// スタート頂点の初期化
/// </summary>
/// <param name="startVertex">スタート頂点</param>
/// <param name="parentIndex">親</param>
/// <param name="cost">コスト</param>
/// <param name="heuristic">ヒューリスティックコスト</param>
void AStar::InitStartVertex(CellVertex& startVertex, int parentIndex, float cost, float heuristic)
{
   startVertex.SetParentVertexIndex(parentIndex);
   startVertex.SetCost(cost);
   startVertex.SetHeuristic(heuristic);
   startVertex.SetScore(cost + heuristic);
}

/// <summary>
/// 最も近いゴールへのヒューリスティック値を計算
/// </summary>
/// <param name="from"></param>
/// <param name="goalSet"></param>
/// <returns></returns>
float AStar::CalcMinHeuristic(const CellVertex& from, const std::unordered_set<int>& goalSet)
{
   float minHeuristic = std::numeric_limits<float>::max();
   for (int goalIdx : goalSet)
   {
	  float h = EuclideanDistance(from, m_vertexList[goalIdx]);
	  if (h < minHeuristic) minHeuristic = h;
   }
   return minHeuristic;
}
