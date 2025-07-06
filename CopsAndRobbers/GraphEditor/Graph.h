/*
	@file	Graph.h
	@brief	�O���t�N���X
*/
#pragma once
#ifndef GRAPH_DEFINED
#define GRAPH_DEFINED
#include <memory>
#include <vector>
//#include <algorithm>
#include <stralign.h>
#include "Libraries/yamadaLib/Graphics.h"
#include "GraphEditor/Vertex.h"
#include <unordered_map>

class Graph
{	
public:
	struct PatrolRoute    //�e�G�̏��񃋁[�g
	{
		std::vector<std::vector<int>> adjacencyList;
	};
public:
	//�w�肳�ꂽ�C���f�b�N�X��vertex���擾����
	const Vertex& GetVertex(int vertexIndex) { return m_vertexList[vertexIndex]; }
	//���_���X�g���擾����
	const std::vector<Vertex>& GetVertexList() { return m_vertexList; }
	//�אڃ��X�g���擾����
	const std::unordered_map<int, PatrolRoute>& GetAdjacencyList() { return m_adjacencyList; }
	//���񃋁[�g���擾����
	const std::vector< DirectX::SimpleMath::Vector3>& GetPatrolRootMap() { return m_patrolRootMap; }
	//�G�̐����擾����
	int GetEnemyIndex() { return m_enemyIndex; }
	//�G�̏������_�ԍ����擾����
	int GetInitailVertex(int number) { 
	   if (m_initialVertexNumber.size() > 0)
	   {
		  return m_initialVertexNumber[number][0];
	   }
    }
public:
	//�R���X�g���N�^
	Graph();
	//�f�X�g���N�^
	~Graph();

	//�O���t��������
	void Clear();
	//���_�𐶐�����
	bool CreateVertex(const int enemyIndex, const Vertex& vertex);
	// ���_���X�g�̒��_�ʒu���X�P�[�����O����
	void ScaleVertexPositionOfVertexList(const float& scale);
	// �אڃ��X�g�ɃG�b�W�̒��_��ǉ�����
	bool AddVertexOfEdgeToAdjacencyList(const int& enemyNumber, const int& vertexIndex, const int& vertexOfEdge);
	// ���_���X�g���璸�_���폜����
	void RemoveVertexFromVertexListByIndex(const int& index);
	// �אڃ��X�g����G�b�W�̒��_���폜����
	void RemoveVertexOfEdgeFromAdjacencyListByIndex(const int& enemyNumber, const int& index);
	// ���_���폜����
	bool RemoveVertexFromVertexList(const Vertex& vertex);
	// ���_���X�g���璸�_���폜����
	bool RemoveVertexFromVertexList(const int& vertexIndex);

	// �אڃ��X�g�̃G�b�W�̒��_���폜����
	bool RemoveVertexOfEdgeFromAdjacencyList(const int& enemyNumber, const int& vertexNumber);
	// �אڃ��X�g����C���f�b�N�X�Ԗڂ̒��_�̃G�b�W�C���f�b�N�X�Ԗڂ̃G�b�W���폜����
	void RemoveEgdeFromAdjacencyListByIndex(const int& enemyNumber, const int& index, const int& edgeIndex);
	// ���_�ԍ����Đݒ肷��
	void ReNumber();
	// JSON�`���ŃO���t�f�[�^��ۑ�����
	bool SaveGraphDataByJSON(int index);
	// JSON�`���ŃO���t�f�[�^��ǂݍ���
	bool LoadGraphDataByJSON(const int stageNumber);
	// �f�[�^��ϊ�����
	void ParseJSON();
	//��̗אڃ��X�g�𐶐�����
	void NewAdjacencyList(const int& enemyNumber);
	//����̓G�ԍ��̗אڃ��X�g���폜����
	void ClearNumberAdjacenyList(const int& enemyNumber);
	//�������_�ԍ��𐶐�����
	void CreateInitailVertex(const int& enemyNumber, const int vertexNumber);
private:
	//���_���X�g(�S�Ă̓G�����L���钸�_�z��)
	std::vector<Vertex> m_vertexList;
	//�אڃ��X�g<�G�̔ԍ�,���_�ԍ�,�q�����Ă��钸�_�ԍ�>
	std::unordered_map<int, PatrolRoute> m_adjacencyList;

	// ���̓O���t�f�[�^
	std::vector<std::string> m_graphData;
	//�X�e�[�W�̂��ׂĂ̏��񃋁[�g
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRootMap;
	//�e�G�̏��񃋁[�g�אڃ��X�g<�G�̔ԍ��A���[�g�ԍ��A�q�����Ă��郋�[�g�ԍ�>
	std::unordered_map<int, std::vector<int>> m_patrolAdjacencyList;
	//�G�̐�
	int m_enemyIndex;
	//�G�̏������_�ԍ�
	std::vector<std::vector<int>> m_initialVertexNumber;
};
#endif // GRAPH_DEFINED

