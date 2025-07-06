/*
	@file	EnemyWanderingReturn.cpp
	@brief	敵の徘徊ルートに戻るクラス
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyWanderingReturn.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
EnemyWanderingReturn::EnemyWanderingReturn(Enemy* enemy)
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
EnemyWanderingReturn::~EnemyWanderingReturn()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void  EnemyWanderingReturn::Initialize()
{	
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void  EnemyWanderingReturn::Update(const float& elapsedTime)
{
   //ターゲットを追跡する
   Move(elapsedTime);
}


/// <summary>
/// 描画する
/// </summary>
void  EnemyWanderingReturn::Render()
{
}


/// <summary>
/// 後始末する
/// </summary>
void EnemyWanderingReturn::Finalize()
{
	// do nothing.
}


/// <summary>
/// 指定した目標座標まで移動
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyWanderingReturn::Move(const float& elapsedTime)
{
   // 頂点リストを取得する
   const std::vector<CellVertex>& vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(m_enemy->GetObjectStageNumber());

   // ウェイポイントインデックスが最短経路配列のサイズより小さい場合
   if (m_enemy->GetWayPointIndex() < (int)m_enemy->GetShortestPath().size() - 1)
   {
	  m_isArrived = false;

	  DirectX::SimpleMath::Vector3 currentPosition = m_enemy->GetPosition();
	  DirectX::SimpleMath::Vector3 nextPosition = vertexList[m_enemy->GetShortestPath()[m_enemy->GetWayPointIndex() + 1]].GetPosition();

	  // 次の頂点への方向を求める
	  DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  // 距離を計算
	  float distanceToNext = direction.Length();

	  // 距離が0でない場合は正規化（方向ベクトルを単位ベクトルにする）
	  if (distanceToNext > 0.0f)
	  {
		 direction.Normalize();
	  }

	  // 進むべき速度ベクトルを計算
	  DirectX::SimpleMath::Vector3 velocity = direction * EnemyWanderingReturn::SLOW_MOVE_SPEED * elapsedTime;

	  // 実際に進む距離を計算（次の頂点に近づきすぎないようにする）
	  if (velocity.Length() >= distanceToNext)
	  {
		 // 次の頂点を通過または到達する場合
		 m_enemy->SetPosition(nextPosition); // 次の頂点に直接移動
		 m_enemy->SetWayPointIndex(m_enemy->GetWayPointIndex() + 1); // ウェイポイントを更新
	  }
	  else
	  {
		 // 通常の移動
		 m_enemy->SetPosition(currentPosition + velocity);
	  }

	  // 進行方向に応じて回転を設定
	  m_enemy->SetAngle(atan2(direction.x, direction.z));
   }
   else
   {
	  //敵確認箇所
	  m_isArrived = true;
	  m_enemy->ChangeState(m_enemy->GetEnemyWandering());
   }
}
