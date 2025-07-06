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


//�����̃X�e�[�W�O���t���g�p����o�H�T��
class MultiStageAStar
{
public:
public:
    // �R���X�g���N�^
    MultiStageAStar(AStarGraph* graphs, const std::vector<GameParameters::StageTransition>& transitions);

    std::vector<GameParameters::MultiStageNode> Search(int startStage, int startNode, int goalStage, int goalNode);
    //�o�H�T��
private:
   AStarGraph* m_graph;      //�X�e�[�W�ԍ����Ƃ̃O���t
   std::vector<GameParameters::StageTransition> m_transitions;          //�X�e�[�W�Ԑڑ��m�[�h

};
#endif // !MULTI_STAGE_ASTAR_DEFINED
