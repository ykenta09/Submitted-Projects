#include <pch.h>
#include "Game/Astar/AStarGraph.h"
#include "Libraries/MyLib/Graphics.h"
#include <algorithm>
#include <cstddef>
#include "Game/Astar/MultiStageAStar.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="graphs">�S�X�e�[�W�̃O���t</param>
/// <param name="transitions">�X�e�[�W�Ԑڑ��m�[�h���X�g</param>
MultiStageAStar::MultiStageAStar(AStarGraph* graphs, const std::vector<GameParameters::StageTransition>& transitions)
   : 
   m_graph(graphs),
   m_transitions(transitions)
{
}


/// <summary>
/// �o�H�T��
/// </summary>
/// <param name="startStage">�J�n�X�e�[�W�ԍ�</param>
/// <param name="startNode">�J�n�m�[�h�ԍ�</param>
/// <param name="goalStage">�ڕW�X�e�[�W�ԍ�</param>
/// <param name="goalNode">�ڕW�m�[�h�ԍ�</param>
/// <returns></returns>
std::vector<GameParameters::MultiStageNode> MultiStageAStar::Search(
   int startStage, int startNode,
   int goalStage, int goalNode
)
{
   // 1. �X�^�[�g�ƃS�[���������X�e�[�W�Ȃ�ʏ�A*
   if (startStage == goalStage) 
   {
      std::vector<GameParameters::MultiStageNode> result;
      AStar astar(m_graph, startStage);
      astar.Initialize();
      //�T�����s���͋�̃��X�g��Ԃ�
      if (!astar.Search(startNode, goalNode)) return {};
      // �o�H�����i�m�[�h�ԍ��̃��X�g��MultiStageNode�ɕϊ��j
      for (int path : astar.GetShortestPath())
      {
         result.push_back({ startStage, path });
      }
      return result;
   }

   // 2. �قȂ�X�e�[�W�̏ꍇ�́A�܂��o���m�[�h��T��
   // ���̃X�e�[�W����o����o���m�[�h�iStageTransition�j�����ׂė�
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
   // �o�����Ȃ�
   if (exits.empty()) return {}; 

   // 3. �X�^�[�g����e�o���m�[�h�܂�A*�ŒT�����A�ŒZ�̂��̂�I��
   AStar astar(m_graph, startStage);
   astar.Initialize();
   if (!astar.SearchToAnyGoal(startNode, exitNodeNumbers)) return {};

   //���ۂɓ��B�����o���m�[�h�����
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

   // �s����o�����Ȃ���Όo�H�Ȃ�
   if (!bestExit) return {};

   auto bestPath = astar.GetShortestPath();

   // 4. �o���m�[�h�������m�[�h�̓��[�v�i�R�X�g0 or �Œ�R�X�g�j����
   // 5. �����m�[�h����S�[���܂ōċA�I�ɒT��
   auto nextPath = Search(bestExit->toStage, bestExit->toNode, goalStage, goalNode);

   // 6. �o�H��A�����ĕԂ�
   std::vector<GameParameters::MultiStageNode> result;
   // �X�^�[�g���o���܂ł̌o�H
   for (int path : bestPath)
   {
      result.push_back({ startStage, path });
   }
   // �����m�[�h�͏d������̂� nextPath �̐擪�̓X�L�b�v���ĘA��
   if (!nextPath.empty()) 
      result.insert(result.end(), nextPath.begin() + 1, nextPath.end());
   return result;
}