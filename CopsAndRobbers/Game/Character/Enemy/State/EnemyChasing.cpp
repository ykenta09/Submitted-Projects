/*
	@file	EnemyChasing.cpp
	@brief	敵の追跡クラス
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyChasing.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
EnemyChasing::EnemyChasing(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_lostTime(0.0f),
    m_isArrived(true)
{
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyChasing::~EnemyChasing()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void EnemyChasing::Initialize()
{	
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyChasing::Update(const float& elapsedTime)
{
    ////再探索判定をする
    //Search();

    //ターゲットを追跡する
    Move(elapsedTime);
	if (!m_enemy->InView(false))
	{
		LostTarget(elapsedTime);
	}
	if (m_isArrived)
	{
	   m_enemy->ChangeState(m_enemy->GetEnemyWaiting());
	}
}


/// <summary>
/// 描画する
/// </summary>
void EnemyChasing::Render()
{
#ifdef _DEBUG
   //// 描画開始
   //m_graphics->GetPrimitiveBatch()->Begin();

   //std::vector<int> path = m_enemy->GetShortestPath();
   //auto vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(m_enemy->GetObjectStageNumber());

   ////最適化前の最短経路
   //if (path.size() > 1) // 頂点が複数ないと描画する線が存在しない
   //{
	  //for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i)
	  //{
		 //// path[i] と path[i+1] の間に線を描画する
		 //m_graphics->GetPrimitiveBatch()->DrawLine(
			//DirectX::VertexPositionColor(vertexList[path[i]].GetPosition(), DirectX::Colors::White),
			//DirectX::VertexPositionColor(vertexList[path[i + 1]].GetPosition(), DirectX::Colors::White));
	  //}
   //}

   ////最適化した最短経路
   //std::vector<int> shotPath = m_enemy->GetAStar()->GetShortestPath();
   //if (shotPath.size() > 1) // 頂点が複数ないと描画する線が存在しない
   //{
	  //for (int i = 0; i < static_cast<int>(shotPath.size()) - 1; ++i)
	  //{
		 //// path[i] と path[i+1] の間に線を描画する
		 //m_graphics->GetPrimitiveBatch()->DrawLine(
			//DirectX::VertexPositionColor(vertexList[shotPath[i]].GetPosition(), DirectX::Colors::Blue),
			//DirectX::VertexPositionColor(vertexList[shotPath[i + 1]].GetPosition(), DirectX::Colors::Blue));
	  //}
   //}

   //// 描画終了
   //m_graphics->GetPrimitiveBatch()->End();
#endif // _DEBUG
}


/// <summary>
/// 後始末する
/// </summary>
void EnemyChasing::Finalize()
{
	// do nothing.
}


/// <summary>
/// 受け取った目標地点に移動する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyChasing::Move(const float& elapsedTime)
{
   //パスを取得する
   const auto& path = m_enemy->GetMultiStagePath();
   int wayPoint = m_enemy->GetWayPointIndex();

   if (m_enemy->GetWayPointIndex() < (int)path.size() - 1)
   {
	  m_isArrived = false;
	  //現在のステージ・ノード
	  int nextStage = path[wayPoint + 1].stage;
	  int nextNode = path[wayPoint + 1].node;

	  //頂点リストからCellVertexを取得する
	  const auto& nextVertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(nextStage);

	  //範囲チェック
	  if (nextNode < 0 || nextNode >= nextVertexList.size()) return;

	  //グラフから座標を取得する
	  const CellVertex& nextVertex = nextVertexList[nextNode];
	  DirectX::SimpleMath::Vector3 currentPosition = m_enemy->GetPosition();
	  DirectX::SimpleMath::Vector3 nextPosition = nextVertex.GetPosition();

	  //ターゲットへの方向と距離を計算
	  DirectX::SimpleMath::Vector3 targetDirection = m_targetPosition - currentPosition;
	  float distanceToTarget = targetDirection.Length();

	  //減速処理パラメータ
	  const float decelerationStartDistance = Enemy::STOPPING_DISTANCE * 3.0f;
	  //距離に基づいた速度計算
	  float speed = Enemy::MOVE_SPEED;
	  if (distanceToTarget < Enemy::STOPPING_DISTANCE)
	  {
		 float t = (distanceToTarget - Enemy::STOPPING_DISTANCE) / (decelerationStartDistance - Enemy::STOPPING_DISTANCE);
		 speed *= std::max(t, 0.0f); // 0未満にならないように	  
	  }
	  //停止距離以内なら移動しない
	  if (distanceToTarget <= Enemy::STOPPING_DISTANCE) return;

	  //次のノード方向
	  DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  float distanceToNext = direction.Length();
	  direction.Normalize();
	  DirectX::SimpleMath::Vector3 velocity = direction * Enemy::MOVE_SPEED * elapsedTime;

	  if (distanceToNext < velocity.Length())
	  {
		 m_enemy->SetPosition(nextPosition);
		 m_enemy->SetWayPointIndex(m_enemy->GetWayPointIndex() + 1);
	  }
	  else
	  {
		 m_enemy->SetPosition(currentPosition + velocity);
	  }

	  // 進行方向に応じて回転を設定
	  m_enemy->SetAngle(atan2(direction.x, direction.z));
   }
   else
   {
	  m_isArrived = true;
   }

   //// 頂点リストを取得する
   //const std::vector<CellVertex>& vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(m_enemy->GetObjectStageNumber());

   //// ウェイポイントインデックスが最短経路配列のサイズより小さい場合
   //if (m_enemy->GetWayPointIndex() < (int)m_enemy->GetShortestPath().size() - 1)
   //{
	  //m_isArrived = false;

	  //DirectX::SimpleMath::Vector3 currentPosition = m_enemy->GetPosition();
	  //DirectX::SimpleMath::Vector3 nextPosition = vertexList[m_enemy->GetShortestPath()[m_enemy->GetWayPointIndex() + 1]].GetPosition();

	  //// 次の頂点への方向を求める
	  //DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  //// 距離を計算
	  //float distanceToNext = direction.Length();

	  //// 距離が0でない場合は正規化（方向ベクトルを単位ベクトルにする）
	  //if (distanceToNext > 0.0f)
	  //{
		 //direction.Normalize();
	  //}

	  //// 進むべき速度ベクトルを計算
	  //DirectX::SimpleMath::Vector3 velocity = direction * Enemy::MOVE_SPEED * elapsedTime;

	  //// 実際に進む距離を計算（次の頂点に近づきすぎないようにする）
	  //if (velocity.Length() >= distanceToNext)
	  //{
		 //// 次の頂点を通過または到達する場合
		 //m_enemy->SetPosition(nextPosition); // 次の頂点に直接移動
		 //m_enemy->SetWayPointIndex(m_enemy->GetWayPointIndex() + 1); // ウェイポイントを更新
	  //}
	  //else
	  //{
		 //// 通常の移動
		 //m_enemy->SetPosition(currentPosition + velocity);
	  //}

	  //// 進行方向に応じて回転を設定
	  //m_enemy->SetAngle(atan2(direction.x, direction.z));
   //}
}

/// <summary>
/// ターゲットを見失った
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyChasing::LostTarget(float elapsedTime)
{
	m_lostTime += elapsedTime;

	if (m_lostTime > EnemyChasing::LOST_TIME)
	{
		//状態を変更する		
	    m_enemy->ChangeState(m_enemy->GetEnemyRouteSearch());
		m_lostTime = 0;
		//経路探索を行う
		m_enemy->GetEnemyRouteSearch()->SetAstar(m_enemy->GetEnemyWandering()->GetWanderTarget());
	}
}

/// <summary>
/// 索敵
/// </summary>
void EnemyChasing::Search()
{
   if (m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexFromPosition(m_enemy->GetEnemyRouteSearch()->GetTargetStageNumber(), m_targetPosition) == nullptr) return;
   //ターゲットの座標が変わったとき再探索
   auto cell = m_enemy->GetEnemyRouteSearch()->GetTargetCell();
   auto vertexNumber = cell->GetVertexNumber();

   if (vertexNumber != m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexFromPosition(m_enemy->GetEnemyRouteSearch()->GetTargetStageNumber(), m_targetPosition)->GetVertexNumber())
   {
	  m_enemy->ChangeState(m_enemy->GetEnemyRouteSearch());
	  m_enemy->GetEnemyRouteSearch()->SetSearchTime(EnemyRouteSearch::TIME_TO_SEARCH);
	  m_enemy->GetEnemyRouteSearch()->SetAstar(m_targetPosition);
   }
}
