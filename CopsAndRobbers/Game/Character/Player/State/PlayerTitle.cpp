/*
	@file	PlayerTitle.cpp
	@brief	プレイヤーのタイトル状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerTitle.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">プレイヤー本体のポインタ</param>
PlayerTitle::PlayerTitle(Player* player)
	:
	m_player( player )
{
}


/// <summary>
/// デストラクタ
/// </summary>
PlayerTitle::~PlayerTitle()
{
   //後始末をする
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void PlayerTitle::Initialize()
{
   //目標地点の設定
   m_targetPoint1 = TARGET_POINT;
   m_targetPoint2 = m_player->GetInitialPosition();
   //初期ターゲット地点の設定
   m_currentTargetPoint = m_targetPoint1;
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerTitle::Update(const float& elapsedTime)
{
   //移動処理
   Move(m_currentTargetPoint, elapsedTime);

   //目標地点に到達したか
   if (IsTargetPoint())
   {
	  //目標地点を切り替える
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
   m_playerVelocity = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// 描画する
/// </summary>
void PlayerTitle::Render()
{
}

/// <summary>
/// 後始末する
/// </summary>
void PlayerTitle::Finalize()
{
}

/// <summary>
/// 移動方向からラジアン角度に変換
/// </summary>
/// <param name="vec">移動方向</param>
/// <returns></returns>
float PlayerTitle::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
    //移動の距離が0なら角度を0で返す
	if (vec.Length() <= 0) return 0.0f;

	//移動方向から角度を求める
	float angle = atan2(vec.x, vec.z);
	return angle;
}


/// <summary>
/// 目標地点に到達したかどうか
/// </summary>
/// <returns></returns>
bool PlayerTitle::IsTargetPoint()
{
   //目標値との距離を絶対値で求める
   float pos = std::abs(m_player->GetPosition().x - m_currentTargetPoint.x);
   //目標値との距離が近くなったら到達したとする
   if (pos <= 1.0f)
   {
	  return true;
   }
   return false;
}

/// <summary>
/// 受け取った地点に移動をする
/// </summary>
/// <param name="target">ターゲット座標</param>
/// <param name="elasedTime">経過時間</param>
void PlayerTitle::Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime)
{
   // ターゲットまでの方向ベクトルを計算する
   DirectX::SimpleMath::Vector3 initialPos = m_player->GetInitialPosition();
   DirectX::SimpleMath::Vector3 player = m_player->GetPosition();
   DirectX::SimpleMath::Vector3 direction = target - m_player->GetPosition();
   // 一定の距離に近づくと止まる
   direction.Normalize();
   //速度を計算する
   DirectX::SimpleMath::Vector3 Velocity = direction * Player::MOVE_SPEED * SPEED_CORRECTION * elasedTime;
   // 座標を更新する
   m_player->SetPosition(m_player->GetPosition() + Velocity);

   // 進行方向に向ける
   if (direction.LengthSquared() > 0.0f)  // 方向ベクトルがゼロでない場合
   {
	  // 新しい方向を計算する
	  m_player->SetAngle(atan2(direction.x, direction.z) + m_player->GetInitialAngle());
	  m_player->SetMoveAngle(0.0f);
   }
}
