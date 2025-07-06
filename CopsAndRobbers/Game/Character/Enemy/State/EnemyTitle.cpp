/*
	@file	EnemyTitle.cpp
	@brief	敵のタイトル状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Enemy/State/EnemyTitle.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
EnemyTitle::EnemyTitle(Enemy* enemy)
	:
    m_enemy( enemy ),
    m_targetPoint1(TARGET_POINT1),
    m_targetPoint2(TARGET_POINT2)
{
    //グラフィックスのインスタンスを取得
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyTitle::~EnemyTitle()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void EnemyTitle::Initialize()
{
   //初期ターゲット地点の設定
   m_currentTargetPoint = m_targetPoint1;
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyTitle::Update(const float& elapsedTime)
{
	//移動処理
   Move(m_currentTargetPoint, elapsedTime);

   //目標地点に達していたら目標地点を切り替える
   if (IsTargetPoint())
   {
	  if (m_currentTargetPoint != m_targetPoint1)
	  {
		 m_currentTargetPoint = m_targetPoint1;
	  }
	  else
	  {
		 m_currentTargetPoint = m_targetPoint2;
	  }
   }

   //移動ベクトルを初期化する
   m_velocity = DirectX::SimpleMath::Vector3::Zero;
}


/// <summary>
/// 描画する
/// </summary>
void EnemyTitle::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// 後始末する
/// </summary>
void EnemyTitle::Finalize()
{
}


/// <summary>
/// 移動方向からラジアン角度に変換
/// </summary>
/// <param name="vec">移動方向</param>
/// <returns></returns>
float EnemyTitle::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
	if (vec.Length() <= 0) return 0.0f;

	//XZ平面で角度を求める
	float angle = atan2(vec.x, -vec.z);
	return angle;
}

/// <summary>
/// 目標地点に達しているかどうか
/// </summary>
/// <returns></returns>
bool EnemyTitle::IsTargetPoint()
{
   float pos =std::abs(m_enemy->GetPosition().x - m_currentTargetPoint.x);
   if (pos <= 1.0f)
   {
	  return true;
   }
   return false;
}


/// <summary>
/// 受け取った地点に移動をする
/// </summary>
/// <param name="target">移動目標の座標</param>
/// <param name="elasedTime">経過時間</param>
void EnemyTitle::Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime)
{
   // ターゲットまでの方向ベクトルを計算する
   DirectX::SimpleMath::Vector3 initialPos = m_enemy->GetInitialPosition();
   DirectX::SimpleMath::Vector3 player = m_enemy->GetPosition();
   DirectX::SimpleMath::Vector3 direction = target - m_enemy->GetPosition();
   // 一定の距離に近づくと止まる
   direction.Normalize();
   //速度を計算する
   DirectX::SimpleMath::Vector3 Velocity = direction * EnemyTitle::TITLE_MOVE_SPEED * elasedTime;
   // 座標を更新する
   m_enemy->SetPosition(m_enemy->GetPosition() + Velocity);

   // 進行方向に向ける
   if (direction.LengthSquared() > 0.0f)  // 方向ベクトルがゼロでない場合
   {
	  // 新しい方向を計算する
	  m_enemy->SetAngle(atan2(direction.x, direction.z) + m_enemy->GetInitialAngle());
	  m_enemy->SetMoveAngle(0.0f);
   }
}
