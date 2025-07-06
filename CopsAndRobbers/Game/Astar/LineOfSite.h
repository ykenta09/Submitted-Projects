/*
	@file	LineOfSite.h
	@brief	ラインオブサイトクラス
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
   //コンストラクタ
   LineOfSite();
   //デストラクタ
   ~LineOfSite();
   //クリア
   void Clear();

   //LOSをチェックする
   std::vector<int> SmoothPathWithLOS(const std::vector<int>& wayPointList, const std::vector<CellVertex>& vertexList, const std::vector<DirectX::BoundingBox>& obstacles);
   //オブジェクトチェック
   bool IsLineOfSightClear(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, const std::vector<DirectX::BoundingBox>& obstacles);

   std::vector<GameParameters::MultiStageNode> SmoothMultiStagePathWithLOS(
	  const std::vector<GameParameters::MultiStageNode>& path,
	  AStarGraph* graph,
	  Map* map
   );

};

