/*
	@file	FellowRouteSearch.cpp
	@brief	仲間の経路探索状態クラス
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
/// コンストラクタ
/// </summary>
/// <param name="fellow">仲間のポインタ</param>
/// <param name="map">マップ情報</param>
/// <param name="stageNumber">ステージ番号</param>
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
/// デストラクタ
/// </summary>
FellowRouteSearch::~FellowRouteSearch()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void FellowRouteSearch::Initialize()
{
   //ラインオブサイトを生成
   m_lineOfSite = std::make_unique<LineOfSite>();

   //グラフを取得する
   if (GameManager::GetInstance())
   {
	  m_graph = GameManager::GetInstance()->GetGraph();
   }
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void FellowRouteSearch::Update(const float& elapsedTime)
{
   //探索時間を計測する
   m_searchTime += elapsedTime;
   //探索時間が超過したらアイドル状態に戻す
   if (m_searchTime >= TIME_TO_SEARCH)
   {
	  m_fellow->ChangeState(m_fellow->GetFellowIdle());
   }
}

/// <summary>
/// 描画する
/// </summary>
void FellowRouteSearch::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// 後始末する
/// </summary>
void FellowRouteSearch::Finalize()
{
   m_lineOfSite.reset();
}

/// <summary>
/// AStarの準備をする
/// </summary>
/// <param name="target">ターゲットの座標</param>
void FellowRouteSearch::SetAstar(DirectX::SimpleMath::Vector3 target)
{
   m_targetPosition = target;

   //スタート・ゴールセルを取得
   int startStage = m_fellow->GetObjectStageNumber();
   int goalStage = m_tagetStageNumber;
   CellVertex* startCell = m_graph->GetVertexFromPosition(startStage, m_fellow->GetPosition());
   CellVertex* goalCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   if (!startCell || !goalCell) return;

   m_targetCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   //ステージ間A*で経路探索
   auto path = m_fellow->GetMultiStageAStar()->Search(
	  startStage, startCell->GetVertexNumber(),
	  goalStage, goalCell->GetVertexNumber()
   );

   if (path.empty()) return;
   
   auto smoothedPath = m_lineOfSite->SmoothMultiStagePathWithLOS(path, m_graph, m_map);

   //経路をセット
   m_fellow->SetMultiStagePath(smoothedPath);
   m_fellow->SetWayPointIndex(0);
   m_fellow->ChangeState(m_fellow->GetFellowChasing());
   m_isFirstExploration = true;
}
