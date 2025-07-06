#include "pch.h"
#include "Game/Astar/LineOfSite.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Map/Map.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
LineOfSite::LineOfSite()
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
LineOfSite::~LineOfSite()
{
}


/// <summary>
/// �N���A
/// </summary>
void LineOfSite::Clear()
{
}


/// <summary>
/// LOS���`�F�b�N����
/// </summary>
/// <param name="wayPointList">�ŒZ�o�H�m�[�h�̃��X�g</param>
/// <param name="vertexList">���_���X�g</param>
/// <param name="obstacles">��Q�����X�g</param>
/// <returns></returns>
std::vector<int> LineOfSite::SmoothPathWithLOS(
   const std::vector<int>& wayPointList,                        
   const std::vector<CellVertex>& vertexList,                   
   const std::vector<DirectX::BoundingBox>& obstacles           
)
{
   // �X���[�W���O���ꂽ�o�H
   std::vector<int> smoothedPath;
   int currentIndex = 0;

   while (currentIndex < wayPointList.size() - 1)
   {
      int nextIndex = static_cast<int>(wayPointList.size() - 1); // ���̎��m�[�h�Ƃ��čŌ��ݒ�

      // ���݂̃m�[�h�Ƃ��ׂĂ̎��m�[�h�Ԃ�LOS���`�F�b�N
      for (int i = currentIndex + 1; i < wayPointList.size(); ++i)
      {
         const auto& start = vertexList[wayPointList[currentIndex]].GetPosition();
         const auto& end = vertexList[wayPointList[i]].GetPosition();

         // ���E���Ə�Q���̔���
         if (IsLineOfSightClear(start, end, obstacles))
         {
            nextIndex = i; // LOS���N���A�Ȃ玟�̃m�[�h���ɐݒ�
         }
         else
         {
            break; // LOS���Ղ�ꂽ��T�����I��
         }
      }

      // LOS���N���A�ȃm�[�h���X���[�W���O���ꂽ�o�H�ɒǉ�
      smoothedPath.push_back(wayPointList[currentIndex]);
      currentIndex = nextIndex; // ���̃m�[�h�Ɉړ�
   } 
   // �Ō�̃m�[�h��ǉ�
   smoothedPath.push_back(wayPointList.back());

   return smoothedPath;
}

/// <summary>
/// ���_�Ԃ̃��C���I�u�W�F�N�g�ƏՓ˂��Ă��邩����
/// </summary>
/// <param name="start">�J�n���_</param>
/// <param name="end">�I�����_</param>
/// <param name="obstacles">�I�u�W�F�N�g�̋��E�{�b�N�X</param>
/// <returns></returns>
bool LineOfSite::IsLineOfSightClear(const DirectX::SimpleMath::Vector3& start,
   const DirectX::SimpleMath::Vector3& end,
   const std::vector<DirectX::BoundingBox>& obstacles)
{
   //���_�Ԃ̋���
   DirectX::SimpleMath::Vector3 direction = end - start;
   //�^�[�Q�b�g�܂ł̋���
   float targetDistance = direction.Length();
   direction.Normalize();
   //���C�̍쐬
   DirectX::SimpleMath::Ray ray(start, direction);

   //��Q����������C���g���čs��
   for (const auto& obstacle : obstacles) 
   {
      float distance = 0;
      if (obstacle.Intersects(ray.position, ray.direction, distance)) 
      {
         if (distance < targetDistance) 
         {
            return false; // ��Q��������
         }
      }
   }
   return true; // ��Q�����Ȃ��̂ŁALOS���N���A
}


std::vector<GameParameters::MultiStageNode> LineOfSite::SmoothMultiStagePathWithLOS(const std::vector<GameParameters::MultiStageNode>& path, AStarGraph* graph, Map* map)
{
   std::vector<GameParameters::MultiStageNode> result;
   if (path.empty()) return result;

   int i = 0;
   while (i < path.size()) 
   {
      int currentStage = path[i].stage;
      // ��Ԓ��o
      int j = i;
      while (j + 1 < path.size() && path[j + 1].stage == currentStage) ++j;

      // ���waypoint���X�g
      std::vector<int> waypoints;
      for (int k = i; k <= j; ++k) waypoints.push_back(path[k].node);

      // LOS�X���[�W���O
      auto smoothed = SmoothPathWithLOS(
         waypoints,
         graph->GetVertexList(currentStage),
         map->GetStageBoundingBoxes(currentStage)
      );

      // �ǉ�
      for (int nodeIdx : smoothed)
         result.push_back({ currentStage, nodeIdx });

      i = j + 1;
   }
   return result;
}


