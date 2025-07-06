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

const float EnemyChasing::LOST_TIME = 5.0f;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
EnemyChasing::EnemyChasing(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_lostTime(0.0f),
    m_isArrived(true)
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyChasing::~EnemyChasing()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void EnemyChasing::Initialize()
{	
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void EnemyChasing::Update(const float& elapsedTime)
{
    //ターゲットを追跡する
    Move(elapsedTime);
	if (!m_enemy->InView(false))
	{
		LostTarget(elapsedTime);
	}
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void EnemyChasing::Render()
{
#ifdef _DEBUG
   // 描画開始
   m_graphics->GetPrimitiveBatch()->Begin();

   std::vector<int> path = m_enemy->GetShortestPath();
   auto vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList();

   //最適化前の最短経路
   if (path.size() > 1) // 頂点が複数ないと描画する線が存在しない
   {
	  for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i)
	  {
		 // path[i] と path[i+1] の間に線を描画する
		 m_graphics->GetPrimitiveBatch()->DrawLine(
			DirectX::VertexPositionColor(vertexList[path[i]].GetPosition(), DirectX::Colors::White),
			DirectX::VertexPositionColor(vertexList[path[i + 1]].GetPosition(), DirectX::Colors::White));
	  }
   }

   //最適化した最短経路
   std::vector<int> shotPath = m_enemy->GetAStar()->GetShortestPath();
   if (shotPath.size() > 1) // 頂点が複数ないと描画する線が存在しない
   {
	  for (int i = 0; i < static_cast<int>(shotPath.size()) - 1; ++i)
	  {
		 // path[i] と path[i+1] の間に線を描画する
		 m_graphics->GetPrimitiveBatch()->DrawLine(
			DirectX::VertexPositionColor(vertexList[shotPath[i]].GetPosition(), DirectX::Colors::Blue),
			DirectX::VertexPositionColor(vertexList[shotPath[i + 1]].GetPosition(), DirectX::Colors::Blue));
	  }
   }

   // 描画終了
   m_graphics->GetPrimitiveBatch()->End();
#endif // _DEBUG
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void EnemyChasing::Finalize()
{
	// do nothing.
}

void EnemyChasing::Move(const float& elapsedTime)
{
   // 頂点リストを取得する
   const std::vector<CellVertex>& vertexList = m_enemy->GetAStar()->GetVertexList();

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
	  DirectX::SimpleMath::Vector3 velocity = direction * Enemy::MOVE_SPEED * elapsedTime;

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
}

//ターゲットを見失った
void EnemyChasing::LostTarget(float elapsedTime)
{
	m_lostTime += elapsedTime;

	if (m_lostTime > EnemyChasing::LOST_TIME)
	{
	   //敵確認箇所
		//状態を変更する		
	    m_enemy->ChangeState(m_enemy->GetEnemyRouteSearch());
		m_lostTime = 0;
		//経路探索を行う
		m_enemy->GetEnemyRouteSearch()->SetAstar(m_enemy->GetEnemyWandering()->GetWanderTarget());
	}
}
