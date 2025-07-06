/*
	@file	Prison.cpp
	@brief	牢屋クラス
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Prison/Prison.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/Character/Player/Player.h"
#include "Game/Render/CustomModelRenderer.h"
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">オブジェクトのタイプ</param>
/// <param name="param">オブジェクトのパラメータ</param>
Prison::Prison(const int& type, const GameParameters::ObjectParameter& param)
   :
   ObjectBase(param.initialPos, param.initialRot, param.scale),
   m_doarPosition(param.initialPos),
   m_doarMoved(0.0f),
   m_isOpen(false),
   m_isOepnEnd(false),
   m_objectStageNumber(param.stageParam.objectNumber),
   m_collisionObject(nullptr)
{
   UNREFERENCED_PARAMETER(type);
}

/// <summary>
/// デストラクタ
/// </summary>
Prison::~Prison()
{
}


/// <summary>
/// 初期化する
/// </summary>
void Prison::Initialize()
{
	// モデルを読み込む
	m_prisonModel[PrisonParts::ROOM] = yamadalib::Resources::GetInstance()->GetModel("Prison_Room");
	m_prisonModel[PrisonParts::DOAR] = yamadalib::Resources::GetInstance()->GetModel("Prison_Door");
	m_roomdds = yamadalib::Resources::GetInstance()->GetTexture("PrisonRoom_DDS");
	m_doordds = yamadalib::Resources::GetInstance()->GetTexture("PrisonDoor_DDS");

	//境界ボックスを生成する
	InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Prison::Update(const float elapsedTime)
{
	if (m_isOpen)
	{
		MoveDoar(elapsedTime);
	}
}

/// <summary>
/// 描画する
/// </summary>
void Prison::Render()
{
	// ワールド行列を更新する
   DirectX::SimpleMath::Matrix prisonRoomMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   DirectX::SimpleMath::Matrix prisonDoarMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(m_doarPosition);

   // モデルを描画する
   m_graphics->GetCustomModelRenderer()->Draw(*m_prisonModel[PrisonParts::ROOM], m_roomdds.Get(), prisonRoomMatrix);
   m_graphics->GetCustomModelRenderer()->Draw(*m_prisonModel[PrisonParts::DOAR], m_doordds.Get(), prisonDoarMatrix);
}

/// <summary>
/// 後始末する
/// </summary>
void Prison::Finalize()
{
	// do nothing.
}


/// <summary>
/// 衝突判定の登録
/// </summary>
void Prison::InitializeCollision()
{
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(PRISON_BASE_EXTENTS, GetRotation());
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_BASE_EXTENTS, GetRotation());

   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,										  //当たり判定目的
	  CollisionManager::CollisionType::PUSH,											  //判定のタイプ
	  DirectX::BoundingBox(GetPosition(), adjustedExtents));							  //境界ボックス
   CollisionManager::Collision eventCollision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::EVENT_TRIGGER,								  //当たり判定の目的
	  CollisionManager::CollisionType::HIT,												  //判定のタイプ
	  DirectX::BoundingBox(GetPosition() + EVENT_BASE_EXTENTS, eventAdjustedExtents));	  //境界ボックス
	  
   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::PRISON, collision };
   //イベント用の衝突判定パラメータ
   CollisionManager::CollisionParameter eventParam = { this, CollisionManager::ObjectType::PRISON, eventCollision, [this](const CollisionObject* other) { HandleCollisionEvent(other); }};

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
   CollisionManager::GetInstance().RegisterObject(eventParam, m_objectStageNumber);
}



/// <summary>
/// イベント衝突判定
/// </summary>
/// <param name="other">衝突対象</param>
void Prison::HandleCollisionEvent(const CollisionObject* other)
{
   if (other->GetCollisionParameter().objectID != CollisionManager::ObjectType::PLAYER) return;
   
   Player* player = static_cast<Player*>(other->GetOwner());
   if (!m_isOpen && player->HasKey())
   {
	  player->UseKey();	//鍵を使用して牢屋を開ける
	  m_isOpen = true;
   }
}

/// <summary>
/// 扉を動かす
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Prison::MoveDoar(const float elapsedTime)
{
   //ローカルの移動量を計算
   const float localMoveAmount = OPEN_SPEED * elapsedTime;

   m_doarMoved += localMoveAmount;
   m_doarMoved = yamadalib::Math::Clamp(m_doarMoved, 0.0f, OPEN_LENGTH);

   //回転を適用
   const DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
   //ローカル座標系の移動ベクトルを作成
   DirectX::SimpleMath::Vector3 localMovement(m_doarMoved, 0.0f, 0.0f);
   //ワールド座標に変換
   const DirectX::SimpleMath::Vector3 worldOfset = DirectX::SimpleMath::Vector3::Transform(localMovement, rotationMatrix);
   //座標を更新
   m_doarPosition = GetPosition() + worldOfset;

   //終了フラグ
   if (m_doarMoved >= OPEN_LENGTH)
   {
	  m_isOepnEnd = true;
   }
}

/// <summary>
/// 牢屋を開ける
/// </summary>
void Prison::OpenPrison()
{
	m_isOpen = true;
}
