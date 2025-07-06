#include "pch.h"
#include "Game/Map/Goal/Goal.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/Math.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
Goal::Goal(const GameParameters::ObjectParameter& param)
    : 
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_doarLeftPosition(param.initialPos),
    m_doarRightPosition(param.initialPos - GOAL_POSITION_OFFSET),
    m_isOpened(false),
    m_time(0.0f),
    m_doarMoved(0.0f),
    m_goalPlayer(false),
    m_goalFellow(false),
    m_isGoal(false),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_collisionObject(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Goal::~Goal()
{
}
 

/// <summary>
/// 初期化する
/// </summary>
void Goal::Initialize()
{
    //モデル情報を取得する
    m_goalModel[GoalParts::FRAM] = yamadalib::Resources::GetInstance()->GetModel("GateFrame");
    m_goalModel[GoalParts::DOOR] = yamadalib::Resources::GetInstance()->GetModel("Gate");
    m_dds[GoalParts::FRAM] = yamadalib::Resources::GetInstance()->GetTexture("GateFrame_DDS");
    m_dds[GoalParts::DOOR] = yamadalib::Resources::GetInstance()->GetTexture("Gate_DDS");

    //衝突判定の登録
    InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Goal::Update(const float elapsedTime)
{
   m_time = elapsedTime;
   if (m_isOpened)
   {
      CollisionManager::GetInstance().UnregisterStageObject(m_objectStageNumber, m_collisionObject);
      m_collisionObject = nullptr;
   }
}

/// <summary>
/// 描画する
/// </summary>
void Goal::Render()
{
    // ワールド行列を更新する
   DirectX::SimpleMath::Matrix goalMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
      DirectX::SimpleMath::Matrix::CreateScale(GetScale());
    DirectX::SimpleMath::Matrix goalFramMatrix = goalMatrix *
       DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());
    DirectX::SimpleMath::Matrix goalDoarLeftMatrix = goalMatrix *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarLeftPosition);
    DirectX::SimpleMath::Matrix goalDoarRightMatrix = goalMatrix *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarRightPosition);

    m_graphics->GetCustomModelRenderer()->Draw(*m_goalModel[GoalParts::FRAM], m_dds[GoalParts::FRAM].Get(), goalFramMatrix);
    m_graphics->GetCustomModelRenderer()->Draw(*m_goalModel[GoalParts::DOOR], m_dds[GoalParts::DOOR].Get(), goalDoarLeftMatrix);
    m_graphics->GetCustomModelRenderer()->Draw(*m_goalModel[GoalParts::DOOR], m_dds[GoalParts::DOOR].Get(), goalDoarRightMatrix);

#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// 後処理をする
/// </summary>
void Goal::Finalize()
{
   // do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void Goal::InitializeCollision()
{
   //回転に応じたサイズ調整
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(COLISION_EXTENTS, GetRotation());
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_COLISION_EXTENTS, GetRotation());

   //当たり判定の設定
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
      CollisionManager::CollisionPurpose::PHYSICAL,
      CollisionManager::CollisionType::PUSH,
      DirectX::BoundingBox(GetPosition(), adjustedExtents));
   CollisionManager::Collision eventCollision = CollisionManager::GetInstance().CreateBoxCollision(
      CollisionManager::CollisionPurpose::EVENT_TRIGGER,
      CollisionManager::CollisionType::HIT,
      DirectX::BoundingBox(GetPosition() + EVENT_COLISION_OFFSET, eventAdjustedExtents));
  
   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::GOAL, collision };
   //イベント用の衝突判定パラメータ
   CollisionManager::CollisionParameter eventParam = { this, CollisionManager::ObjectType::GOAL, eventCollision, [this](const CollisionObject* other) { HandleCollisionEvent(other); } };

   //当たり判定の登録
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
   CollisionManager::GetInstance().RegisterObject(eventParam, m_objectStageNumber);
}

/// <summary>
/// ゴール判定の当たり判定通知
/// </summary>
/// <param name="other"></param>
void Goal::HandleCollisionEvent(const CollisionObject* other)
{
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::PLAYER)
   {
      m_goalPlayer = true;
   }
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::FELLOW)
   {
      m_goalFellow = true;
   }
   //プレイヤーと仲間がゴールに辿り着くとゴール
   if (m_goalPlayer && m_goalFellow)
   {
      m_isGoal = true;
      MoveDoar(m_time);
   }
}


/// <summary>
/// ゴールの扉を開く
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Goal::MoveDoar(const float elapsedTime)
{
   ////扉が開ききったら処理を行わない
   //if (m_isOpened) return;

   //ローカル座標で移動量を計算
   const float localMoveAmout = OPNE_GOAL_SPEED * elapsedTime;

   m_doarMoved += localMoveAmout;
   m_doarMoved = yamadalib::Math::Clamp(m_doarMoved, 0.0f, OPEN_LENGTH);
   
   //回転を適用
   const DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
   //ローカル座標の移動ベクトルを作成
   const DirectX::SimpleMath::Vector3 localMovement(m_doarMoved, 0.0f, 0.0f);
   //ワールド座標変換
   const DirectX::SimpleMath::Vector3 worldOfset = DirectX::SimpleMath::Vector3::Transform(localMovement, rotationMatrix);
   //座標更新
   m_doarLeftPosition = GetPosition() + worldOfset;
   m_doarRightPosition = GetPosition() - worldOfset - GOAL_POSITION_OFFSET;
   if (m_doarMoved >= OPEN_LENGTH)
   {
      m_isOpened = true;
   }
}
