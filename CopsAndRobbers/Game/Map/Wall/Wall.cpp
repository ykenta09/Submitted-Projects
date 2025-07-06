/*
	@file	Wall.cpp
	@brief	壁クラス
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Wall/Wall.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Render/CustomModelRenderer.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">オブジェクトタイプ</param>
/// <param name="param">オブジェクトのパラメータ</param>
Wall::Wall(const int& type, const GameParameters::ObjectParameter& param)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_collisionObject(nullptr)
{
}

/// <summary>
///デストラクタ
/// </summary>
Wall::~Wall()
{
   Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void Wall::Initialize()
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
void Wall::Update(const float elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
}

/// <summary>
/// 描画する
/// </summary>
void Wall::Render()
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
void Wall::Finalize()
{
	// do nothing.
}

/// <summary>
/// 当たり判定の初期化
/// </summary>
void Wall::InitializeCollision()
{
   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //当たり判定の目的
	  CollisionManager::CollisionType::PUSH,						 //判定のタイプ
	  DirectX::BoundingBox(GetPosition(), GetScale() / 2));			 //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::WALL, collision };
   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}
