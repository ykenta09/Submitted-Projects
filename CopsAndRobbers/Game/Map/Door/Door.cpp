#include "pch.h"
#include "Game/Map/Door/Door.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/MyLib/BlackBoard.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Player/Player.h"

/// <summary>
///  コンストラクタ
/// </summary>
/// <param name="resources">共通リソース</param>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="nextStage">移動先のステージ番号</param>
/// <param name="backStage">戻るステージ番号</param>
Door::Door(CommonResources* resources, const GameParameters::ObjectParameter& param, const int nextStage, const int backStage)
    : 
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_commonResources(resources),
    m_doarLeftInitialPosition(param.initialPos - DOOR_OFFSET),
    m_doarLeftPosition(param.initialPos),
    m_doarRightInitialPosition(param.initialPos + DOOR_OFFSET),
    m_doarRightPosition(param.initialPos),   
    m_nextStageNumber(nextStage),
    m_backStageNumber(backStage),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_isOpened(false),
    m_time(0.0f),
    m_closeTimer(CLOSE_TIME),
    m_isOpen(false),
    m_isChangeStage(false),
    m_collisionleftObject(nullptr)
{
}


/// <summary>
/// デストラクタ
/// </summary>
Door::~Door()
{
   Finalize();
}

/// <summary>
/// 初期化
/// </summary>
void Door::Initialize()
{
    //モデル情報を取得する
    SetModel(yamadalib::Resources::GetInstance()->GetModel("Door"));
    m_dds = yamadalib::Resources::GetInstance()->GetTexture("Door_DDS");

    //座標を調整する
    UpdatePosition();

    //衝突判定の登録
    InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Door::Update(const float elapsedTime)
{
   //時間を更新する
   m_time = elapsedTime;
   m_closeTimer -= elapsedTime;

   //時間経過したら扉の開くフラグをfalseにする
   if (m_closeTimer <= 0)
   {
      m_closeTimer = CLOSE_TIME;
      m_isOpen = false;
   }
   //フラグfalseになったら扉を閉じる
   if (!m_isOpen)
   {
      MoveDoar(false, m_time);
   }
   //当たり判定の更新
   if (m_collisionleftObject)
   {
      m_collisionleftObject->UpdatePosition(m_doarLeftPosition);
      m_collisionrightObject->UpdatePosition(m_doarRightPosition);
   }
}

/// <summary>
/// 描画する
/// </summary>
void Door::Render()
{      
    //左扉の行列
    DirectX::SimpleMath::Matrix goalDoarLeftMatrix =
       DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
       DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarLeftPosition);
    //右扉の行列
    DirectX::SimpleMath::Matrix goalDoarRightMatrix = 
       DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
       DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
       DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180)) *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarRightPosition);

   // モデルを描画する
    m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), goalDoarLeftMatrix);
    m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), goalDoarRightMatrix);


#ifdef _DEBUG
    //境界ボックスの描画
    m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
       [this](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
       {
          if (!m_collisionleftObject)return;

          m_graphics->DrawBoundingBox(m_collisionleftObject->GetCollisionParameter().collision.box);
          m_graphics->DrawBoundingBox(m_collisionrightObject->GetCollisionParameter().collision.box);
          m_graphics->DrawBoundingBox(m_frontEventObject->GetCollisionParameter().collision.box);
          m_graphics->DrawBoundingBox(m_backEventObject->GetCollisionParameter().collision.box);
       });

#endif // _DEBUG
}

/// <summary>
/// 後処理をする
/// </summary>
void Door::Finalize()
{
   // do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void Door::InitializeCollision()
{
   //回転に応じてサイズ調整
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(COLISION_EXTENTS, GetRotation());
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_COLISION_EXTENTS, GetRotation());
   
   //当たり判定の設定(衝突判定の目的、　衝突判定のタイプ、　生成する衝突判定）
   CollisionManager::Collision leftCollision = 
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::PHYSICAL, CollisionManager::CollisionType::PUSH, DirectX::BoundingBox(m_doarLeftPosition, adjustedExtents));
   CollisionManager::Collision rightCollision = 
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::PHYSICAL, CollisionManager::CollisionType::PUSH, DirectX::BoundingBox(m_doarRightPosition, adjustedExtents));
   CollisionManager::Collision eventCollisionA =
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::EVENT_TRIGGER, CollisionManager::CollisionType::HIT, DirectX::BoundingBox(m_frontEventPosition, eventAdjustedExtents));
   CollisionManager::Collision eventCollisionB =
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::EVENT_TRIGGER, CollisionManager::CollisionType::HIT, DirectX::BoundingBox(m_backEventPosition, eventAdjustedExtents));

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter paramLeft = { this, CollisionManager::ObjectType::DOOR, leftCollision };
   CollisionManager::CollisionParameter paramRight = { this, CollisionManager::ObjectType::DOOR, rightCollision };
   //イベント用の衝突判定パラメータ
   CollisionManager::CollisionParameter eventParamA = { this, CollisionManager::ObjectType::DOOR, eventCollisionA, [this](const CollisionObject* other) { HandleFrontCollisionEvent(other); }};
   CollisionManager::CollisionParameter eventParamB = { this, CollisionManager::ObjectType::DOOR, eventCollisionB, [this](const CollisionObject* other) { HandleBackCollisionEvent(other); } };

   //当たり判定の登録
   m_collisionleftObject = CollisionManager::GetInstance().RegisterObject(paramLeft);
   m_collisionrightObject = CollisionManager::GetInstance().RegisterObject(paramRight);
   m_frontEventObject = CollisionManager::GetInstance().RegisterObject(eventParamA);
   m_backEventObject = CollisionManager::GetInstance().RegisterObject(eventParamB);
}

