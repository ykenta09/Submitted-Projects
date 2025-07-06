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
#include "Game/Map/Floor/MovingFloor.h"
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
MovingFloor::MovingFloor(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID)
	:
    GimmickObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_startPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_movePosition(movePos),
    m_gimickID(gimmickID),
    m_moveProgress(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
MovingFloor::~MovingFloor()
{
}


/// <summary>
/// 初期化する
/// </summary>
void MovingFloor::Initialize()
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
void MovingFloor::Update(const float elapsedTime)
{
   //時間の更新
   m_time = elapsedTime;

   //衝突判定の情報更新
   if (m_collisionObject)
   {
	  m_collisionObject->UpdatePosition(GetPosition());
   }
}

/// <summary>
/// 描画する
/// </summary>
void MovingFloor::Render()
{
   // ワールド行列を更新する
   DirectX::SimpleMath::Matrix matrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	// モデルを描画する
    m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), matrix);
}

/// <summary>
/// 後始末する
/// </summary>
void MovingFloor::Finalize()
{
	// do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void MovingFloor::InitializeCollision()
{
   //回転を適応した衝突判定のサイズの調整
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, GetRotation());

   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::Sensor,
	  CollisionManager::CollisionType::HIT,
	  DirectX::BoundingBox(GetPosition(), adjustedExtents));
   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FLOOR, collision };

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}


/// <summary>
/// ギミックアクティブ
/// </summary>
void MovingFloor::Active()
{
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);
   SetPosition(yamadalib::Math::Lerp(m_startPosition, m_movePosition, m_moveProgress));
   if (m_moveProgress >= 1.0f)
   {
	  m_isFinished = true;
	  m_moveProgress = 0.0f;
   }
}


/// <summary>
/// ギミック非アクティブ
/// </summary>
void MovingFloor::Deactive()
{
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);
   SetPosition(yamadalib::Math::Lerp(m_movePosition, m_startPosition, m_moveProgress));
   if (m_moveProgress >= 1.0f)
   {
	  m_isFinished = true;
	  m_moveProgress = 0.0f;
   }
}

