/*
	@file	Player.cpp
	@brief	プレイヤークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Factory/CharacterFactory.h"
#include "Game/Character/Player/MouseControl.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Map/Key/Key.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="data">キャラクターデータ</param>
Player::Player(IComponent* parent, CommonResources* resources, const GameParameters::CharacterData data, const int& stageNumber)
	:
	CharacterBase(parent, resources, data.position, data.rotation, data.scale),
    m_commonResources(resources),
	m_controlMouse{},
    m_nextMoveAngle(0.0f),
    m_previousVector(DirectX::SimpleMath::Vector3::Forward),
    m_sprintCoolTime(SPRINT_COOLTIME),
    m_isHit{ false },
    m_isGoal{false},
	m_hasKey(false),
    m_isGoalEvent(false),
    m_isStartEvent(true),
    m_player(this)
{
    m_objectStageNumber = stageNumber;
    //キャラクターのタイプを設定する
    SetCharacterType(CharacterType::Player);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void Player::Initialize()
{
    //モデル情報を適用する
    ApplyModelAndTexture();
	
	//プレイヤーのパーツを生成
	CharacterBase::Initialize();
	//マウスコントロールを生成
	m_controlMouse = std::make_unique<MouseControl>(m_commonResources);
	//スタミナを生成する
	m_stamina = std::make_unique<Stamina>();
	m_stamina->Initialize();

	//プレイヤーのアイドル状態を生成、初期化
	m_playerIdle = std::make_unique<PlayerIdle>(this);
	m_playerIdle->Initialize();
	//プレイヤーの移動状態を生成、初期化
	m_playerMove = std::make_unique<PlayerMove>(this);
	m_playerMove->Initialize();
	//プレイヤーの回避状態を生成、初期化
	m_playerSprint = std::make_unique<PlayerSprint>(this);
	m_playerSprint->Initialize();
	//プレイヤーのゴール状態
	m_playerGoal = std::make_unique<PlayerGoal>(this);
	m_playerGoal->Initialize();
	//プレイヤーのタイトル状態
	m_playerTitle = std::make_unique<PlayerTitle>(this);
	m_playerTitle->Initialize();

	m_currentPosition = m_initialPosition;

	//初期状態を設定する
	m_currentState = m_playerIdle.get();
	//衝突判定の初期化
	InitializeCollision();
	//ゲームデータを取得して設定する
	SetGameData();
}

/// <summary>
///  更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">ターゲット座標の更新</param>
void Player::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
    UNREFERENCED_PARAMETER(angle);
	m_time += elapsedTime;
	//クールタイムを減らす
	m_sprintCoolTime -= elapsedTime;
	m_playerVelocity = DirectX::SimpleMath::Vector3::Vector3::Zero;

	//マウスの更新とマウスの総合移動量を更新
	if (!m_isStartEvent)
	{
	   m_controlMouse->Update(elapsedTime);
	}

	//視点の回転量を取得する
	m_cameraAngle = m_controlMouse->GetYaw();

	//現在の状態を更新する
	m_currentState->Update(elapsedTime);

	//現在の情報を更新
	m_currentPosition += position;

	//当たり判定の更新
	if (m_collisionObject)
	{
	   m_collisionObject->UpdatePosition(m_currentPosition + COLLISION_OFFSET);
	}

	//スタミナを更新
	m_stamina->Update(elapsedTime);

	//ゴール状態に変更
	if (m_isGoal)
	{
	   ChangeState(m_playerGoal.get());
	}
	// プレイヤーパーツを更新する
	CharacterBase::Update(elapsedTime, m_currentPosition, m_cameraAngle + m_nextMoveAngle, target);
}

/// <summary>
/// 描画する
/// </summary>
inline void Player::Render()
{
	//現在の状態を描画する
	m_currentState->Render();

	//共通処理の描画
	CharacterBase::Render();

#ifdef _DEBUG

	// デバッグ情報を「DebugString」で表示する
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("PlayerCurrentPosition, %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	debugString->AddString("PlayerCurrentAngle, %f", DirectX::XMConvertToDegrees(GetAngle()));
	debugString->AddString("HasKey, %d", m_keyNum);
	debugString->AddString("PlayerCurrentStage, %d", m_objectStageNumber);

	if (m_currentState == GetPlayerIdle())
	{
		debugString->AddString("PlayerState : IDLE");
	}
	else if (m_currentState == GetPlayerMove())
	{
		debugString->AddString("PlayerState: MOVE");
	}
	else if(m_currentState == GetPlayerSprint())
	{
		debugString->AddString("PlayerState : Sprint");
	}
	else if (m_currentState == GetPlayerGoal())
	{
	   debugString->AddString("PlayerState : Goal");
	}

	//境界ボックスのデバック描画
    //m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
    //[this](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
    //{
   	//	 m_graphics->DrawBoundingSphere(m_collisionObject->GetCollisionParameter().collision.sphere);
   	//	 m_graphics->DrawBoundingBox(m_collisionObject->GetCollisionParameter().collision.box);
    //});

#endif // _DEBUG
}

/// <summary>
///  後始末する
/// </summary>
void Player::Finalize()
{
   m_playerGoal.reset();
   m_playerIdle.reset();
   m_playerMove.reset();
   m_playerSprint.reset();
   m_playerTitle.reset();
   m_controlMouse.reset();
   m_stamina.reset();
   m_commonResources->GetBlackBoard()->SetHasKeyNum(m_keyNum);
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void Player::InitializeCollision()
{
   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,						//当たり判定目的
	  CollisionManager::CollisionType::HIT,								//判定のタイプ
	  DirectX::BoundingBox(m_currentPosition , COLLISION_EXTENTS));	   //境界ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::PLAYER, collision, [this](const CollisionObject* other) { HandleCollision(other); } };

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param);
}

/// <summary>
/// ゲームデータを設定する
/// </summary>
void Player::SetGameData()
{
   m_keyNum = m_commonResources->GetBlackBoard()->GetHasKeyNum();
}

/// <summary>
///  衝突時の処理
/// </summary>
/// <param name="other">衝突対象のコリジョンオブジェクト</param>
void Player::HandleCollision(const CollisionObject* other)
{
   //衝突対象が敵の場合
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::ENEMY)
   {
	  m_isHit = true;
   }
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::KEY)
   {
	  const Key* key = static_cast<const Key*>(other->GetOwner());
	  if (key)
	  {
		 m_hasKey = true;  //鍵を所持する
		 m_keyNum++;	   //所持している鍵の数を増やす
		 m_commonResources->GetBlackBoard()->SetHasKeyNum(m_keyNum);
	  }
   }
   //コリジョンタイプが押し出しではない場合処理をしない
   if (other->GetCollisionParameter().collision.collisionType != CollisionManager::CollisionType::PUSH) return;
   PushPosition(m_currentPosition + m_collisionObject->CalculatePushBack(other));
}

/// <summary>
/// 衝突したときにプレイヤーを押し出す
/// </summary>
/// <param name="position">押し出し座標</param>
void Player::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
    //座標を更新する
	m_currentPosition = position;
	//当たり判定の更新
	m_collisionObject->UpdatePosition(m_currentPosition + COLLISION_OFFSET);
}

/// <summary>
/// 現在の状態を変更する
/// </summary>
/// <param name="newState">新しい状態</param>
void Player::ChangeState(IState* newState)
{
    //新しい状態に変更
	m_currentState = newState;
	//初期回転を消す
	if(m_applyInitialRotation) LostInitialRotation();
}

/// <summary>
/// 所持している鍵を使用する
/// </summary>
void Player::UseKey()
{
   //所持している鍵の数を減らす
   m_keyNum -= 1;
   m_commonResources->GetBlackBoard()->SetHasKeyNum(m_keyNum);
   if (m_keyNum > 0) return;
   m_hasKey = false;	//所持している鍵が0ならフラグを変更
}

/// <summary>
/// モデル情報を適用する
/// </summary>
void Player::ApplyModelAndTexture()
{
   CharacterBase::CharacterResources resource;
   resource.head = yamadalib::Resources::GetInstance()->GetModel("PlayerHead");
   resource.body = yamadalib::Resources::GetInstance()->GetModel("PlayerBody");
   resource.arm = yamadalib::Resources::GetInstance()->GetModel("PlayerArm");
   resource.foot = yamadalib::Resources::GetInstance()->GetModel("PlayerFoot");
   resource.headDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerHead_DDS");
   resource.bodyDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerBody_DDS");
   resource.armDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerArm_DDS");
   resource.footDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerFoot_DDS");
   //ベースクラスにモデル情報を適用
   CharacterBase::SetPartModels(resource);
}

/// <summary>
/// 観察者に通知する
/// </summary>
/// <param name="keys">押しているキー</param>
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	//移動キーの処理
	switch (keys)
	{
	case DirectX::Keyboard::W:// 前進ベクトルを加算
	    m_playerVelocity += DirectX::SimpleMath::Vector3::Forward;
		break;
	case DirectX::Keyboard::S:// 後退ベクトルを加算
		m_playerVelocity += DirectX::SimpleMath::Vector3::Backward;
		break;
	case DirectX::Keyboard::A:// 左移動ベクトルを加算
		m_playerVelocity += DirectX::SimpleMath::Vector3::Left;
		break;
	case DirectX::Keyboard::D:// 右移動ベクトルを加算
		m_playerVelocity += DirectX::SimpleMath::Vector3::Right;
		break;
	case DirectX::Keyboard::LeftShift:// ダッシュ・回避処理
		if (m_sprintCoolTime <= 0.0f && !m_stamina->IsAvoidance() || m_playerSprint->GetIsSprint()) break;//回避中なら処理しない
		ChangeState(m_playerSprint.get());			  // スプリント状態に遷移
		m_playerSprint->PreSprint(m_previousVector);  // スプリント前の移動ベクトルを記録
		m_stamina->ConsumeStamina();				  //回避分のスタミナを消費する
		break;
	}

	// 何らかの移動入力があった場合
	if (m_playerVelocity.Length() > 0.0f)
	{
	    // 前フレームの移動ベクトルを保存
		m_previousVector = m_playerVelocity;
		// スプリント中でなければ通常移動状態へ
		if (!m_playerSprint->GetIsSprint())
		{
		   m_player->SetMoveAngle(m_playerMove->GetAngleFromVector(m_playerVelocity));		//移動ベクトルから角度を求める
		   m_playerMove->SetVelocity(m_playerVelocity);
		   ChangeState(m_playerMove.get());
		}
	}
}