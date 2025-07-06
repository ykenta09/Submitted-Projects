/*
	@file	AStar.cpp
	@brief	AStar�N���X
*/
#include "pch.h"
#include "Game/Astar/AStar.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/yamadaLib/Resources.h"
#include <cassert>
#include <Game/Screen.h>
#include <Libraries/MyLib/Geometry.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="graph"></param>
AStar::AStar(AStarGraph* graph, const int& stageNumber)
	:
   m_graph(graph),
   m_stageNumber(stageNumber),
   m_openList{},
   m_closedList{},
   m_startVertex(DirectX::SimpleMath::Vector3::Zero),
   m_goalVertex(DirectX::SimpleMath::Vector3::Zero)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
AStar::~AStar()
{
	// do nothing.
}

/// <summary>
/// ����������
/// </summary>
void AStar::Initialize()
{
   //�N���A����
   Clear();
   if (!m_graph) return;

   //���_���X�g�Ɨאڃ��X�g�̃Z�b�g�͏���̂�
   if (m_vertexList.empty())
   {
	  m_vertexList = m_graph->GetVertexList(m_stageNumber);
   }
   if (m_adjacencyList.empty())
   {
	  m_adjacencyList = m_graph->GetAdjaceneyList(m_stageNumber);
   }
}


/// <summary>
///  �T������
/// </summary>
/// <param name="startVertexIndex">�T���J�n���_</param>
/// <param name="goalVertexIndex">�T���ڕW���_</param>
/// <returns></returns>
bool AStar::Search(int startVertexIndex, int goalVertexIndex)
{
   // ���_���X�g�Ɨאڃ��X�g�͏��������ɃZ�b�g�ς݁i�ė��p�j
   Clear();

   //�ŐV�̒��_���X�g�Ɨאڃ��X�g���擾����
   m_vertexList = m_graph->GetVertexList(m_stageNumber);
   m_adjacencyList = m_graph->GetAdjaceneyList(m_stageNumber);

   //�X�^�[�g���_���擾����
   CellVertex startVertex = m_vertexList[startVertexIndex];
   //�S�[�����_���擾����
   CellVertex goalVertex = m_vertexList[goalVertexIndex];

   InitStartVertex(startVertex, -1, 0.0f, EuclideanDistance(startVertex, goalVertex));
   m_openList.insert(startVertex);
   m_closedList.insert(startVertex.GetVertexNumber());

   //�I�[�v�����X�g����ɂȂ�܂ŒT�����J��Ԃ�
   while (!m_openList.empty())
   {
	  //�I�[�v�����X�g�̐擪���_���擾����
	  CellVertex parentVertex = *m_openList.begin();
	  //�I�[�v�����Xt�̐擪���_���폜����
	  m_openList.erase(m_openList.begin());
	  //�e���_�̒��_�ԍ���e���_�C���f�b�N�X�ɐݒ肷��
	  int parentVertexIndex = parentVertex.GetVertexNumber();
	  //�S�[���ɓ��B�����ꍇ�A�T�����I������
	  if (parentVertexIndex == goalVertexIndex)
	  {
		 //�X�^�[�g���_�̏����l��ݒ肷��
		 m_startVertex.SetVertexNumber(startVertexIndex);
		 //�S�[�����_�̏����l��ݒ肷��
		 m_goalVertex.SetVertexNumber(goalVertexIndex);
		 return true;
	  }
	  //�אڃm�[�h��T������(�Ή������ԍ��̗אڃm�[�h��T������)
	  for (int neighborIndex : m_adjacencyList[parentVertexIndex])
	  {
		 //�אڒ��_���擾����
 		 CellVertex& neighborVertex = m_vertexList[neighborIndex];
		 //�אڒ��_���N���[�Y�h���X�g�ɑ��݂��Ă���ꍇ�̓X�L�b�v
		 if (GetVertexInClosedList(neighborVertex)) continue;
		 //�R�X�g���v�Z����
		 float cost = parentVertex.GetCost() + EuclideanDistance(parentVertex, neighborVertex);
		 //�I�[�v�����X�g�ɗאڒ��_�����݂��Ȃ��@|| �R�X�g���אڒ��_�̃R�X�g��菬�����ꍇ
		 if (!GetVertexInOpenList(neighborVertex) || cost < neighborVertex.GetCost())
		 {
			InitStartVertex(neighborVertex, parentVertexIndex, cost, EuclideanDistance(neighborVertex, goalVertex));
			//�I�[�v�����X�g�ɗאڃ��X�g�ɗאڒ��_�����݂��Ă��Ȃ��ꍇ�A���_��ǉ�����
			if (!GetVertexInOpenList(neighborVertex))
			{
			   //�I�[�v�����X�g�ɒǉ�����
			   m_openList.insert(neighborVertex);
			}
		 }
	  }
	  //�N���[�Y�h���X�g�ɐe���_�ԍ���ǉ�����
	  m_closedList.insert(parentVertex.GetVertexNumber());
   }
   //�T���Ɏ��s����
   return false;
}


