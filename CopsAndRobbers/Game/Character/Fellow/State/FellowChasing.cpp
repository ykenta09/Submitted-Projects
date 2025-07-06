/*
	@file	FellowChasing.cpp
	@brief	仲間の追跡クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <Libraries/yamadaLib/Resources.h>

#include "Game/Screen.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Fellow/State/FellowChasing.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fellow">本体のポインタ</param>
FellowChasing::FellowChasing(Fellow* fellow)
	:
	m_fellow(fellow),
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_isArrived(true),
    m_isGoal(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
FellowChasing::~FellowChasing()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void FellowChasing::Initialize()
{
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void FellowChasing::Update(const float& elapsedTime)
{
   //再探索が必要かどうか
   Search();

   if (!m_isGoal)
   {
	  m_targetPosition = m_fellow->GetTargetPosition();
	  //ターゲットを追跡する
	  Move(elapsedTime);
   }
   //目的地に達したらアイドル状態に変更
   if (m_isArrived)
   {
	  m_fellow->ChangeState(m_fellow->GetFellowIdle());
   }
}


/// <summary>
/// 描画する
/// </summary>
void FellowChasing::Render()
{

#ifdef _DEBUG
#endif // _DEBUG
}

/// <summary>
/// 後始末する
/// </summary>
void FellowChasing::Finalize()
{
	// do nothing.
}

/// <summary>
/// 受け取った地点に移動をする
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void FellowChasing::Move(const float& elapsedTime)
{
   //パスを取得する
   const auto& path = m_fellow->GetMultiStagePath();
   int wayPoint = m_fellow->GetWayPointIndex();

   if (m_fellow->GetWayPointIndex() < (int)path.size() - 1)
   {
	  m_isArrived = false;
	  //現在のステージ・ノード
	  int nextStage = path[wayPoint + 1].stage;
	  int nextNode = path[wayPoint+ 1].node;

	  //頂点リストからCellVertexを取得する
	  const auto& nextVertexList = m_fellow->GetFellowRouteSearch()->GetGrid()->GetVertexList(nextStage);

	  //範囲チェック
	  if (nextNode < 0 || nextNode >= nextVertexList.size()) return;

	  //グラフから座標を取得する
	  const CellVertex& nextVertex = nextVertexList[nextNode];
	  DirectX::SimpleMath::Vector3 currentPosition = m_fellow->GetPosition();
	  DirectX::SimpleMath::Vector3 nextPosition = nextVertex.GetPosition();

	  //ターゲットへの方向と距離を計算
	  DirectX::SimpleMath::Vector3 targetDirection = m_targetPosition - currentPosition;
	  float distanceToTarget = targetDirection.Length();

	  //減速処理パラメータの設定
	  const float decelerationStartDistance = Fellow::STOPPING_DISTANCE * 3.0f;

	  //距離に基づいた速度計算
	  float speed = Fellow::MOVE_SPEED;
	  //ターゲットの距離が減速のかかる距離範囲なら減速処理をする
	  if (distanceToTarget < Fellow::STOPPING_DISTANCE)
	  {
		 float t = (distanceToTarget - Fellow::STOPPING_DISTANCE) / (decelerationStartDistance - Fellow::STOPPING_DISTANCE);
		 speed *= std::max(t, 0.0f); // 0未満にならないように	  
	  }

	  //停止距離以内なら移動しない
	  if (distanceToTarget <= Fellow::STOPPING_DISTANCE) return;

	  //次のノード方向
	  DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  float distanceToNext = direction.Length();
	  direction.Normalize();
	  DirectX::SimpleMath::Vector3 velocity = direction * Fellow::MOVE_SPEED * elapsedTime;

	  if (distanceToNext < velocity.Length())
	  {
		 m_fellow->SetPosition(nextPosition);
		 m_fellow->SetWayPointIndex(m_fellow->GetWayPointIndex() + 1);
	  }
	  else
	  {
		 m_fellow->SetPosition(currentPosition + velocity);
	  }

	  // 進行方向に応じて回転を設定
	  m_fellow->SetAngle(atan2(direction.x, direction.z));
   }
   else 
   {
	  m_isArrived = true;
   }
}


/// <summary>
/// 受け取った地点に移動をする
/// </summary>
/// <param name="target">移動先の座標</param>
/// <param name="elapsedTime">経過時間</param>
void FellowChasing::TargetMove(const DirectX::SimpleMath::Vector3& target, const float& elapsedTime)
{
   // 敵の現在の座標を計算
   DirectX::SimpleMath::Vector3 position = m_fellow->GetPosition();
   // ターゲットまでの方向ベクトルを計算する
   DirectX::SimpleMath::Vector3 direction = target - position;
   // 一定の距離に近づくと止まる
   if (direction.Length() <= Fellow::STOPPING_DISTANCE) return;
   direction.Normalize();
   // 敵の速度を計算する
   DirectX::SimpleMath::Vector3 enemyVelocity = direction * Fellow::MOVE_SPEED * elapsedTime;
   // 敵の座標を更新する
   m_fellow->SetPosition(m_fellow->GetPosition() + enemyVelocity);

   // 敵の進行方向に向ける
   if (direction.LengthSquared() > 0.0f)  // 方向ベクトルがゼロでない場合
   {
	  // 新しい方向を計算する
	  m_fellow->SetAngle(atan2(direction.x, direction.z));
   }
}


/// <summary>
/// 索敵
/// </summary>
void FellowChasing::Search()
{
   if (m_fellow->GetFellowRouteSearch()->GetGrid()->GetVertexFromPosition(m_fellow->GetFellowRouteSearch()->GetTargetStageNumber(), m_targetPosition) == nullptr) return;
   //ターゲットの座標が変わったとき再探索
   auto cell = m_fellow->GetFellowRouteSearch()->GetTargetCell();
   auto vertexNumber = cell->GetVertexNumber();

   if (vertexNumber != m_fellow->GetFellowRouteSearch()->GetGrid()->GetVertexFromPosition(m_fellow->GetFellowRouteSearch()->GetTargetStageNumber(), m_targetPosition)->GetVertexNumber())
   {
	  m_fellow->ChangeState(m_fellow->GetFellowRouteSearch());
	  m_fellow->GetFellowRouteSearch()->SetSearchTime(0);
	  m_fellow->GetFellowRouteSearch()->SetAstar(m_targetPosition);
   }
}
