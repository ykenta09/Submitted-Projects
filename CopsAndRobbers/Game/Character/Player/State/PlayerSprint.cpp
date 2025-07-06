/*
	@file	PlayerSprint.cpp
	@brief	プレイヤーの高速移動状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerSprint.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Astar/AStarGraph.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">プレイヤー本体のポインタ</param>
PlayerSprint::PlayerSprint(Player* player)
	:
	m_player( player ),
    m_sprintStartTime(0.0f),
	m_moveAngle( 0.0f ),
    m_sprintVelocity(DirectX::SimpleMath::Vector3::Zero),
	m_isSprint(false)
{
}


/// <summary>
/// デストラクタ
/// </summary>
PlayerSprint::~PlayerSprint()
{
   //後始末をする
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void PlayerSprint::Initialize()
{
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerSprint::Update(const float& elapsedTime)
{	
    //高速移動中かどうか
	if (m_isSprint)
	{
	    //高速移動の処理
		Sprint(elapsedTime);
	}
	else
	{
	    //アイドル状態に戻す
		m_player->ChangeState(m_player->GetPlayerIdle());
	}
}

/// <summary>
/// 描画する
/// </summary>
void PlayerSprint::Render()
{
}

/// <summary>
/// 後始末する
/// </summary>
void PlayerSprint::Finalize()
{
}

/// <summary>
/// 高速移動
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerSprint::Sprint(const float& elapsedTime)
{
    //カメラの向ている方向から回転行列を作成
	m_playerMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_player->GetCameraAngle());

	// 回避行動の時間を更新
	m_sprintStartTime += elapsedTime;

	if (m_sprintStartTime < ACCELERATION_DURATION)
	{
	   // 加速中：加速値を計算（線形加速）
	   float acceleration = MAX_MOVEMENT_SPEED * (m_sprintStartTime / ACCELERATION_DURATION);
	   m_sprintVelocity = DirectX::SimpleMath::Vector3::Transform(m_sprintDirection * acceleration, m_playerMatrix);
	   m_sprintVelocity *= elapsedTime;
	   IsFloor();
	   m_player->SetPosition(m_player->GetPosition() + m_finalVelocity);
	}
	else
	{
	   // 一定速度で移動
	   m_sprintVelocity = DirectX::SimpleMath::Vector3::Transform(m_sprintDirection * MAX_MOVEMENT_SPEED, m_playerMatrix);
	   m_sprintVelocity *= elapsedTime;
	   IsFloor();
	   m_player->SetPosition(m_player->GetPosition() + m_finalVelocity);

	   // 転がりが終了したらフラグをリセット
	   if (m_sprintStartTime > MOVEMENT_DURATION)
	   {
		  m_isSprint = false;
		  m_sprintStartTime = 0.0f;
	   }
	}
}

/// <summary>
/// 移動方向からラジアン角度に変換
/// </summary>
/// <param name="vec">移動方向</param>
/// <returns></returns>
float PlayerSprint::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
   //移動の距離が0なら角度を0で返す
   if (vec.Length() <= 0) return 0.0f;

   //移動方向から角度を求める
   float angle = atan2(vec.x, vec.z);
   return angle;
}

/// <summary>
/// 高速移動の準備をする
/// </summary>
/// <param name="dir">高速移動で移動する方向</param>
void PlayerSprint::PreSprint(const DirectX::SimpleMath::Vector3& dir)
{
	m_isSprint = true;
	m_sprintStartTime = 0.0f;
	m_sprintDirection = dir;
	m_sprintDirection.Normalize();
}


/// <summary>
/// 床の判定と移動量の調整
/// </summary>
void PlayerSprint::IsFloor()
{
   auto* graph = GameManager::GetInstance()->GetGraph();
   //移動先の座標(x,z軸ごと）
   DirectX::SimpleMath::Vector3 nextMoveX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_sprintVelocity.x, 0.0f, 0.0f);
   DirectX::SimpleMath::Vector3 nextMoveZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_sprintVelocity.z);
   // x軸方向の判定
   DirectX::SimpleMath::Vector3 nextX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_sprintVelocity.x, 0.0f, 0.0f);
   bool floorX = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextX);

   // z軸方向の判定
   DirectX::SimpleMath::Vector3 nextZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_sprintVelocity.z);
   bool floorZ = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextZ);

   // 移動ベクトルを調整
   m_finalVelocity = DirectX::SimpleMath::Vector3::Zero;
   if (floorX) m_finalVelocity.x = m_sprintVelocity.x;
   if (floorZ) m_finalVelocity.z = m_sprintVelocity.z;
}
