/*
	@file	Enemy.cpp
	@brief	敵クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Enemy/EnemyVisibility.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"
#include "Game/Map/Map.h"
#include "Game/Collision/CollisionObject.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="map">マップ</param>
/// <param name="data">敵のパラメータ</param>
/// <param name="stageNumber">ステージ番号</param>
Enemy::Enemy(
   IComponent* parent, 
   CommonResources* resources, 
   Map* map, 
   const GameParameters::EnemyData data, 
   const int& stageNumber)
   :
   CharacterBase(parent, resources, data.charaData.position, data.charaData.rotation, data.charaData.scale),
   m_map(map),
   m_patrolRootMap(data.patrolRouteMap),
   m_adjacencyList(data.patrolAdjacencyList),
   m_initialVertexNumber(data.initalNumber),
   m_enemyNumber(data.enemyNumber),
   m_enemyDirection(DirectX::SimpleMath::Vector3::Zero),
   m_enemy(this),
   m_nextAngle(0.0f),
   m_viewType(true)
{
   m_objectStageNumber = stageNumber;
   //キャラクタータイプの設定
   SetCharacterType(CharacterType::Enemy);
}


/// <summary>
/// デストラクタ
/// </summary>
Enemy::~Enemy()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void Enemy::Initialize()
{
    //モデル情報を適用する
    ApplyModelAndTexture();
	//敵のパーツを生成
   	CharacterBase::Initialize();
	//テクスチャを取得する
	m_detectionTexture = yamadalib::Resources::GetInstance()->GetTexture("Detection");

	//敵の待機状態を生成、初期化
	m_enemyWaiting = std::make_unique<EnemyWaiting>(this);
	m_enemyWaiting->Initialize();
	//敵の徘徊状態を生成、初期化
	m_enemyWandering = std::make_unique<EnemyWandering>(this);
	m_enemyWandering->SetGraphDate(m_patrolRootMap, m_adjacencyList, m_initialVertexNumber);
	m_enemyWandering->Initialize();
	//初期座標を取得する
	if (m_enemyWandering->GetPatrolMap().size() > 0)
	{
	   m_initialPosition = m_enemyWandering->GetInitialPosition();
	}
	//敵の徘徊に戻る状態を生成、初期化
	m_enemyWanderingReturn = std::make_unique <EnemyWanderingReturn>(this);
	m_enemyWanderingReturn->Initialize();
	//敵の追跡状態を生成、初期化
	m_enemyChasing = std::make_unique<EnemyChasing>(this);
	m_enemyChasing->Initialize();
	//敵の探索状態を生成、初期化
	m_enemyRouteSearch = std::make_unique<EnemyRouteSearch>(this, m_map, m_objectStageNumber);
	m_enemyRouteSearch->Initialize();
	//敵の視界を生成、初期化
	m_visibility = std::make_unique<EnemyVisibility>(this);
	m_visibility->Initialize();
	//敵のタイトル状態の生成、初期化
	m_enemyTitle = std::make_unique<EnemyTitle>(this);
	m_enemyTitle->Initialize();
	//AStarの生成、初期化
	if (m_map)
	{   
	   m_multiStageAStar = std::make_unique<MultiStageAStar>(m_enemyRouteSearch->GetGrid(), m_map->GetStageTransition());
	}

	//初期状態を設定する
	m_currentState = m_enemyWaiting.get();
	m_currentPosition = m_initialPosition;
	//境界ボックスを設定する
	InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">ターゲット座標</param>
void Enemy::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
    //時間の更新
	m_time = elapsedTime;
  	m_searchTime -= elapsedTime;

	//現在の状態を更新する
	m_currentState->Update(elapsedTime);

	//追いかけるターゲット座標を更新する
	m_currentPosition += position;
	m_currentAngle += angle;
	m_targetPosition = target;

	//当たり判定の更新
	m_collisionObject->UpdatePosition(m_currentPosition);

	//追跡するターゲットの設定
	m_enemyRouteSearch->SetTargetStageNumber(m_commonResources->GetBlackBoard()->GetStageNumber());

	//視界の更新
	m_visibility->Update();
	// 敵パーツを更新する
	CharacterBase::Update(elapsedTime, m_currentPosition, m_currentAngle, m_targetPosition);
	
	if (m_currentState == m_enemyTitle.get()) return;

	//敵の視野に入ったら状態を変更する
	if (m_currentState == m_enemyChasing.get())
	   m_viewType = false;
	else
	   m_viewType = true;

	if (InView(m_viewType))
	   Search();

}

/// <summary>
/// 描画する
/// </summary>
void Enemy::Render()
{
	//現在の状態を描画する
	m_currentState->Render();

	m_visibility->DrawVisibility(m_map);

	//敵のパーツを描画する
	CharacterBase::Render();

	//視界に入ったら描画する
	if (m_currentState == m_enemyTitle.get()) return;
    if (InView(true))
    {
	   //敵の行列を生成
	   DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_currentPosition);
	   //プレイヤーを発見した時の!マーク描画
	   m_graphics->DrawTextureBillboard(m_detectionTexture.Get(), world);
    }

#ifdef _DEBUG

	// デバッグ情報を「DebugString」で表示する
	//auto debugString = m_commonResources->GetDebugString();
	//debugString->AddString("EnemyPosisiton, %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	//debugString->AddString("EnemyCurrentStage, %d", m_objectStageNumber);

	//if (m_currentState == GetEnemyChasing())
	//{
	//	debugString->AddString("EnemyState : Chasing");
	//}
	//else if (m_currentState == GetEnemyWaiting())
	//{
	//	debugString->AddString("EnemyState: Waiting");
	//}
	//else if (m_currentState == GetEnemyWandering())
	//{
	//	debugString->AddString("EnemyState : Wandering");
	//}
	//else if (m_currentState == GetEnemyWanderingReturn())
	//{
	//   debugString->AddString("EnemyState : Wandering Return");
	//}
	//else if (m_currentState == GetEnemyRouteSearch())
	//{
	//   debugString->AddString("EnemyState : RouteSearch");
	//}
#endif // _DEBUG
}

/// <summary>
/// 後始末する
/// </summary>
void Enemy::Finalize()
{
	m_enemyWaiting.reset();
	m_enemyChasing.reset();
	m_enemyRouteSearch.reset();
	m_enemyWandering.reset();
	m_enemyWanderingReturn.reset();
}

/// <summary>
/// 当たり判定の初期化
/// </summary>
void Enemy::InitializeCollision()
{
   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //当たり判定目的
	  CollisionManager::CollisionType::HIT,							 //判定のタイプ
	  DirectX::BoundingBox(m_currentPosition, m_scale / 2.0f));		 //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this,	CollisionManager::ObjectType::ENEMY, collision,	[this](const CollisionObject* other) { HandleCollision(other); }};

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param);
}

/// <summary>
/// 衝突時の処理
/// </summary>
/// <param name="other">衝突対象</param>
void Enemy::HandleCollision(const CollisionObject* other)
{
   //コリジョンタイプが押し出しではない場合処理をしない
   if (other->GetCollisionParameter().collision.collisionType != CollisionManager::CollisionType::PUSH) return;
   DirectX::SimpleMath::Vector3 pushBack = m_collisionObject->CalculatePushBack(other);
   PushPosition(m_currentPosition + pushBack);
}

/// <summary>
/// 視野の中に入っているか？
/// </summary>
/// <param name="object">障害物との判定を行うかどうか</param>
/// <returns></returns>
bool Enemy::InView(bool object)
{
	// ターゲットまでの方向ベクトルを計算する
	m_targetDirection = m_targetPosition - m_currentPosition;

	//ターゲットが視野範囲外なら処理しない
	if (m_targetDirection.Length() >= EnemyVisibility::VISIBILITIY_DISTANCE) return false;

	// 敵の向きベクトル
	m_enemyDirection = DirectX::SimpleMath::Vector3(sinf(m_currentAngle), 0.0f, cosf(m_currentAngle)); // m_currentAngle は敵の向いている角度
	m_targetDirection.Normalize();

	// 内積を計算する
	float dotProduct = m_enemyDirection.Dot(m_targetDirection);

	// 視野の角度をラジアンに変換
	float viewAngle = cosf(DirectX::XMConvertToRadians(EnemyVisibility::FIELD_OF_VIEW / 2.0f));

	// 内積が視野の角度内にあるかをチェック
	if (dotProduct > viewAngle) 
	{
	   //障害物に遮られているか確認
	   if (object)
	   {
		  return RayMapObject();
	   }
	   return true;
	}
	return false;
}

/// <summary>
/// レイとマップオブジェクトの判定
/// </summary>
/// <returns></returns>
bool Enemy::RayMapObject()
{
   // プレイヤー方向へのレイを設定
   DirectX::SimpleMath::Vector3 directionToTarget = m_targetPosition - m_currentPosition;
   // プレイヤーまでの距離と正規化
   float targetDistance = directionToTarget.Length(); 
   directionToTarget.Normalize();
   //敵からプレイヤーまでの距離にレイを作成
   DirectX::SimpleMath::Ray viewRay(m_currentPosition, directionToTarget);

   if (m_map == nullptr) return true;
   //マップオブジェクトと衝突判定
   for (const auto& object : m_map->GetBoundingBoxes())
   {
	  float distance = 0;
	  if (object.Intersects(viewRay.position, viewRay.direction, distance))
	  {
		 if (distance < targetDistance && distance <= EnemyVisibility::VISIBILITIY_DISTANCE) 
		 return false;
	  }
   }
   return true;
}

/// <summary>
/// 状態を変更する
/// </summary>
/// <param name="newState">変更するステイト</param>
void Enemy::ChangeState(IState* newState)
{
	m_currentState = newState;
	//初期回転を消す
	if(m_applyInitialRotation)	LostInitialRotation();
}

/// <summary>
/// 索敵
/// </summary>
void Enemy::Search()
{
   //最初の索敵
   if (!m_enemyRouteSearch->GetFirstExploration())
   {
	  m_searchTime = TIME_TO_SEARCH;
	  ChangeState(m_enemyRouteSearch.get());
	  m_enemyRouteSearch->SetAstar(m_targetPosition);
   }
   else//二度目以降の再探索
   {
	  if (m_enemyRouteSearch->GetGrid()->GetVertexFromPosition(m_objectStageNumber, m_targetPosition) == nullptr) return;
	  //ターゲットの座標が変わったとき再探索
	  if (m_enemyRouteSearch->GetTargetCell()->GetVertexNumber() != m_enemyRouteSearch->GetGrid()->GetVertexFromPosition(m_objectStageNumber, m_targetPosition)->GetVertexNumber())
	  {
		 m_searchTime = TIME_TO_SEARCH;
		 ChangeState(m_enemyRouteSearch.get());
		 m_enemyRouteSearch->SetAstar(m_targetPosition);
	  }
   }
}

/// <summary>
/// モデル情報を適用する
/// </summary>
void Enemy::ApplyModelAndTexture()
{
   CharacterBase::CharacterResources resource;
   resource.head = yamadalib::Resources::GetInstance()->GetModel("EnemyHead");
   resource.body = yamadalib::Resources::GetInstance()->GetModel("EnemyBody");
   resource.arm = yamadalib::Resources::GetInstance()->GetModel("EnemyArm");
   resource.foot = yamadalib::Resources::GetInstance()->GetModel("EnemyFoot");
   resource.headDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyHead_DDS");
   resource.bodyDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyBody_DDS");
   resource.armDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyArm_DDS");
   resource.footDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyFoot_DDS");
   //ベースクラスにモデル情報を適用
   CharacterBase::SetPartModels(resource);
}

/// <summary>
/// キーボード処理
/// </summary>
/// <param name="keys">押されたキー</param>
inline void Enemy::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// 衝突したときにプレイヤーを押し出す
/// </summary>
/// <param name="position">押し出す座標</param>
void Enemy::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
   m_currentPosition = position;
   //境界球の更新
   m_collisionObject->UpdatePosition(m_currentPosition);
}
