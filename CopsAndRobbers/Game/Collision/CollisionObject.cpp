/*
	@file	CollisionManager.cpp
	@brief	コリジョンオブジェクトクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Collision/CollisionManager.h"
#include "CollisionObject.h"
#include "Libraries/yamadaLib/Math.h"


/// <summary>
///  コンストラクタ
/// </summary>
/// <param name="param">衝突判定のパラメータ</param>
CollisionObject::CollisionObject(const CollisionManager::CollisionParameter& param)
   :
   m_param(param),
   m_baseExtents(param.collision.box.Extents),
   m_aabb{},
   m_owner(param.owner)
{
}

bool CollisionObject::CollidesWith(const CollisionObject* other) const
{
   //衝突判定のチェックタイプを取得
   auto myType = m_param.collision.checkType;
   auto otherType = other->m_param.collision.checkType;

   //両方球体のみの判定
   if (myType == CollisionManager::CollisionCheckType::SphereOnly && otherType == CollisionManager::CollisionCheckType::SphereOnly)
   {
	  return IntersectsSphere(other);
   }

   //両方ボックスのみの判定
   if (myType == CollisionManager::CollisionCheckType::BoxOnly && otherType == CollisionManager::CollisionCheckType::BoxOnly)
   {
	  return IntersectsAABB(other);
   }

   //球体とボックスの判定
   if (myType == CollisionManager::CollisionCheckType::SphereAndBox || otherType == CollisionManager::CollisionCheckType::SphereAndBox)
   {
	  if (IntersectsSphere(other))
	  {
		 return IntersectsAABB(other);
	  }
   }

   return false;
}


/// <summary>
/// AABB衝突判定
/// </summary>
/// <param name="other">衝突判定を行う対象</param>
/// <returns></returns>
bool CollisionObject::IntersectsAABB(const CollisionObject* other) const
{
   //AABBの衝突判定を行う
   return m_param.collision.box.Intersects(other->m_param.collision.box);
}

/// <summary>
/// 球体の衝突判定
/// </summary>
/// <param name="other">衝突判定を行う対象</param>
/// <returns></returns>
bool CollisionObject::IntersectsSphere(const CollisionObject* other) const
{
   //球体の衝突判定を行う
   return m_param.collision.sphere.Intersects(other->m_param.collision.sphere);
}

/// <summary>
/// 押し出し量を計算する
/// </summary>
/// <param name="other">めり込み量を求めるコリジョンオブジェクト</param>
/// <returns></returns>
DirectX::SimpleMath::Vector3 CollisionObject::CalculatePushBack(const CollisionObject* other) const
{
   //押し出し量の計算を行う
   return m_aabb.CalculatePushBackVector(m_param.collision.box, other->m_param.collision.box);
}


/// <summary>
/// 衝突時のイベント処理を行う
/// </summary>
/// <param name="other">衝突した対象</param>
void CollisionObject::HandleCollision(const CollisionObject* other) const 
{
   //衝突イベントがない場合は行わない
   if (m_param.NotifyEvent)
   {
	  m_param.NotifyEvent(other);
   }
}

/// <summary>
/// 衝突判定を離脱時イベント処理を行う
/// </summary>
/// <param name="other">離脱した対象</param>
void CollisionObject::HandleCollisionExit(const CollisionObject* other) const
{
   //衝突判定離脱イベントがない場合は行わない
   if (m_param.NotifyExitEvent)
   {
	  m_param.NotifyExitEvent(other);
   }
}

/// <summary>
/// 衝突判定の更新
/// </summary>
/// <param name="newPosition">衝突判定の更新座標</param>
void CollisionObject::UpdatePosition(const DirectX::SimpleMath::Vector3 newPosition, const DirectX::SimpleMath::Vector3 extents)
{
   //コリジョンオブジェクトのパラメータを更新
   m_param.collision.box.Center = newPosition;
   m_param.collision.sphere.Center = newPosition;
   //デフォルト引数のままなら処理をしない
   if (extents == DirectX::SimpleMath::Vector3::One) return;
   m_param.collision.box.Extents = extents;
}


/// <summary>
/// コリジョンオブジェクトのパラメータをリセットする
/// </summary>
/// <param name="param">衝突判定のパラメータ</param>
void CollisionObject::Reset(const CollisionManager::CollisionParameter& param)
{
   //パラメータをセット
   m_param = param;
   m_aabb = {};

   //境界ボックスと境界球の座標をセット
   m_param.collision.box.Center = param.collision.box.Center;
   m_param.collision.sphere.Center = param.collision.sphere.Center;
}


