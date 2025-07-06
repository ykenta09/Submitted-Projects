/*
	@file	FellowRouteSearch.cpp
	@brief	���Ԃ̌o�H�T����ԃN���X
*/
#include "pch.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Fellow/State/FellowRouteSearch.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"
#include "Game/GameManager/GameManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fellow">���Ԃ̃|�C���^</param>
/// <param name="map">�}�b�v���</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
FellowRouteSearch::FellowRouteSearch(Fellow* fellow, Map* map, const int& stageNumber)
	:
	m_fellow(fellow),
    m_map(map),
    m_stageNumber(stageNumber),
    m_tagetStageNumber(0),
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_gridSize(0),
    m_isFirstExploration(false),
    m_searchTime(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FellowRouteSearch::~FellowRouteSearch()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void FellowRouteSearch::Initialize()
{
   //���C���I�u�T�C�g�𐶐�
   m_lineOfSite = std::make_unique<LineOfSite>();

   //�O���t���擾����
   if (GameManager::GetInstance())
   {
	  m_graph = GameManager::GetInstance()->GetGraph();
   }
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void FellowRouteSearch::Update(const float& elapsedTime)
{
   //�T�����Ԃ��v������
   m_searchTime += elapsedTime;
   //�T�����Ԃ����߂�����A�C�h����Ԃɖ߂�
   if (m_searchTime >= TIME_TO_SEARCH)
   {
	  m_fellow->ChangeState(m_fellow->GetFellowIdle());
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void FellowRouteSearch::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// ��n������
/// </summary>
void FellowRouteSearch::Finalize()
{
   m_lineOfSite.reset();
}

/// <summary>
/// AStar�̏���������
/// </summary>
/// <param name="target">�^�[�Q�b�g�̍��W</param>
void FellowRouteSearch::SetAstar(DirectX::SimpleMath::Vector3 target)
{
   m_targetPosition = target;

   //�X�^�[�g�E�S�[���Z�����擾
   int startStage = m_fellow->GetObjectStageNumber();
   int goalStage = m_tagetStageNumber;
   CellVertex* startCell = m_graph->GetVertexFromPosition(startStage, m_fellow->GetPosition());
   CellVertex* goalCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   if (!startCell || !goalCell) return;

   m_targetCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   //�X�e�[�W��A*�Ōo�H�T��
   auto path = m_fellow->GetMultiStageAStar()->Search(
	  startStage, startCell->GetVertexNumber(),
	  goalStage, goalCell->GetVertexNumber()
   );

   if (path.empty()) return;
   
   auto smoothedPath = m_lineOfSite->SmoothMultiStagePathWithLOS(path, m_graph, m_map);

   //�o�H���Z�b�g
   m_fellow->SetMultiStagePath(smoothedPath);
   m_fellow->SetWayPointIndex(0);
   m_fellow->ChangeState(m_fellow->GetFellowChasing());
   m_isFirstExploration = true;
}
