/*
	@file	LineOfSite.h
	@brief	���C���I�u�T�C�g�N���X
*/
#pragma once
#include "SimpleMath.h"
#include "Game/Astar/CellVertex.h"
#include "Libraries/yamadaLib/GameParameter.h"

class AStarGraph;
class Map;

class LineOfSite
{
public:
   //�R���X�g���N�^
   LineOfSite();
   //�f�X�g���N�^
   ~LineOfSite();
   //�N���A
   void Clear();

   //LOS���`�F�b�N����
   std::vector<int> SmoothPathWithLOS(const std::vector<int>& wayPointList, const std::vector<CellVertex>& vertexList, const std::vector<DirectX::BoundingBox>& obstacles);
   //�I�u�W�F�N�g�`�F�b�N
   bool IsLineOfSightClear(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, const std::vector<DirectX::BoundingBox>& obstacles);

   std::vector<GameParameters::MultiStageNode> SmoothMultiStagePathWithLOS(
	  const std::vector<GameParameters::MultiStageNode>& path,
	  AStarGraph* graph,
	  Map* map
   );

};