/// <summary>
/// ドアの正面イベント
/// </summary>
/// <param name="other">衝突相手</param>
void Door::HandleFrontCollisionEvent(const CollisionObject* other)
{
   HandleDoorEvent(other, m_backStageNumber);
}

/// <summary>
/// ドアの後ろ側のイベント
/// </summary>
/// <param name="other">衝突相手</param>
void Door::HandleBackCollisionEvent(const CollisionObject* other)
{
   HandleDoorEvent(other, m_nextStageNumber);
}

/// <summary>
/// ドアのイベント処理
/// </summary>
/// <param name="other">衝突相手</param>
/// <param name="targetStage">変更するステージ番号</param>
void Door::HandleDoorEvent(const CollisionObject* other, int targetStage)
{
   // ドアを開ける処理
   m_isOpen = true;
   MoveDoar(true, m_time);

   void* owner = other->GetOwner();
   if (!owner) return;

   auto objectID = other->GetCollisionParameter().objectID;
   if (objectID == CollisionManager::ObjectType::PLAYER)
   {
      static_cast<Player*>(owner)->SetObjectStageNumber(targetStage);
      LoadStage(targetStage);
   }
   if (objectID == CollisionManager::ObjectType::ENEMY)
   {
      static_cast<Enemy*>(owner)->SetObjectStageNumber(targetStage);
   }
   if (objectID == CollisionManager::ObjectType::FELLOW)
   {
      static_cast<Fellow*>(owner)->SetObjectStageNumber(targetStage);
   }
}

/// <summary>
/// ステージデータをロードする
/// </summary>
/// <param name="stageNumber">ロードするステージ番号</param>
void Door::LoadStage(int stageNumber)
{
   m_commonResources->GetBlackBoard()->SetStageNumber(stageNumber);
   CollisionManager::GetInstance().ChangeStageObjects(stageNumber);
}

/// <summary>
/// ドアの移動処理
/// </summary>
/// <param name="isOpening"></param>
/// <param name="elapsedTime"></param>
void Door::MoveDoar(bool isOpening, const float elapsedTime)
{
   //ドアの移動速度
    const float localMoveAmount = OPEN_GOAL_SPEED * elapsedTime;
    
    // ドアの開閉フラグに応じて移動量を加算または減算
    if (isOpening) 
       m_doarMoved += localMoveAmount;
    else 
       m_doarMoved -= localMoveAmount;

    // クランプ範囲を常に[0.0f, OPEN_LENGTH]に維持
    m_doarMoved = yamadalib::Math::Clamp(m_doarMoved, 0.0f, OPEN_LENGTH);
    
    // ドアの回転行列を取得
    const auto& rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
    // ローカル空間での移動ベクトルを作成
    const DirectX::SimpleMath::Vector3 localMovement(m_doarMoved, 0.0f, 0.0f);
    // ローカル移動ベクトルをワールド空間に変換
    const auto& worldOffset = DirectX::SimpleMath::Vector3::Transform(localMovement, rotationMatrix);
    // 左右のドアの現在座標を更新
    m_doarLeftPosition = m_doarLeftInitialPosition - worldOffset;
    m_doarRightPosition = m_doarRightInitialPosition + worldOffset;

    // 状態フラグ更新（必要に応じて閉じた状態も追跡）
    m_isOpened = (m_doarMoved >= OPEN_LENGTH);
}

/// <summary>
/// 座標を調整する
/// </summary>
void Door::UpdatePosition()
{
   // 回転クォータニオンから回転行列を作成
   DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
   // 回転オフセットを適用
   m_doarLeftInitialPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(-DOOR_OFFSET, rotMat);
   m_doarRightInitialPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(DOOR_OFFSET, rotMat);
   m_frontEventPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(EVENT_OFFSET, rotMat);
   m_backEventPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(-EVENT_OFFSET, rotMat);
}
