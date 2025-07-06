/*
	@file	MovingFloor.cpp
	@brief	床クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Floor/RotationFloor.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">モデルのタイプ</param>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="movePos">床が移動する先の座標</param>
/// <param name="gimmickID">登録するギミックのID</param>
RotationFloor::RotationFloor(const int& type, const GameParameters::ObjectParameter& param, const int& gimmickID)
	:
    GimmickObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_startPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_floorRotation(param.initialRot),
    m_gimickID(gimmickID),
    m_moveProgress(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
RotationFloor::~RotationFloor()
{
}

/// <summary>
/// 初期化する
/// </summary>
void RotationFloor::Initialize()
{
   // モデルを読み込む
   switch (m_type)
   {
   case 1:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("MovingFloor01"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("MoveFloor01_DDS");
	  break;
   }

   //当たり判定の登録
   InitializeCollision();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void RotationFloor::Update(const float elapsedTime)
{
   //時間の更新
   m_time = elapsedTime;

   //衝突判定の情報更新
   if (m_collisionObject)
   {
	  //ギミックが終了したタイミングで更新
	  if (!m_isFinished) return;
	  m_baseExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, m_floorRotation);
	  m_collisionObject->UpdatePosition(GetPosition(), m_baseExtents);
   }
}

/// <summary>
/// 描画する
/// </summary>
void RotationFloor::Render()
{
   // ワールド行列を更新する
   DirectX::SimpleMath::Matrix matrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_floorRotation) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   // モデルを描画する
   m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), matrix);
}

/// <summary>
/// 後始末する
/// </summary>
void RotationFloor::Finalize()
{
	// do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void RotationFloor::InitializeCollision()
{
   //衝突判定のサイズ
   //const DirectX::SimpleMath::Vector3 baseExtents = GetScale() / 2.0f;
   m_baseExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, m_floorRotation);

   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::Sensor,
	  CollisionManager::CollisionType::HIT,
	  DirectX::BoundingBox(GetPosition(), m_baseExtents));
   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FLOOR, collision };

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}


/// <summary>
/// ギミックアクティブ
/// </summary>
void RotationFloor::Active()
{
   //アニメーションの進行度の更新
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);

   //回転開始地点と終了地点を設定
   DirectX::SimpleMath::Quaternion start = GetRotation();
   DirectX::SimpleMath::Quaternion delta = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, ROTATION_Y);
   DirectX::SimpleMath::Quaternion end = start * delta;
   //回転の間を補間する
   m_floorRotation = DirectX::SimpleMath::Quaternion::Slerp(start, end, m_moveProgress);

   //進行度が達したら終了
   if (m_moveProgress >= 1.0f)
   {
	  m_moveProgress = 0.0f;
	  m_isFinished = true;
   }
}


/// <summary>
/// ギミック非アクティブ
/// </summary>
void RotationFloor::Deactive()
{
   //アニメーションの進行度の更新
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);

   //回転開始地点と終了地点を設定
   auto start = GetRotation();
   auto delta = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, ROTATION_Y);
   auto end = start * delta;
   //回転の間を補間する
   m_floorRotation = DirectX::SimpleMath::Quaternion::Slerp(end, start, m_moveProgress);

   //進行度が達したら終了
   if (m_moveProgress >= 1.0f)
   {
	  m_moveProgress = 0.0f;
	  m_isFinished = true;
   }
}

