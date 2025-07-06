/*
	@file	PlayerMove.cpp
	@brief	プレイヤーの移動状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerMove.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Astar/AStarGraph.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="player">プレイヤー本体のポインタ</param>
PlayerMove::PlayerMove(Player* player)
	:
	m_player( player ),
	m_playerMoveMatrix( DirectX::SimpleMath::Matrix::Identity )
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerMove::~PlayerMove()
{
    //後始末をする
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void PlayerMove::Initialize()
{
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerMove::Update(const float& elapsedTime)
{
	//移動処理
	Move(elapsedTime);

	//止まっている場合、状態を変更
	if (m_velocity.Length() <= 0.0f)
	{
		m_player->ChangeState(m_player->GetPlayerIdle());
	}
	//移動ベクトルを初期化する
	m_velocity = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// 描画する
/// </summary>
void PlayerMove::Render()
{
}

/// <summary>
/// 後始末する
/// </summary>
void PlayerMove::Finalize()
{
}

/// <summary>
/// 移動方向からラジアン角度に変換
/// </summary>
/// <param name="vec">移動方向</param>
/// <returns></returns>
float PlayerMove::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
    //移動の距離が0なら角度を0で返す
	if (vec.Length() <= 0) return 0.0f;

	//移動方向から角度を求める
	float angle = atan2(vec.x, vec.z);
	return angle;
}


/// <summary>
/// 移動処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayerMove::Move(const float& elapsedTime)
{
   // 移動中なら正規化をして座標に加算
   if (m_velocity.LengthSquared() > 0.0f)
   {
	  //移動量を正規化とスケーリング
	  m_velocity.Normalize();
	  m_velocity *= Player::MOVE_SPEED * elapsedTime;

	  // カメラの向きに合わせて移動ベクトルを回転
	  m_player->SetMoveAngle(GetAngleFromVector(m_velocity));
	  m_playerMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_player->GetCameraAngle()); 
	  m_velocity = DirectX::SimpleMath::Vector3::Transform(m_velocity, m_playerMatrix);

	  //床の判定と移動量の調整
	  IsFloor();

	  m_player->SetPosition(m_player->GetPosition() + m_finalVelocity);
   }
}

/// <summary>
/// 床の判定と移動量の調整
/// </summary>
void PlayerMove::IsFloor()
{
   auto* graph = GameManager::GetInstance()->GetGraph();
   //移動先の座標(x,z軸ごと）
   DirectX::SimpleMath::Vector3 nextMoveX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_velocity.x, 0.0f, 0.0f);
   DirectX::SimpleMath::Vector3 nextMoveZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_velocity.z);
   // x軸方向の判定
   DirectX::SimpleMath::Vector3 nextX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_velocity.x, 0.0f, 0.0f);
   bool floorX = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextX);

   // z軸方向の判定
   DirectX::SimpleMath::Vector3 nextZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_velocity.z);
   bool floorZ = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextZ);

   // 移動ベクトルを調整
   m_finalVelocity = DirectX::SimpleMath::Vector3::Zero;
   if (floorX) m_finalVelocity.x = m_velocity.x;
   if (floorZ) m_finalVelocity.z = m_velocity.z;
}
