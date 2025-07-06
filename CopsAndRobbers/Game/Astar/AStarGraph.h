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

//�o�H�T���O���t
class AStarGraph
{
public:   
   //���_���X�g���擾����
   const std::vector<CellVertex> GetVertexList(const int& stageNumber) { return m_vertexList[stageNumber]; }
   //�אڃ��X�g���擾����
   const std::vector<std::vector<int>> GetAdjaceneyList(const int& stageNumber) { return m_adjacencyList[stageNumber]; }
   //���W����O���b�h�̂ǂ̃Z���J�擾����
   CellVertex* GetVertexFromPosition(const int& stageNumber, const DirectX::SimpleMath::Vector3& position);
   //�ڑ��m�[�h�z����擾����
   std::vector<GameParameters::StageTransition> GetStageTransition() { return m_stageTransition; }
public:
    // �R���X�g���N�^
    AStarGraph(Map* map);
    //���_���X�g���쐬����
    void CreateVertexList();
    //�אڃ��X�g��o�^����֐�
    void AddAdjacentVertices(int stageNumber, int x, int z);
    //�O���b�h�͈͓����m�F����
    bool IsValidCell(int x, int z) const;
    //�������邩�m�F����
    bool IsFloorCell(int stageNumber, const DirectX::SimpleMath::Vector3& position);

    //���I�O���t���x�[�X�O���t�ŏ�����
    void InitializeDynamicGraph();
    //���I�I�u�W�F�N�g���X�V�i�O������Ăяo���p�j
    void UpdateDynamicObjects();
private:
   //�P��M�~�b�N�̉e����K�p
   void ApplySingleGimmick(const GimmickObjectBase* gimmick);
   //�אڃ��X�g���č\�z
   void RebuildAdjacencyList();
   //�X�e�[�W�P�ʂŗאڃ��X�g���č\�z
   void RebuildStageAdjacency(int stageNumber);
   //�Z���̗אڊ֌W���X�V
   void UpdateAdjacencyForCell(int stage, int x, int z);
   //�M�~�b�N�I�u�W�F�N�g�����X�g�ɔ��f
   void ApplyDynamicGimmicks();
private:
   yamadalib::Graphics* m_graphics;
   //�}�b�v
   Map* m_map;

   //�c�Ɖ�
   int m_width;
   int m_height;

   // === �������X�g ===
   //���_���X�g
   std::unordered_map<int, std::vector<CellVertex>> m_baseVertexList;
   //�אڃ��X�g
   std::unordered_map<int, std::vector<std::vector<int>>> m_baseAdjacencyList;

   // === ���N���X���擾���郊�X�g(�M�~�b�N�̏��𔽉f�����O���t) ===
   //���_���X�g
   std::unordered_map<int, std::vector<CellVertex>> m_vertexList;
   //�אڃ��X�g
   std::unordered_map<int, std::vector<std::vector<int>>> m_adjacencyList;
   //�ڑ��m�[�h�z��
   std::vector<GameParameters::StageTransition> m_stageTransition;
   //�X�e�[�W�ԍ�
   int m_stageNumber;
};
#endif // !ASTAR_GRAPH_DEFINED
