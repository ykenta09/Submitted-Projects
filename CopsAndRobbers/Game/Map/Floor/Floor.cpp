/*
	@file	Floor.cpp
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
#include "Game/Map/Floor/Floor.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/Render/CustomModelRenderer.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">オブジェクトのタイプ</param>
/// <param name="param">オブジェクトのパラメータ</param>
Floor::Floor(const int& type, const GameParameters::ObjectParameter& param)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Floor::~Floor()
{
}

/// <summary>
/// 初期化する
/// </summary>
void Floor::Initialize()
{
   // タイプに応じてリソースを取得する
   switch (m_type)
   {
   case 1:
	  m_model = yamadalib::Resources::GetInstance()->GetModel("Floor_01");
	  m_texture = yamadalib::Resources::GetInstance()->GetTexture("Floor01_DDS");
	  break;
   case 2:
	  m_model = yamadalib::Resources::GetInstance()->GetModel("Floor_02");
	  m_texture = yamadalib::Resources::GetInstance()->GetTexture("Floor02_DDS");
	  break;
   case 3:
	  m_model = yamadalib::Resources::GetInstance()->GetModel("Floor_03");
	  m_texture = yamadalib::Resources::GetInstance()->GetTexture("Floor03_DDS");
	  break;
   }
   //当たり判定の登録
   InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Floor::Update(const float elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
}


/// <summary>
/// 描画する
/// </summary>
void Floor::Render()
{
	// ワールド行列
	DirectX::SimpleMath::Matrix world =
	   DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	   DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	//モデルを描画する
	m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_texture.Get(), world, DirectX::SimpleMath::Vector2(GetScale().x, GetScale().z));
}


/// <summary>
/// クワッドを生成関数
/// </summary>
/// <param name="x">頂点のX座標</param>
/// <param name="y">頂点のY座標</param>
/// <param name="z">頂点のZ座標</param>
/// <param name="u">テクスチャU座標<</param>
/// <param name="v">テクスチャV座標<</param>
/// <returns></returns>
inline DirectX::VertexPositionTexture Floor::MakeVPT(float x, float y, float z, float u, float v) 
{
   return DirectX::VertexPositionTexture(DirectX::XMFLOAT3(x, y, z), DirectX::XMFLOAT2(u, v));
}


/// <summary>
/// 後始末する
/// </summary>
void Floor::Finalize()
{
	// do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void Floor::InitializeCollision()
{
   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,						   //当たり判定目的
	  CollisionManager::CollisionType::HIT,								   //判定のタイプ
	  DirectX::BoundingBox(GetPosition(), GetScale() / 2));				   //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FLOOR, collision };
   // 当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}
