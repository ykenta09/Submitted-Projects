#include <pch.h>
#include "Game/Astar/AStarGraph.h"
#include "Libraries/MyLib/Graphics.h"
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Game/Collision/CollisionObject.h"
#include <algorithm>
#include <cstddef>


/// <summary>
/// �O���b�h�̕��ƍ�����ݒ肵�Am_grid�̃T�C�Y��ݒ�
/// </summary>
/// <param name="map">�}�b�v</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
AStarGraph::AStarGraph(Map* map)
   : 
   m_map(map)
{
   //���_���X�g�Ɨאڃ��X�g���쐬����
   CreateVertexList();
   //���I�O���t���x�[�X�O���t�ŏ�����
   InitializeDynamicGraph();
   //�M�~�b�N�I�u�W�F�N�g�����X�g�ɔ��f
   ApplyDynamicGimmicks();

   m_graphics = yamadalib::Graphics::GetInstance(); // Graphics�C���X�^���X���擾
   m_stageTransition = m_map->GetStageTransition();
}


/// <summary>
/// ���_���X�g���쐬����
/// </summary>
void AStarGraph::CreateVertexList()
{
   //���_���X�g�Ɨאڃ��X�g���N���A
   m_baseVertexList.clear();
   m_baseAdjacencyList.clear();

   //�e�X�e�[�W�̒��_���X�g���쐬
   for (int stageNum = GameParameters::MIN_STAGE_NUM; stageNum < GameParameters::MAX_STAGE_NUM; stageNum++)
   {
      //�}�b�v�f�[�^���擾����
      const auto& mapData = m_map->GetMapData(stageNum);
      //�X�e�[�W�̒��S���W���擾����
      DirectX::SimpleMath::Vector3 stageCenter = m_map->GetStageCenterPosition(stageNum);
      //�}�b�v�̃T�C�Y��ݒ肷��
      m_width = static_cast<int>(mapData.size());
      m_height = static_cast<int>(mapData.size());

      m_baseVertexList[stageNum].resize(m_width * m_height);
      m_baseAdjacencyList[stageNum].resize(m_width * m_height);

      for (int z = 0; z < m_height; z++)
      {
         for (int x = 0; x < m_width; x++)
         {
            //�Z���f�[�^���擾
            const auto& cell = mapData[z][x];
            int vertexNumber = z * m_width + x;

            //���W�v�Z(���S�����_)
            float worldX = static_cast<float>(x - m_width / 2) + stageCenter.x;
            float worldZ = static_cast<float>(z - m_height / 2) + stageCenter.z;
            DirectX::SimpleMath::Vector3 position(worldX, 0.0f, worldZ);

            //CellVertex�̍쐬
            CellVertex vertex(position);
            vertex.SetVertexNumber(vertexNumber);
            vertex.SetState(cell.objectID);

            //�O���b�h���𒸓_���X�g�ɓo�^����
            m_baseVertexList[stageNum][vertexNumber] = vertex;

            //�אڃ��X�g���쐬���钸�_���ʍs�\�Ȃ烊�X�g���쐬����
            if (cell.objectID == 1)
            {
               AddAdjacentVertices(stageNum, x, z);
            }
         }
      }
   }
}


/// <summary>
/// �אڃ��X�g��o�^����֐�
/// </summary>
/// <param name="x">���_���X�g�̂�</param>
/// <param name="z">���_���X�g�̂�</param>
void AStarGraph::AddAdjacentVertices(int stageNumber, int x, int z)
{
   //���݂̒��_�ԍ�
   int currentVertexNumber = z * m_width + x;
   // �אڕ����̑��΍��W�i�㉺���E�j
   constexpr int dx[] = { 0,  0, -1,  1};
   constexpr int dz[] = { -1,  1,  0,  0};
   //�X�e�[�W�f�[�^���擾
   const auto& mapData = m_map->GetMapData(stageNumber);

   for (int i = 0; i < 4; i++)
   {
      int neighborX = x + dx[i];
      int neighborZ = z + dz[i];
      //�L���ȃZ�����m�F
      if (IsValidCell(neighborX, neighborZ))
      {
         const auto& neighborCell = mapData[neighborZ][neighborX];
         //�Z�����ʍs�\���m�F
         if (neighborCell.objectID == 1)
         {
            //�אڒ��_�̒��_�ԍ���ݒ肵�A�אڃ��X�g�Ɋi�[
            int neighborVertexNumber = neighborZ * m_width + neighborX;
            m_baseAdjacencyList[stageNumber][currentVertexNumber].push_back(neighborVertexNumber);
         }
      }
   }
}

