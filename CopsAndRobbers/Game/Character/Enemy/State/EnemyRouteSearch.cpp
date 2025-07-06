/*
	@file	EnemyRouteSearch.cpp
	@brief	敵の経路探索状態クラス
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
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
/// <param name="map">マップ情報</param>
/// <param name="stageNumber">ステージ番号</param>
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
///  デストラクタ
/// </summary>
EnemyRouteSearch::~EnemyRouteSearch()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void EnemyRouteSearch::Initialize()
{
   //ラインオブサイトを生成
   m_lineOfSite = std::make_unique<LineOfSite>();
   if (GameManager::GetInstance())
   {
	  m_graph = GameManager::GetInstance()->GetGraph();
   }
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyRouteSearch::Update(const float& elapsedTime)
{
   //時間の更新
   m_searchTime += elapsedTime;

   //探索時間を超えたら探索を終える
   if (m_searchTime >= 4.0f)
   {
	  //探索に時間がかかる場合、巡回経路に戻る
	  m_enemy->GetEnemyRouteSearch()->SetAstar(m_enemy->GetEnemyWandering()->GetWanderTarget());
	  m_isFirstExploration = false;
	  m_searchTime = 0.0f;
   }
}


/// <summary>
/// 描画する
/// </summary>
void EnemyRouteSearch::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}

/// <summary>
/// 後始末する
/// </summary>
void EnemyRouteSearch::Finalize()
{
	// do nothing.
}


/// <summary>
/// AStarの準備をする
/// </summary>
/// <param name="target">追跡する目標の座標</param>
void EnemyRouteSearch::SetAstar(DirectX::SimpleMath::Vector3 target)
{
   m_targetPosition = target;

   //スタート・ゴールセルを取得
   int startStage = m_enemy->GetObjectStageNumber();
   int goalStage = m_tagetStageNumber;
   CellVertex* startCell = m_graph->GetVertexFromPosition(startStage, m_enemy->GetPosition());
   CellVertex* goalCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   //スタート・ゴールセルがなければ処理をしない
   if (!startCell || !goalCell) return;

   m_targetCell = m_graph->GetVertexFromPosition(goalStage, m_targetPosition);

   //ステージ間A*で経路探索
   auto path = m_enemy->GetMultiStageAStar()->Search(
	  startStage, startCell->GetVertexNumber(),
	  goalStage, goalCell->GetVertexNumber()
   );

   if (path.empty()) return;

   auto smoothedPath = m_lineOfSite->SmoothMultiStagePathWithLOS(path, m_graph, m_map);

   //視界に入っている場合、追跡状態、入っていない場合経路に戻る状態に変更
   if (m_enemy->InView(false))
   {
	  //経路をセット
	  m_enemy->SetMultiStagePath(smoothedPath);
	  m_enemy->SetWayPointIndex(0);
	  m_enemy->ChangeState(m_enemy->GetEnemyChasing());
	  m_isFirstExploration = true;
   }
   else
   {
	  //敵確認箇所
	  m_enemy->ChangeState(m_enemy->GetEnemyWanderingReturn());
   }



   ////ターゲットがグリッドセル範囲内か確認
   //if (m_graph->GetVertexFromPosition(m_stageNumber, m_targetPosition) == nullptr) return;
   //m_targetCell = m_graph->GetVertexFromPosition(m_stageNumber, m_targetPosition);

   ////A*アルゴリズムの探索を行う
   //if (m_enemy->GetAStar()->Search(
	  //m_graph->GetVertexFromPosition(m_stageNumber, m_enemy->GetPosition())->GetVertexNumber(),
	  //m_graph->GetVertexFromPosition(m_stageNumber, m_targetPosition)->GetVertexNumber()))
   //{
	  ////ラインオブサイトを使って最短経路を最適化
	  //auto path = m_lineOfSite->SmoothPathWithLOS(
		 //m_enemy->GetAStar()->GetShortestPath(), 
		 //m_graph->GetVertexList(m_enemy->GetObjectStageNumber()), 
		 //m_map->GetBoundingBoxes());
	  //
	  //m_enemy->SetShortestPath(path);
	  ////ウェイポイントインデックスをクリアする
	  //m_enemy->SetWayPointIndex(0);
	  ////視界に入っている場合、追跡状態、入っていない場合経路に戻る状態に変更
	  //if (m_enemy->InView(false))
	  //{
		 //m_enemy->ChangeState(m_enemy->GetEnemyChasing());
	  //}
	  //else
	  //{
		 ////敵確認箇所
		 //m_enemy->ChangeState(m_enemy->GetEnemyWanderingReturn());
	  //}
	  //m_isFirstExploration = true;	
	  //m_serchTime = 0.0f;
   //}
}
