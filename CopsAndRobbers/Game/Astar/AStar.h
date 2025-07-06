/*
	@file	AStar.h
	@brief	AStar�N���X
*/
#pragma once
#ifndef ASTAR_DEFINED
#define ASTAR_DEFINED
#include <unordered_set>
#include "Game/Character/Enemy/Comparator.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Map/Map.h"

// �O���錾
class CommonResources;

//�o�H�T��
class AStar
{
public:
   //�O���t���擾����
   AStarGraph* GetGraph() const { return m_graph; }
   //�O���b�h��ݒ肷��
   void SetGraph(AStarGraph* graph) { m_graph = graph; }

   //�ŒZ�o�H���擾����
   const std::vector<int> GetShortestPath();
   //���_���X�g���擾����
   std::vector<CellVertex> GetVertexList() const { return m_vertexList; }
   //���_���I�[�v�����X�g�ɑ��݂��Ă��邩�ǂ������ׂ�
   const bool GetVertexInOpenList(const CellVertex& currentVertex);
   //���_���N���[�Y���X�g�ɑ��݂��Ă��邩�ǂ���
   bool GetVertexInClosedList(const CellVertex& currentVartex);
public:
   //�R���X�g���N�^
	AStar(AStarGraph* graph, const int& stageNumber);
	//�f�X�g���N�^
   ~AStar();
   //����������
	void Initialize();
	//�N���A����
	void Clear();
	//�T������
	bool Search(int startVertexIndex, int goalVertexIndex);
	//�����S�[���Ή��T��
	bool SearchToAnyGoal(int startVertexIndex, const std::vector<int>& goalVertexIndices);
	//���ۂɓ��B�����S�[���m�[�h
	int GetReachedGoalNode() const { return m_goalVertex.GetVertexNumber(); }
private:
   //���[�N���b�h�������v�Z����
   float EuclideanDistance(const CellVertex& vertex, const CellVertex& vertex1);
   //�}���n�b�^���������v�Z����
   float ManhattanDistance(const CellVertex& vertex, const CellVertex& vertex1);
   //�X�^�[�g���_�̏�����
   void InitStartVertex(CellVertex& startVertex, int parentIndex, float cost, float heuristic);
   //�ł��߂��S�[���ւ̃q���[���X�e�B�b�N
   float CalcMinHeuristic(const CellVertex& from, const std::unordered_set<int>& goalSet);
private:
    //�O���t
    AStarGraph* m_graph;

	//���_���X�g
	std::vector<CellVertex> m_vertexList;
	//�אڃ��X�g
	std::vector<std::vector<int>> m_adjacencyList;

	//�X�^�[�g���_
	CellVertex m_startVertex;
	//�S�[�����_
	CellVertex m_goalVertex;

	//�I�[�v�����X�g
	std::set<CellVertex, Comparator> m_openList;
	//�N���[�Y���X�g
	std::unordered_set<int> m_closedList;

	//�X�e�[�W�ԍ�
	int m_stageNumber;
};
#endif // ASTAR_DEFINED
