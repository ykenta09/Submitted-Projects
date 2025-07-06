#pragma once
#ifndef ASTAR_GRAPH_DEFINED
#define ASTAR_GRAPH_DEFINED

#include <pch.h>
#include "Libraries/yamadaLib/Graphics.h"
#include <GraphEditor/Vertex.h>
#include "Game/Astar/CellVertex.h"
#include "Game/Map/Map.h"
#include "Libraries/yamadaLib/GameParameter.h"

class GimmickObjectBase;

//経路探索グラフ
class AStarGraph
{
public:   
   //頂点リストを取得する
   const std::vector<CellVertex> GetVertexList(const int& stageNumber) { return m_vertexList[stageNumber]; }
   //隣接リストを取得する
   const std::vector<std::vector<int>> GetAdjaceneyList(const int& stageNumber) { return m_adjacencyList[stageNumber]; }
   //座標からグリッドのどのセルカ取得する
   CellVertex* GetVertexFromPosition(const int& stageNumber, const DirectX::SimpleMath::Vector3& position);
   //接続ノード配列を取得する
   std::vector<GameParameters::StageTransition> GetStageTransition() { return m_stageTransition; }
public:
    // コンストラクタ
    AStarGraph(Map* map);
    //頂点リストを作成する
    void CreateVertexList();
    //隣接リストを登録する関数
    void AddAdjacentVertices(int stageNumber, int x, int z);
    //グリッド範囲内か確認する
    bool IsValidCell(int x, int z) const;
    //床があるか確認する
    bool IsFloorCell(int stageNumber, const DirectX::SimpleMath::Vector3& position);

    //動的グラフをベースグラフで初期化
    void InitializeDynamicGraph();
    //動的オブジェクトを更新（外部から呼び出す用）
    void UpdateDynamicObjects();
private:
   //単一ギミックの影響を適用
   void ApplySingleGimmick(const GimmickObjectBase* gimmick);
   //隣接リストを再構築
   void RebuildAdjacencyList();
   //ステージ単位で隣接リストを再構築
   void RebuildStageAdjacency(int stageNumber);
   //セルの隣接関係を更新
   void UpdateAdjacencyForCell(int stage, int x, int z);
   //ギミックオブジェクトをリストに反映
   void ApplyDynamicGimmicks();
private:
   yamadalib::Graphics* m_graphics;
   //マップ
   Map* m_map;

   //縦と横
   int m_width;
   int m_height;

   // === 初期リスト ===
   //頂点リスト
   std::unordered_map<int, std::vector<CellVertex>> m_baseVertexList;
   //隣接リスト
   std::unordered_map<int, std::vector<std::vector<int>>> m_baseAdjacencyList;

   // === 他クラスが取得するリスト(ギミックの情報を反映したグラフ) ===
   //頂点リスト
   std::unordered_map<int, std::vector<CellVertex>> m_vertexList;
   //隣接リスト
   std::unordered_map<int, std::vector<std::vector<int>>> m_adjacencyList;
   //接続ノード配列
   std::vector<GameParameters::StageTransition> m_stageTransition;
   //ステージ番号
   int m_stageNumber;
};
#endif // !ASTAR_GRAPH_DEFINED