/// <summary>
/// ���W����O���t�f�[�^�֕ϊ�����
/// </summary>
/// <param name="position">�ϊ�������W</param>
/// <returns></returns>
CellVertex* AStarGraph::GetVertexFromPosition(const int& stageNumber, const DirectX::SimpleMath::Vector3& position)
{
   DirectX::SimpleMath::Vector3 stageCenter = m_map->GetStageCenterPosition(stageNumber);
   DirectX::SimpleMath::Vector3 localPosition = position - stageCenter;
   // �Z���̒��S�ɋ߂��Z���փ}�b�s���O
   int xIndex = static_cast<int>(std::round(localPosition.x + (m_width / 2.0f)));
   int zIndex = static_cast<int>(std::round(localPosition.z + (m_height / 2.0f)));

   if (xIndex < 0 || xIndex >= m_width || zIndex < 0 || zIndex >= m_height)
      return nullptr;

   int vertexIndex = zIndex * m_width + xIndex;
   if (vertexIndex >= 0 && vertexIndex < static_cast<int>(m_vertexList[stageNumber].size()))
      return &m_vertexList[stageNumber][vertexIndex];

   return nullptr;
}

/// <summary>
/// �Z�����L�����ǂ������m�F����֐�
/// </summary>
/// <param name="x">���ׂ�Z��</param>
/// <param name="z">���ׂ�Z��</param>
/// <returns></returns>
bool AStarGraph::IsValidCell(int x, int z) const
{
   return x >= 0 && x < m_width && z >= 0 && z < m_height;
}

/// <summary>
/// �ړ������ɏ��̂���Z�����m�F����֐�
/// </summary>
/// <param name="stageNumber"></param>
/// <param name="position"></param>
/// <returns></returns>
bool AStarGraph::IsFloorCell(int stageNumber, const DirectX::SimpleMath::Vector3& position) 
{
   CellVertex* cell = GetVertexFromPosition(stageNumber, position);
   //�Z�����Ȃ��ꍇ���ړ��\(�X�e�[�W�Ԉړ������̂��ɂ��邽��)
   if (!cell) return true;
   //�Z�������݂��A���Ȃ�true
   return cell->GetState() == 1;
}

/// <summary>
/// ���I�O���t���x�[�X�O���t�ŏ�����
/// </summary>
void AStarGraph::InitializeDynamicGraph()
{
   //�x�[�X���X�g���擾����
   m_vertexList = m_baseVertexList;
   m_adjacencyList = m_baseAdjacencyList;
}

/// <summary>
/// ���I�I�u�W�F�N�g���X�V�i�O������Ăяo���p�j
/// </summary>
void AStarGraph::UpdateDynamicObjects()
{
   //���I�O���t���x�[�X�O���t�ŏ�����
   InitializeDynamicGraph();
   //�M�~�b�N�I�u�W�F�N�g�𔽉f����
   ApplyDynamicGimmicks();
}

/// <summary>
/// �M�~�b�N�I�u�W�F�N�g�����X�g�ɔ��f
/// </summary>
void AStarGraph::ApplyDynamicGimmicks()
{
   //�M�~�b�N�I�u�W�F�N�g���擾����
   const std::vector<GimmickObjectBase*> gimmicks = m_map->GetGimmickObjects();

   for (const auto& gimmick : gimmicks)
   {
      //�M�~�b�N�𔽉f
      ApplySingleGimmick(gimmick);
   }

   //�אڃ��X�g���č\�z
   RebuildAdjacencyList();
}

