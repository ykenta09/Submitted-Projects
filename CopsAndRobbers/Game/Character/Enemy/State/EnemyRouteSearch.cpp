/*
	@file	EnemyRouteSearch.cpp
	@brief	�G�̌o�H�T����ԃN���X
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyRouteSearch.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"
#include "Game/GameManager/GameManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemy">�G�̃|�C���^</param>
/// <param name="map">�}�b�v���</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
EnemyRouteSearch::EnemyRouteSearch(Enemy* enemy, Map* map, const int& stageNumber)
	:
	m_enemy(enemy),
    m_map(map),
    m_stageNumber(stageNumber),
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_isFirstExploration(false),
    m_searchTime(0.0f)
{
}

/// <summary>
///  �f�X�g���N�^
/// </summary>
EnemyRouteSearch::~EnemyRouteSearch()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void EnemyRouteSearch::Initialize()
{
   //���C���I�u�T�C�g�𐶐�
   m_lineOfSite = std::make_unique<LineOfSite>();
   if (GameManager::GetInstance())
   {
	  m_graph = GameManager::GetInstance()->GetGraph();
   }
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyRouteSearch::Update(const float& elapsedTime)
{
   //���Ԃ̍X�V
   m_searchTime += elapsedTime;

   //�T�����Ԃ𒴂�����T�����I����
   if (m_searchTime >= 4.0f)
   {
	  //�T���Ɏ��Ԃ�������ꍇ�A����o�H�ɖ߂�
	  m_enemy->GetEnemyRouteSearch()->SetAstar(m_enemy->GetEnemyWandering()->GetWanderTarget());
	  m_isFirstExploration = false;
	  m_searchTime = 0.0f;
   }
}


/// <summary>
/// �`�悷��
/// </summary>
void EnemyRouteSearch::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}

/// <summary>
/// ��n������
/// </summary>
void EnemyRouteSearch::Finalize()
{
	// do nothing.
}


/// <summary>
/// AStar�̏���������
/// </summary>
/// <param name="target">�ǐՂ���ڕW�̍��W</param>
void EnemyRouteSearch::SetAstar(DirectX::SimpleMath::Vector3 target)
{
   m_targetPosition = target;

   //�X�^�[�g�E�S�[���Z�����擾
   int startStage = m_enemy->GetObjectStageNumber();
   int goalStage = m_tagetStageNumber;
   CellVertex* startCell = m_graph->GetVertexFromPosition(startStage, m_enemy->GetPosition());
   CellVertex* goalCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   //�X�^�[�g�E�S�[���Z�����Ȃ���Ώ��������Ȃ�
   if (!startCell || !goalCell) return;

   m_targetCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   //�X�e�[�W��A*�Ōo�H�T��
   auto path = m_enemy->GetMultiStageAStar()->Search(
	  startStage, startCell->GetVertexNumber(),
	  goalStage, goalCell->GetVertexNumber()
   );

   if (path.empty()) return;

   auto smoothedPath = m_lineOfSite->SmoothMultiStagePathWithLOS(path, m_graph, m_map);

   //���E�ɓ����Ă���ꍇ�A�ǐՏ�ԁA�����Ă��Ȃ��ꍇ�o�H�ɖ߂��ԂɕύX
   if (m_enemy->InView(false))
   {
	  //�o�H���Z�b�g
	  m_enemy->SetMultiStagePath(smoothedPath);
	  m_enemy->SetWayPointIndex(0);
	  m_enemy->ChangeState(m_enemy->GetEnemyChasing());
	  m_isFirstExploration = true;
   }
   else
   {
	  //�G�m�F�ӏ�
	  m_enemy->ChangeState(m_enemy->GetEnemyWanderingReturn());
   }



   ////�^�[�Q�b�g���O���b�h�Z���͈͓����m�F
   //if (m_graph->GetVertexFromPosition(m_stageNumber, m_targetPosition) == nullptr) return;
   //m_targetCell = m_graph->GetVertexFromPosition(m_stageNumber, m_targetPosition);

   ////A*�A���S���Y���̒T�����s��
   //if (m_enemy->GetAStar()->Search(
	  //m_graph->GetVertexFromPosition(m_stageNumber, m_enemy->GetPosition())->GetVertexNumber(),
	  //m_graph->GetVertexFromPosition(m_stageNumber, m_targetPosition)->GetVertexNumber()))
   //{
	  ////���C���I�u�T�C�g���g���čŒZ�o�H���œK��
	  //auto path = m_lineOfSite->SmoothPathWithLOS(
		 //m_enemy->GetAStar()->GetShortestPath(), 
		 //m_graph->GetVertexList(m_enemy->GetObjectStageNumber()), 
		 //m_map->GetBoundingBoxes());
	  //
	  //m_enemy->SetShortestPath(path);
	  ////�E�F�C�|�C���g�C���f�b�N�X���N���A����
	  //m_enemy->SetWayPointIndex(0);
	  ////���E�ɓ����Ă���ꍇ�A�ǐՏ�ԁA�����Ă��Ȃ��ꍇ�o�H�ɖ߂��ԂɕύX
	  //if (m_enemy->InView(false))
	  //{
		 //m_enemy->ChangeState(m_enemy->GetEnemyChasing());
	  //}
	  //else
	  //{
		 ////�G�m�F�ӏ�
		 //m_enemy->ChangeState(m_enemy->GetEnemyWanderingReturn());
	  //}
	  //m_isFirstExploration = true;	
	  //m_serchTime = 0.0f;
   //}
}
