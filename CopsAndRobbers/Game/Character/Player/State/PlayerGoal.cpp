/*
	@file	PlayerGoal.cpp
	@brief	プレイヤーのゴール状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerGoal.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player"></param>
PlayerGoal::PlayerGoal(Player* player)
	:
	m_player( player )
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerGoal::~PlayerGoal()
{
   //後始末をする
   Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void PlayerGoal::Initialize()
{
   //初期値の設定
   m_openTime = OPEN_TIME;
   m_endStateTime = END_STATE_TIME;
}

/// <summary>
///  更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerGoal::Update(const float& elapsedTime)
{
    //時間の更新
    m_openTime -= elapsedTime;
	m_endStateTime -= elapsedTime;
	//移動処理
   if (m_openTime <= 0)
   {
	  Move(GOAL_MOVE_POSITION, elapsedTime);
   }

   if (m_endStateTime <= 0)
   {
	  m_player->ChangeState(m_player->GetPlayerIdle());
	  m_player->SetEndGaolEvent(true);
   }
   //移動ベクトルを初期化する
   m_playerVelocity = DirectX::SimpleMath::Vector3::Zero;
}


/// <summary>
/// 描画する
/// </summary>
void PlayerGoal::Render()
{
}

/// <summary>
/// 後始末する
/// </summary>
void PlayerGoal::Finalize()
{
}

/// <summary>
/// 移動方向からラジアン角度に変換
/// </summary>
/// <param name="vec">移動方向</param>
/// <returns></returns>
float PlayerGoal::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
    //移動の距離が0なら角度を0で返す
	if (vec.Length() <= 0) return 0.0f;

	//移動方向から角度を求める
	float angle = atan2(vec.x, vec.z);
	return angle;
}

/// <summary>
/// 受け取った地点に移動をする
/// </summary>
/// <param name="target">ターゲット座標</param>
/// <param name="elasedTime">経過時間</param>
void PlayerGoal::Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime)
{
   // ターゲットまでの方向ベクトルを計算する
   DirectX::SimpleMath::Vector3 direction = target - m_player->GetPosition();
   // 一定の距離に近づくと止まる
   direction.Normalize();
   //速度を計算する
   DirectX::SimpleMath::Vector3 Velocity = direction * Player::MOVE_SPEED * elasedTime;
   // 座標を更新する
   m_player->SetPosition(m_player->GetPosition() + Velocity);

   // 進行方向に向ける
   if (direction.LengthSquared() > 0.0f)  // 方向ベクトルがゼロでない場合
   {
	  // 新しい方向を計算する
	  m_player->SetAngle(atan2(direction.x, direction.z));
	  m_player->SetMoveAngle(0.0f);

   }
}
