#include "pch.h"
#include "Game/Astar/LineOfSite.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Map/Map.h"

/// <summary>
/// コンストラクタ
/// </summary>
LineOfSite::LineOfSite()
{
}


/// <summary>
/// デストラクタ
/// </summary>
LineOfSite::~LineOfSite()
{
}


/// <summary>
/// クリア
/// </summary>
void LineOfSite::Clear()
{
}


/// <summary>
/// LOSをチェックする
/// </summary>
/// <param name="wayPointList">最短経路ノードのリスト</param>
/// <param name="vertexList">頂点リスト</param>
/// <param name="obstacles">障害物リスト</param>
/// <returns></returns>
std::vector<int> LineOfSite::SmoothPathWithLOS(
   const std::vector<int>& wayPointList,                        
   const std::vector<CellVertex>& vertexList,                   
   const std::vector<DirectX::BoundingBox>& obstacles           
)
{
   // スムージングされた経路
   std::vector<int> smoothedPath;
   int currentIndex = 0;

   while (currentIndex < wayPointList.size() - 1)
   {
      int nextIndex = static_cast<int>(wayPointList.size() - 1); // 仮の次ノードとして最後を設定

      // 現在のノードとすべての次ノード間でLOSをチェック
      for (int i = currentIndex + 1; i < wayPointList.size(); ++i)
      {
         const auto& start = vertexList[wayPointList[currentIndex]].GetPosition();
         const auto& end = vertexList[wayPointList[i]].GetPosition();

         // 境界球と障害物の判定
         if (IsLineOfSightClear(start, end, obstacles))
         {
            nextIndex = i; // LOSがクリアなら次のノード候補に設定
         }
         else
         {
            break; // LOSが遮られたら探索を終了
         }
      }

      // LOSがクリアなノードをスムージングされた経路に追加
      smoothedPath.push_back(wayPointList[currentIndex]);
      currentIndex = nextIndex; // 次のノードに移動
   } 
   // 最後のノードを追加
   smoothedPath.push_back(wayPointList.back());

   return smoothedPath;
}

/// <summary>
/// 頂点間のレイがオブジェクトと衝突しているか判定
/// </summary>
/// <param name="start">開始頂点</param>
/// <param name="end">終了頂点</param>
/// <param name="obstacles">オブジェクトの境界ボックス</param>
/// <returns></returns>
bool LineOfSite::IsLineOfSightClear(const DirectX::SimpleMath::Vector3& start,
   const DirectX::SimpleMath::Vector3& end,
   const std::vector<DirectX::BoundingBox>& obstacles)
{
   //頂点間の距離
   DirectX::SimpleMath::Vector3 direction = end - start;
   //ターゲットまでの距離
   float targetDistance = direction.Length();
   direction.Normalize();
   //レイの作成
   DirectX::SimpleMath::Ray ray(start, direction);

   //障害物判定をレイを使って行う
   for (const auto& obstacle : obstacles) 
   {
      float distance = 0;
      if (obstacle.Intersects(ray.position, ray.direction, distance)) 
      {
         if (distance < targetDistance) 
         {
            return false; // 障害物がある
         }
      }
   }
   return true; // 障害物がないので、LOSがクリア
}


std::vector<GameParameters::MultiStageNode> LineOfSite::SmoothMultiStagePathWithLOS(const std::vector<GameParameters::MultiStageNode>& path, AStarGraph* graph, Map* map)
{
   std::vector<GameParameters::MultiStageNode> result;
   if (path.empty()) return result;

   int i = 0;
   while (i < path.size()) 
   {
      int currentStage = path[i].stage;
      // 区間抽出
      int j = i;
      while (j + 1 < path.size() && path[j + 1].stage == currentStage) ++j;

      // 区間waypointリスト
      std::vector<int> waypoints;
      for (int k = i; k <= j; ++k) waypoints.push_back(path[k].node);

      // LOSスムージング
      auto smoothed = SmoothPathWithLOS(
         waypoints,
         graph->GetVertexList(currentStage),
         map->GetStageBoundingBoxes(currentStage)
      );

      // 追加
      for (int nodeIdx : smoothed)
         result.push_back({ currentStage, nodeIdx });

      i = j + 1;
   }
   return result;
}


