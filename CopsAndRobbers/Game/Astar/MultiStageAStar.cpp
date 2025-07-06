#include <pch.h>
#include "Game/Astar/AStarGraph.h"
#include "Libraries/MyLib/Graphics.h"
#include <algorithm>
#include <cstddef>
#include "Game/Astar/MultiStageAStar.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="graphs">全ステージのグラフ</param>
/// <param name="transitions">ステージ間接続ノードリスト</param>
MultiStageAStar::MultiStageAStar(AStarGraph* graphs, const std::vector<GameParameters::StageTransition>& transitions)
   : 
   m_graph(graphs),
   m_transitions(transitions)
{
}


/// <summary>
/// 経路探索
/// </summary>
/// <param name="startStage">開始ステージ番号</param>
/// <param name="startNode">開始ノード番号</param>
/// <param name="goalStage">目標ステージ番号</param>
/// <param name="goalNode">目標ノード番号</param>
/// <returns></returns>
std::vector<GameParameters::MultiStageNode> MultiStageAStar::Search(
   int startStage, int startNode,
   int goalStage, int goalNode
)
{
   // 1. スタートとゴールが同じステージなら通常A*
   if (startStage == goalStage) 
   {
      std::vector<GameParameters::MultiStageNode> result;
      AStar astar(m_graph, startStage);
      astar.Initialize();
      //探索失敗時は空のリストを返す
      if (!astar.Search(startNode, goalNode)) return {};
      // 経路復元（ノード番号のリストをMultiStageNodeに変換）
      for (int path : astar.GetShortestPath())
      {
         result.push_back({ startStage, path });
      }
      return result;
   }

   // 2. 異なるステージの場合は、まず出口ノードを探す
   // このステージから出られる出口ノード（StageTransition）をすべて列挙
   std::vector<const GameParameters::StageTransition*> exits;
   std::vector<int> exitNodeNumbers;

   for (const auto& transition : m_transitions)
   {
      if (transition.fromStage == startStage)
      {
         exits.push_back(&transition);
         exitNodeNumbers.push_back(transition.fromNode);
      }
   }
   // 出口がない
   if (exits.empty()) return {}; 

   // 3. スタートから各出口ノードまでA*で探索し、最短のものを選ぶ
   AStar astar(m_graph, startStage);
   astar.Initialize();
   if (!astar.SearchToAnyGoal(startNode, exitNodeNumbers)) return {};

   //実際に到達した出口ノードを特定
   int reachedExitNode = astar.GetReachedGoalNode();
   const GameParameters::StageTransition* bestExit = nullptr;
   for (const auto* exit : exits)
   {
      if (exit->fromNode == reachedExitNode)
      {
         bestExit = exit;
         break;
      }
   }

   // 行ける出口がなければ経路なし
   if (!bestExit) return {};

   auto bestPath = astar.GetShortestPath();

   // 4. 出口ノード→入口ノードはワープ（コスト0 or 固定コスト）扱い
   // 5. 入口ノードからゴールまで再帰的に探索
   auto nextPath = Search(bestExit->toStage, bestExit->toNode, goalStage, goalNode);

   // 6. 経路を連結して返す
   std::vector<GameParameters::MultiStageNode> result;
   // スタート→出口までの経路
   for (int path : bestPath)
   {
      result.push_back({ startStage, path });
   }
   // 入口ノードは重複するので nextPath の先頭はスキップして連結
   if (!nextPath.empty()) 
      result.insert(result.end(), nextPath.begin() + 1, nextPath.end());
   return result;
}