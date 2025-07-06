/*
	@file	Key.cpp
	@brief	鍵クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Key/Key.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
Key::Key(const GameParameters::ObjectParameter& param)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
	m_isExist(true),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_collisionObject(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Key::~Key()
{
}

/// <summary>
/// 初期化する
/// </summary>
void Key::Initialize()
{
	// モデルを読み込む
	SetModel(yamadalib::Resources::GetInstance()->GetModel("Key"));
	m_dds = yamadalib::Resources::GetInstance()->GetTexture("Key_DDS");
	//境界ボックスを生成する
	InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Key::Update(const float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	//回転を加える
	SetRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, 0.01f));
}


/// <summary>
/// 描画する
/// </summary>
void Key::Render()
{
	//存在しない場合処理しない
	if (!m_isExist) return;

	// ワールド行列を更新する
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
		DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	// モデルを描画する
	m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), world);

}

/// <summary>
/// 後始末する
/// </summary>
void Key::Finalize()
{
	// do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void Key::InitializeCollision()
{
   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,						   //当たり判定目的
	  CollisionManager::CollisionType::HIT,								   //判定のタイプ
	  DirectX::BoundingBox(GetPosition(), GetScale() / 2));				   //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::KEY, collision, [this](const CollisionObject* other) { HandleCollision(other); }};

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}

/// <summary>
/// 衝突時のイベント処理
/// </summary>
/// <param name="other">衝突対象のコリジョンオブジェクト</param>
void Key::HandleCollision(const CollisionObject* other)
{
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::PLAYER)
   {
	  //鍵の衝突判定を消す
	  LostExist();
	  m_isExist = false;
   }
}


/// <summary>
/// 鍵の存在を消す
/// </summary>
void Key::LostExist() const
{
	//オブジェクトの登録解除
   CollisionManager::GetInstance().UnregisterStageObject(m_objectStageNumber, m_collisionObject);
}
