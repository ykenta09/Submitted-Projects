/*
	@file	EnemyWaiting.cpp
	@brief	敵の待機クラス
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyWaiting.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"
#include "Libraries/yamadaLib/Math.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
EnemyWaiting::EnemyWaiting(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_waitTime (0.0f),
    m_isRotationComplete(false)
{
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyWaiting::~EnemyWaiting()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void EnemyWaiting::Initialize()
{	
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyWaiting::Update(const float& elapsedTime)
{
   //次の移動先の方向に回転する
   RotateToNextDestination(elapsedTime);
}


/// <summary>
/// 描画する
/// </summary>
void EnemyWaiting::Render()
{

#ifdef _DEBUG

#endif // _DEBUG

}


/// <summary>
/// 後始末する
/// </summary>
void EnemyWaiting::Finalize()
{
	// do nothing.
}

/// <summary>
/// 次の移動先方向に回転
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyWaiting::RotateToNextDestination(const float elapsedTime)
{
   m_waitTime += elapsedTime;

   // 目標角度の計算
   const float rotationSpeed = DirectX::XMConvertToRadians(60.0f);
   // 現在の角度から次の移動先の方向
   float angleDiff = m_enemy->GetNextAngle() - m_enemy->GetAngle();

   // 最短距離で回転するために、角度差を正規化する
   if (std::abs(angleDiff) > DirectX::XM_PI)
   {
      if (angleDiff > 0)
      {
         angleDiff -= 2 * DirectX::XM_PI;
      }
      else
      {
         angleDiff += 2 * DirectX::XM_PI;
      }
   }

   //待機時間が終了したら回転を開始
   if (m_waitTime >= Enemy::WAIT_INTERVAL)
   {
      // 角度差に基づいて回転
      m_enemy->SetAngle(yamadalib::Math::Lerp(m_enemy->GetAngle(), m_enemy->GetAngle() + angleDiff, rotationSpeed * elapsedTime));

      // 回転が完了したか確認
      if (std::abs(angleDiff) <= DirectX::XMConvertToRadians(1.0f))
      {
         m_isRotationComplete = true;
      }
      else
      {
         m_isRotationComplete = false;
      }

      if (m_isRotationComplete)
      {
         m_waitTime = 0.0f;
         m_enemy->ChangeState(m_enemy->GetEnemyWandering());
      }
   }
}