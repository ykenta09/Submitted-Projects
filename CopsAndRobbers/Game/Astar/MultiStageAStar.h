#pragma once
#ifndef MULTI_STAGE_ASTAR_DEFINED
#define MULTI_STAGE_ASTAR_DEFINED

#include <pch.h>
#include "Libraries/yamadaLib/Graphics.h"
#include <GraphEditor/Vertex.h>
#include "Game/Astar/CellVertex.h"
#include "Game/Map/Map.h"
#include "Libraries/yamadaLib/GameParameter.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Astar/AStar.h"


//複数のステージグラフを使用する経路探索
class MultiStageAStar
{
public:
public:
    // コンストラクタ
    MultiStageAStar(AStarGraph* graphs, const std::vector<GameParameters::StageTransition>& transitions);

    std::vector<GameParameters::MultiStageNode> Search(int startStage, int startNode, int goalStage, int goalNode);
    //経路探索
private:
   AStarGraph* m_graph;      //ステージ番号ごとのグラフ
   std::vector<GameParameters::StageTransition> m_transitions;          //ステージ間接続ノード

};
#endif // !MULTI_STAGE_ASTAR_DEFINED
