/*
	@file	Fellow.cpp
	@brief	仲間クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/MyLib/BlackBoard.h"


/// <summary>
///  コンストラクタ
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="map">マップ</param>
/// <param name="data">キャラクターデータ</param>
/// <param name="stageNumber">ステージ番号</param>
Fellow::Fellow(IComponent* parent, CommonResources* resources, Map* map, const GameParameters::CharacterData data, const int& stageNumber)
	:
	CharacterBase(parent, resources, data.position, data.rotation, data.scale),
    m_commonResources(resources),
    m_map(map),
    m_fellow(this)
{
   m_objectStageNumber = stageNumber;
   //キャラクタータイプを設定
   SetCharacterType(CharacterType::Fellow);
}

/// <summary>
/// デストラクタ
/// </summary>
Fellow::~Fellow()
{
	Finalize();
}


/// <summary>
/// /初期化する
/// </summary>
void Fellow::Initialize()
{
   //モデル情報を適用する
   ApplyModelAndTexture();

   //パーツを生成
   CharacterBase::Initialize();

   //仲間待機状態を生成、初期化
   m_fellowIdle = std::make_unique<FellowIdle>(this, m_map);
   m_fellowIdle->Initialize();
   //仲間追跡状態を生成、初期化
   m_fellowChasing = std::make_unique<FellowChasing>(this);
   m_fellowChasing->Initialize();
   //仲間探索状態を生成、初期化
   m_fellowSearch = std::make_unique<FellowRouteSearch>(this, m_map, m_objectStageNumber);
   m_fellowSearch->Initialize();

   if (m_map)
   {   //AStarの生成、初期化
	  m_multiStageAStar = std::make_unique<MultiStageAStar>(m_fellowSearch->GetGrid(), m_map->GetStageTransition());
   }
   
   //初期状態を設定
   m_currentState = m_fellowIdle.get();
   //当たり判定の設定
   InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">ターゲット座標</param>
void Fellow::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
    //時間の更新
    m_time = elapsedTime;

	//現在の状態を更新する
	m_currentState->Update(elapsedTime);

	//情報を更新する
	m_currentPosition += position;
	m_currentAngle += angle;
	m_targetPosition = target;

	//当たり判定の更新
	m_collisionObject->UpdatePosition(m_currentPosition);

	//ゴールフラグがtrueなら目標の位置まで移動する
	if (m_fellow->GetFellowChasing()->GeteGoalFlag())
	{
	   m_fellow->GetFellowChasing()->TargetMove(target, elapsedTime);
	}

	//追跡するターゲットの設定
	m_fellowSearch->SetTargetStageNumber(m_commonResources->GetBlackBoard()->GetStageNumber());

	//パーツの更新
	CharacterBase::Update(elapsedTime, m_currentPosition, m_currentAngle, m_targetPosition);
}


/// <summary>
/// 描画する
/// </summary>
void Fellow::Render()
{
	//現在の状態を描画する
	m_currentState->Render();

	CharacterBase::Render();
#ifdef _DEBUG
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("FellowCurrentPosition, %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	//debugString->AddString("FellowCurrentStage, %d", m_objectStageNumber);


	if (m_currentState == GetFellowChasing())
	{
	   debugString->AddString("FellowState : Chasing");
	}
	else if (m_currentState == GetFellowIdle())
	{
	   debugString->AddString("FellowState: Idle");
	}
	else if (m_currentState == GetFellowRouteSearch())
	{
	   debugString->AddString("FellowState : Search");
	}
#endif // _DEBUG
}

/// <summary>
/// 後始末する
/// </summary>
void Fellow::Finalize()
{
	m_fellowIdle.reset();
	m_fellowChasing.reset();
	m_fellowSearch.reset();
	//m_astar.reset();
}

/// <summary>
/// 当たり判定の登録
/// </summary>
void Fellow::InitializeCollision()
{
   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //当たり判定目的
	  CollisionManager::CollisionType::PUSH,						 //判定のタイプ
	  DirectX::BoundingBox(m_currentPosition, m_scale / 2.0f));		 //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FELLOW, collision, [this](const CollisionObject* other) { HandleCollision(other); }};

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param);
}

/// <summary>
/// 衝突時の処理
/// </summary>
/// <param name="other"></param>
void Fellow::HandleCollision(const CollisionObject* other)
{
   //牢屋とゴールの当たり判定を行わない
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::PRISON) return;	
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::GOAL) return;
   
   if (other->GetCollisionParameter().collision.collisionType != CollisionManager::CollisionType::PUSH) return;
   //押し出し処理
   DirectX::SimpleMath::Vector3 pushBack = m_collisionObject->CalculatePushBack(other);
   PushPosition(m_currentPosition + pushBack);
}

/// <summary>
/// 状態を変更する
/// </summary>
/// <param name="newState">変更ステート</param>
void Fellow::ChangeState(IState* newState)
{
	m_currentState = newState;
	//初期回転をなくす
	if(m_applyInitialRotation) LostInitialRotation();
}

/// <summary>
/// 押し出し処理
/// </summary>
/// <param name="position">押し出し量</param>
void Fellow::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
   m_currentPosition = position;
   //当たり判定の更新
   m_collisionObject->UpdatePosition(m_currentPosition);
}

/// <summary>
/// モデル情報を適用する
/// </summary>
void Fellow::ApplyModelAndTexture()
{
   CharacterBase::CharacterResources resource;
   resource.head = yamadalib::Resources::GetInstance()->GetModel("FellowHead");
   resource.body = yamadalib::Resources::GetInstance()->GetModel("FellowBody");
   resource.arm = yamadalib::Resources::GetInstance()->GetModel("FellowArm");
   resource.foot = yamadalib::Resources::GetInstance()->GetModel("FellowFoot");
   resource.headDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowHead_DDS");
   resource.bodyDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowBody_DDS");
   resource.armDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowArm_DDS");
   resource.footDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowFoot_DDS");
   //ベースクラスにモデル情報を適用
   CharacterBase::SetPartModels(resource);
}
