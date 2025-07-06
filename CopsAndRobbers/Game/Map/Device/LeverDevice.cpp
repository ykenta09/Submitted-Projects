#include "pch.h"
#include "Game/Map/Device/LeverDevice.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Astar/AStarGraph.h"

/// <summary>
///  コンストラクタ
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="leverID">レバーのID</param>
LeverDevice::LeverDevice(const GameParameters::ObjectParameter& param, const int leverID)
    : 
    DeviceObjectBase(param.initialPos, param.initialRot, param.scale),
    m_initialPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_leverHandle(HANDLE_ON_ROTATION),
    m_eventPosition(param.initialPos),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_leverID(leverID),
    m_isAnimating(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
LeverDevice::~LeverDevice()
{
   Finalize();
}

/// <summary>
///  初期化する
/// </summary>
void LeverDevice::Initialize()
{
    //モデル情報を取得する
    m_leverStandModel = yamadalib::Resources::GetInstance()->GetModel("Lever01Stand");
    m_leverHandleModel = yamadalib::Resources::GetInstance()->GetModel("Lever01Handle");
	m_standTexture = yamadalib::Resources::GetInstance()->GetTexture("Lever01Stand_DDS");
	m_handleTexture = yamadalib::Resources::GetInstance()->GetTexture("Lever01Handle_DDS");

    //衝突判定の登録
    InitializeCollision();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void LeverDevice::Update(const float elapsedTime)
{
   //装置が起動中のみ処理する
   if (IsTriggered())
   {
	  Trigger(IsEnabled());
	  AnimateHandle(elapsedTime, IsEnabled());
	  //ギミックが起動し終えたかの確認
	  AreAllGimmicksFinished();
	  //起動中はグラフ反映していない物に変更
	  GameManager::GetInstance()->GetGraph()->InitializeDynamicGraph();
	  //全てのギミックが起動し終えてたらアニメーションをリセットする
	  if (!IsTriggered())
	  {
		 //グラフにギミックの変化を反映する
		 GameManager::GetInstance()->GetGraph()->UpdateDynamicObjects();
		 m_isAnimating = false;
	  }
   }
}

/// <summary>
/// 描画する
/// </summary>
void LeverDevice::Render()
{
   // ワールド行列を更新する
   DirectX::SimpleMath::Matrix leverMatrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   // ハンドルのローカル変換（回転＋オフセット）
   DirectX::SimpleMath::Matrix handleLocalMatrix =
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_leverHandle) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(HANDOLE_LOCAL_OFFSET);
   DirectX::SimpleMath::Matrix handleWorldMatrix = handleLocalMatrix * leverMatrix;

   //モデルを描画する
   m_graphics->GetCustomModelRenderer()->Draw(*m_leverStandModel, m_standTexture.Get(), leverMatrix);
   m_graphics->GetCustomModelRenderer()->Draw(*m_leverHandleModel, m_handleTexture.Get(), handleWorldMatrix);
}

/// <summary>
/// 後処理をする
/// </summary>
void LeverDevice::Finalize()
{
   // do nothing.
}


/// <summary>
/// 衝突判定の登録
/// </summary>
void LeverDevice::InitializeCollision()
{
   //回転に応じて境界ボックスのサイスを調整
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_COLISION_EXTENTS, GetRotation());
   DirectX::SimpleMath::Vector3 rotatedOffset = DirectX::SimpleMath::Vector3::Transform(EVENT_COLISION_OFFSET, GetRotation());
   m_eventPosition += rotatedOffset;
   //イベント衝突判定を生成
   CollisionManager::Collision eventCollision =
	  CollisionManager::GetInstance().CreateBoxCollision(
		 CollisionManager::CollisionPurpose::EVENT_TRIGGER,                 //衝突判定の目的 
		 CollisionManager::CollisionType::HIT,                              //衝突処理
		 DirectX::BoundingBox(m_eventPosition, eventAdjustedExtents));      //ボックス

   //当たり判定のパラメータを設定(オブジェクトのポインタ、オブジェクトタイプ、　当たり判定、 衝突時の処理関数)
   CollisionManager::CollisionParameter eventParam = {
	  this, 
	  CollisionManager::ObjectType::LEVER, 
	  eventCollision,
	  [this](const CollisionObject* other) { HandleCollision(other); }, 
	  [this](const CollisionObject* other) { HandleCollisionExit(other); } 
   };

   //当たり判定の登録
   m_eventObject = CollisionManager::GetInstance().RegisterObject(eventParam, m_objectStageNumber);
}

/// <summary>
/// 衝突時のイベント処理
/// </summary>
/// <param name="other">衝突対象のコリジョンオブジェクト</param>
void LeverDevice::HandleCollision(const CollisionObject* other)
{
   UNREFERENCED_PARAMETER(other);
   //装置を操作可能にする
   SetInsideControlZone(true);
}

/// <summary>
/// 衝突判定離脱時の処理
/// </summary>
/// <param name="other"></param>
void LeverDevice::HandleCollisionExit(const CollisionObject* other)
{
   UNREFERENCED_PARAMETER(other);
   //装置を操作不能にする
   SetInsideControlZone(false);
}



/// <summary>
/// レバーハンドルのアニメーション
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="leverFlag">レバーのON・OFFフラグ</param>
void LeverDevice::AnimateHandle(const float elapsedTime, bool leverFlag)
{
   //アニメーションが完了したら処理をしない
   if (m_isAnimating) return;

   //アニメーションの進行度の更新
   m_handleAnimetionProgress += HANDLE_ANIMETION_SPEED * elapsedTime;
   m_handleAnimetionProgress = yamadalib::Math::Clamp(m_handleAnimetionProgress, 0.0f, 1.0f);

   //ハンドルのフラグごとにハンドルを動かす
   if (leverFlag) //レバーON
   {
	  m_leverHandle = DirectX::SimpleMath::Quaternion::Slerp(HANDLE_ON_ROTATION, HANDLE_OFF_ROTATION, m_handleAnimetionProgress);
   }
   else  //レバーOFF
   {
	  m_leverHandle = DirectX::SimpleMath::Quaternion::Slerp(HANDLE_OFF_ROTATION, HANDLE_ON_ROTATION, m_handleAnimetionProgress);
   }

   //アニメーション進行度が1.0fになったらアニメーションを終了する
   if (m_handleAnimetionProgress >= 1.0f)
   {
	  m_handleAnimetionProgress = 0.0f;
	  m_isAnimating = true;
   }
}