bool AStar::SearchToAnyGoal(int startVertexIndex, const std::vector<int>& goalVertexIndices)
{
   Clear();

   //�ŐV�̒��_���X�g�Ɨאڃ��X�g���擾����
   m_vertexList = m_graph->GetVertexList(m_stageNumber);
   m_adjacencyList = m_graph->GetAdjaceneyList(m_stageNumber);

   std::unordered_set<int> goalSet(goalVertexIndices.begin(), goalVertexIndices.end());

   // �X�^�[�g���_���擾����
   CellVertex startVertex = m_vertexList[startVertexIndex];
   float heuristic = CalcMinHeuristic(startVertex, goalSet);
   InitStartVertex(startVertex, -1, 0.0f, heuristic);

   m_openList.insert(startVertex);
   m_closedList.insert(startVertex.GetVertexNumber());

   while (!m_openList.empty())
   {
	  CellVertex parentVertex = *m_openList.begin();
	  m_openList.erase(m_openList.begin());
	  int parentVertexIndex = parentVertex.GetVertexNumber();

	  // �S�[���Q�ɓ��B������I��
	  if (goalSet.count(parentVertexIndex))
	  {
		 m_startVertex.SetVertexNumber(startVertexIndex);
		 m_goalVertex.SetVertexNumber(parentVertexIndex);
		 return true;
	  }

	  for (int neighborIndex : m_adjacencyList[parentVertexIndex])
	  {
		 CellVertex& neighborVertex = m_vertexList[neighborIndex];
		 if (GetVertexInClosedList(neighborVertex)) continue;
		 float cost = parentVertex.GetCost() + EuclideanDistance(parentVertex, neighborVertex);

		 // �ł��߂��S�[���ւ̃q���[���X�e�B�b�N
		 heuristic = CalcMinHeuristic(neighborVertex, goalSet);

		 if (!GetVertexInOpenList(neighborVertex) || cost < neighborVertex.GetCost())
		 {
			InitStartVertex(neighborVertex, parentVertexIndex, cost, heuristic);

			if (!GetVertexInOpenList(neighborVertex))
			{
			   m_openList.insert(neighborVertex);
			}
		 }
	  }
	  m_closedList.insert(parentVertex.GetVertexNumber());
   }
   return false;
}


/// <summary>
/// �T����Ԃ��N���A����
/// </summary>
void AStar::Clear()
{
   //�I�[�v�����X�g���N���A����
   m_openList.clear();
   //�N���[�Y�h���X�g���N���A����
   m_closedList.clear();
}

