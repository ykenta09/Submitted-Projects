/*
	@file	FellowRouteSearch.h
	@brief	���Ԃ̌o�H�T����ԃN���X
*/
#ifndef FELLOW_ROUTE_SEARCH_DEFINED
#define FELLOW_ROUTE_SEARCH_DEFINED
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
class Fellow;
class GraphScene;


class FellowRouteSearch : public IState
{
public:
   const int MAXGRIDSIZE = 60;
   static constexpr float TIME_TO_SEARCH = { 0.5f };		   //�o�H�T���܂ł̎���
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
    FellowRouteSearch(Fellow* fellow, Map* map, const int& stageNumber);
	//�f�X�g���N�^
	~FellowRouteSearch();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();

	void SetAstar(DirectX::SimpleMath::Vector3 target);
private:
	//���Ԃ̃|�C���^
	Fellow* m_fellow;
	//�O���t
	AStarGraph* m_graph;
	//���C���I�u�T�C�g
	std::unique_ptr<LineOfSite> m_lineOfSite;
	//�}�b�v
	Map* m_map;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X�@�f�o�b�N�p
	yamadalib::Graphics* m_graphics;
	//�^�[�Q�b�g�̍��W
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//�O���b�h�̃T�C�Y
	int m_gridSize;
	//�^�[�Q�b�g�̃Z��
	CellVertex* m_targetCell;
	//�n�߂Ă̒T����
	bool m_isFirstExploration;
	//�T������
	float m_searchTime;
	//�X�e�[�W�ԍ�
	int m_stageNumber;
	//�^�[�Q�b�g���X�e�[�W�ԍ�
	int m_tagetStageNumber;

	//�T�����K�v��
	bool m_needSearch;
};
#endif // FELLOW_ROUTE_SEARCH_DEFINED
