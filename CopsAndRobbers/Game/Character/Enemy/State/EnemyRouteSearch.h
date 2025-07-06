/*
	@file	EnemyRouteSearch.h
	@brief	�G�̌o�H�T����ԃN���X
*/
#ifndef ENEMY_ROUTE_SEARCH_DEFINED
#define ENEMY_ROUTE_SEARCH_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Astar/AStarGraph.h"
#include "Game/Astar/LineOfSite.h"
#include "Game/Map/Map.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// �O���錾
class CommonResources;
class Enemy;
class GraphScene;

class EnemyRouteSearch : public IState
{
public:
   static constexpr float TIME_TO_SEARCH = { 0.5f };		//�o�H�T���܂ł̎���
public:
   //�O���t���擾����
   AStarGraph* GetGrid() const { return m_graph; }
   //�^�[�Q�b�g�̃Z�����擾����
   CellVertex* GetTargetCell() const { return m_targetCell; }
   //�ŏ��̒T�����ǂ����擾����
   bool GetFirstExploration() const { return m_isFirstExploration; }
   //�ŏ��̒T���t���O��ݒ肷��
   void SetFirstExploration(bool flag) { m_isFirstExploration = flag; }
   int GetTargetStageNumber() const { return m_tagetStageNumber; }
   //�^�[�Q�b�g�̃X�e�[�W�ԍ���ݒ肷��
   void SetTargetStageNumber(const int& stageNumber) { m_tagetStageNumber = stageNumber; }
   //�o�H�T�����Ԃ��Z�b�g����
   void SetSearchTime(const float& time) { m_searchTime = time; }

public:
	//�R���X�g���N�^
    EnemyRouteSearch(Enemy* enemy, Map* map, const int& stageNumber);
	//�f�X�g���N�^
	~EnemyRouteSearch();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();
	//Astar�̏���������
	void SetAstar(DirectX::SimpleMath::Vector3 target);
private:
    CommonResources* m_commonResources;    // ���ʃ��\�[�X
	Enemy* m_enemy;						   //�G
    yamadalib::Graphics* m_graphics;	   //�O���t�B�b�N�X�@�f�o�b�N�p
	Map* m_map;							   //�}�b�v
	CellVertex* m_targetCell;			   //�^�[�Q�b�g�̃Z��

	AStarGraph* m_graph;		//�O���t
	std::unique_ptr<LineOfSite> m_lineOfSite;	//���C���I�u�T�C�g

	DirectX::SimpleMath::Vector3 m_targetPosition;	//�^�[�Q�b�g�̍��W

	//�^�[�Q�b�g���X�e�[�W�ԍ�
	int m_tagetStageNumber;

	bool m_isFirstExploration;	//�n�߂Ă̒T����
	float m_searchTime;		    //�T������
	int m_stageNumber;		    //�X�e�[�W�ԍ�

};
#endif // ENEMY_ROUTE_SEARCH_DEFINED