/// <summary>
/// �ŒZ�o�H���擾����
/// </summary>
/// <returns></returns>
const std::vector<int> AStar::GetShortestPath()
{
   std::vector<int> wayPointList;
   //�S�[�����_�����݂̒��_�ɐݒ肷��
   CellVertex currentVertex = m_vertexList[m_goalVertex.GetVertexNumber()];
   //���݂̒��_�ԍ����X�^�[�g���_�ԍ��Ɠ����ԌJ��Ԃ�
   while (currentVertex.GetVertexNumber() != m_startVertex.GetVertexNumber())
   {
	  //�E�F�C�|�C���g�Ɍ��݂̒��_�ԍ���ǉ�����
	  wayPointList.push_back(currentVertex.GetVertexNumber());
	  //�e���_�����݂̒��_�ɐݒ肷��
	  currentVertex = m_vertexList[currentVertex.GetParentVertexIndex()];
   }
   //�E�F�C�|�C���g�Ɍ��݂̒��_�ԍ���ǉ�����
   wayPointList.push_back(currentVertex.GetVertexNumber());
   //�E�F�C�|�C���g�z��𔽓]������
   std::reverse(wayPointList.begin(), wayPointList.end());

   return wayPointList;
}

/// <summary>
/// ���_���I�[�v�����X�g�ɑ��݂��Ă��邩�ǂ������ׂ�
/// </summary>
/// <param name="currentVertex">���ׂ钸�_</param>
/// <returns></returns>
const bool AStar::GetVertexInOpenList(const CellVertex& currentVertex)
{
   return std::any_of(
	  m_openList.begin(),
	  m_openList.end(),
	  [currentVertex](const CellVertex& vertex)
	  {
		 return vertex.GetVertexNumber() == currentVertex.GetVertexNumber();
	  });
}

/// <summary>
/// ���_���N���[�Y�h���X�g�ɑ��݂��Ă��邩�ǂ������ׂ�	
/// </summary>
/// <param name="currentVartex">���ׂ钸�_</param>
/// <returns></returns>
bool AStar::GetVertexInClosedList(const CellVertex& currentVartex)
{
   return m_closedList.find(currentVartex.GetVertexNumber()) != m_closedList.end();
}

/// <summary>
/// ���[�N���b�h�������v�Z����
/// </summary>
/// <param name="vertex">�J�n���_</param>
/// <param name="vertex1">�I�����_</param>
/// <returns></returns>
float AStar::EuclideanDistance(const CellVertex& vertex, const CellVertex& vertex1)
{
   //X�������v�Z����
   float width = vertex.GetPosition().x - vertex1.GetPosition().x;
   //Z�������v�Z����
   float height = vertex.GetPosition().z - vertex1.GetPosition().z;
   return sqrtf((width * width) + (height * height));
}

/// <summary>
/// �}���n�b�^���������v�Z����
/// </summary>
/// <param name="vertex">�J�n���_</param>
/// <param name="vertex1">�I�����_</param>
/// <returns></returns>
float AStar::ManhattanDistance(const CellVertex& vertex, const CellVertex& vertex1)
{
   // �����v�Z����
   float width = abs(vertex.GetPosition().x - vertex1.GetPosition().x);
   // �������v�Z����
   float height = abs(vertex.GetPosition().z - vertex1.GetPosition().z);
   // �}���n�b�^��������Ԃ�
   return width + height;
}

/// <summary>
/// �X�^�[�g���_�̏�����
/// </summary>
/// <param name="startVertex">�X�^�[�g���_</param>
/// <param name="parentIndex">�e</param>
/// <param name="cost">�R�X�g</param>
/// <param name="heuristic">�q���[���X�e�B�b�N�R�X�g</param>
void AStar::InitStartVertex(CellVertex& startVertex, int parentIndex, float cost, float heuristic)
{
   startVertex.SetParentVertexIndex(parentIndex);
   startVertex.SetCost(cost);
   startVertex.SetHeuristic(heuristic);
   startVertex.SetScore(cost + heuristic);
}

/// <summary>
/// �ł��߂��S�[���ւ̃q���[���X�e�B�b�N�l���v�Z
/// </summary>
/// <param name="from"></param>
/// <param name="goalSet"></param>
/// <returns></returns>
float AStar::CalcMinHeuristic(const CellVertex& from, const std::unordered_set<int>& goalSet)
{
   float minHeuristic = std::numeric_limits<float>::max();
   for (int goalIdx : goalSet)
   {
	  float h = EuclideanDistance(from, m_vertexList[goalIdx]);
	  if (h < minHeuristic) minHeuristic = h;
   }
   return minHeuristic;
}