/// <summary>
/// �P��M�~�b�N�̉e����K�p
/// </summary>
/// <param name="gimmick">�K�p����M�~�b�N�I�u�W�F�N�g</param>
void AStarGraph::ApplySingleGimmick(const GimmickObjectBase* gimmick)
{
   //���E�{�b�N�X���擾����
   DirectX::BoundingBox box = gimmick->GetCollisionObject()->GetCollisionParameter().collision.box;

   //�M�~�b�N�̃^�C�v�Œʍs�ۂ�����
   int cellState = 0;
   switch (gimmick->GetType())
   {
   case IMapObject::Type::FLOOR:
      cellState = 1;
      break;
   case IMapObject::Type::WALL:
      cellState = 2; 
      break;
   }

   //�O���b�h�S�̂𑖍�����AABB���̃Z���𔽉f
   const int stageNumber = gimmick->GetObjectStageNumber();
   auto& vertexList = m_vertexList[stageNumber];
   //�O���b�h�̌��_�E�Z���T�C�Y���擾
   DirectX::SimpleMath::Vector3 stageCenter = m_map->GetStageCenterPosition(stageNumber);

   float cellSize = 1.0f; // �Z��1�ӂ̒����i�K�v�ɉ����Đݒ�j

   float localMinX = (box.Center.x - box.Extents.x - stageCenter.x) / cellSize;
   float localMaxX = (box.Center.x + box.Extents.x - stageCenter.x) / cellSize;
   float localMinZ = (box.Center.z - box.Extents.z - stageCenter.z) / cellSize;
   float localMaxZ = (box.Center.z + box.Extents.z - stageCenter.z) / cellSize;
   //���E�{�b�N�X��min/max���O���b�h�C���f�b�N�X�ɕϊ�
   int minX = static_cast<int>(std::ceil(localMinX + m_width / 2.0f));
   int maxX = static_cast<int>(std::floor(localMaxX + m_width / 2.0f));
   int minZ = static_cast<int>(std::ceil(localMinZ + m_height / 2.0f));
   int maxZ = static_cast<int>(std::floor(localMaxZ + m_height / 2.0f));
   //�͈͓��̃Z���ɏ�ԓK�p
   for (int z = minZ; z <= maxZ; ++z)
   {
      for (int x = minX; x <= maxX; ++x)
      {
         if (!IsValidCell(x, z)) continue;
         int vertexIndex = z * m_width + x;
         //�I�u�W�F�N�g�̍������n�ʂɋ߂��ꍇ�̂ݔ��f����
         float cellY = vertexList[vertexIndex].GetPosition().y;
         if (std::abs(cellY - box.Center.y) >= 1.0f) continue;
         vertexList[vertexIndex].SetState(cellState);
      }
   }
}

/// <summary>
/// �אڃ��X�g���č\�z
/// </summary>
void AStarGraph::RebuildAdjacencyList()
{
   for (int stageNum = GameParameters::MIN_STAGE_NUM; stageNum < GameParameters::MAX_STAGE_NUM; stageNum++)
   {
      if (m_vertexList.find(stageNum) == m_vertexList.end()) continue;

      // �X�e�[�W���Ƃ̗אڃ��X�g���č\�z
      RebuildStageAdjacency(stageNum);
   }
}

/// <summary>
/// �X�e�[�W�P�ʂŗאڃ��X�g���č\�z
/// </summary>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
void AStarGraph::RebuildStageAdjacency(int stageNumber)
{
   //�T�C�Y�ݒ�
   m_width = static_cast<int>(std::sqrt(m_vertexList[stageNumber].size()));
   m_height = m_width;

   m_adjacencyList[stageNumber].clear();
   m_adjacencyList[stageNumber].resize(m_width * m_height);

   // �e�Z���̗אڊ֌W���X�V
   for (int z = 0; z < m_height; z++)
   {
      for (int x = 0; x < m_width; x++)
      {
         int vertexNumber = z * m_width + x;
         if (m_vertexList[stageNumber][vertexNumber].GetState() == 1) // �ʍs�\
         {
            UpdateAdjacencyForCell(stageNumber, x, z);
         }
      }
   }
}

/// <summary>
/// �Z���̗אڊ֌W���X�V
/// </summary>
/// <param name="stage">�X�e�[�W�o����</param>
/// <param name="x">�Z����x��</param>
/// <param name="z">�Z����z��</param>
void AStarGraph::UpdateAdjacencyForCell(int stage, int x, int z) 
{
   int idx = z * m_width + x;
   m_adjacencyList[stage][idx].clear();

   if (m_vertexList[stage][idx].GetState() == 1)
   {
      // �ʍs�\���`�F�b�N
      // �㉺���E�̃Z�����`�F�b�N
      static constexpr int dx[] = { 0, 0, -1, 1 };
      static constexpr int dz[] = { -1, 1, 0, 0 };

      for (int i = 0; i < 4; ++i) 
      {
         int nx = x + dx[i], nz = z + dz[i];
         if (IsValidCell(nx, nz))
         {
            int nidx = nz * m_width + nx;
            if (m_vertexList[stage][nidx].GetState() == 1)
            {
               m_adjacencyList[stage][idx].push_back(nidx);
            }
         }
      }
   }
}
