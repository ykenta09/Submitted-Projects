/*
	@file	MovingWall.cpp
	@brief	動く壁クラス
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Wall/MovingWall.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Render/CustomModelRenderer.h"
#include "Game/Collision/CollisionObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">モデルのタイプ</param>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="movePos">移動先に座標</param>
/// <param name="gimmickID">ギミックID</param>
MovingWall::MovingWall(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID)
	:
    GimmickObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_startPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_movePosition(movePos),
    m_gimickID(gimmickID),
    m_moveProgress(0.0f),
    m_collisionObject(nullptr)
{
}

/// <summary>
///デストラクタ
/// </summary>
MovingWall::~MovingWall()
{
   Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void MovingWall::Initialize()
{
	// モデルを読み込む
   switch (m_type)
   {
   case 1:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_01"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall01_DDS");
	  break;
   case 2:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_02"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall02_DDS");
	  break;
   case 3:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_03"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall03_DDS");
	  break;
   case 4:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_04"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall04_DDS");
	  break;
   }
	//衝突判定を登録する
	InitializeCollision();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void MovingWall::Update(const float elapsedTime)
{
   m_time = elapsedTime;
   if (m_collisionObject)
   {
	  m_collisionObject->UpdatePosition(GetPosition());
   }
}

/// <summary>
/// 描画する
/// </summary>
void MovingWall::Render()
{
   // ワールド行列を更新する
   DirectX::SimpleMath::Matrix matrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   //モデルを描画する
   m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), matrix);

}

/// <summary>
/// 後始末する
/// </summary>
void MovingWall::Finalize()
{
	// do nothing.
}

/// <summary>
/// 当たり判定の初期化
/// </summary>
void MovingWall::InitializeCollision()
{
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, GetRotation());

   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //当たり判定の目的
	  CollisionManager::CollisionType::PUSH,						 //判定のタイプ
	  DirectX::BoundingBox(GetPosition(), adjustedExtents));			 //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::WALL, collision };
   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}

/// <summary>
/// ギミックアクティブ
/// </summary>
void MovingWall::Active()
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
void MovingWall::Deactive()
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
