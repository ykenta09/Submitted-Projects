/*
	@file	CollisionObject.h
	@brief	コリジョンオブジェクトクラス
*/
#pragma once
#ifndef COLLISION_OBJECT_DEFINED
#define COLLISION_OBJECT_DEFINED
#include "Game/Collision/CollisionManager.h"
#include "Game/Collider.h"


class CollisionObject
{
public:
   //パラメータの取得
   const CollisionManager::CollisionParameter& GetCollisionParameter() const { return m_param; }
   //所有者を取得する
   void* GetOwner() const { return m_owner; }
   //所有者を設定する
   void SetOwner(void* owner) { m_owner = owner; }

public:
   //コンストラクタ
   CollisionObject(const CollisionManager::CollisionParameter& param);

   //衝突判定のチェックタイプごとに判定を行う
   bool CollidesWith(const CollisionObject* other) const;

   //AABB衝突判定
   bool IntersectsAABB(const CollisionObject* other) const;
   //球体衝突判定
   bool IntersectsSphere(const CollisionObject* other) const;
   //押し出し量計算
   DirectX::SimpleMath::Vector3 CalculatePushBack(const CollisionObject* other) const;

   //衝突時のイベント処理を行う
   void HandleCollision(const CollisionObject* other) const;
   //衝突判定を離脱時イベント処理を行う
   void HandleCollisionExit(const CollisionObject* other) const;
   //当たり判定の更新
   void UpdatePosition(const DirectX::SimpleMath::Vector3 newPosition = DirectX::SimpleMath::Vector3::Zero, const DirectX::SimpleMath::Vector3 extents = DirectX::SimpleMath::Vector3::One);
   //コリジョンオブジェクトをリセットする
   void Reset(const CollisionManager::CollisionParameter& param);
private:
   CollisionManager::CollisionParameter m_param;	  //衝突判定のパラメータ
   AABB m_aabb;										  //AABBの判定
   void* m_owner = nullptr;							  //所有者
   DirectX::SimpleMath::Vector3 m_baseExtents;		  //初期境界ボックスサイズ
};
#endif // COLLISION_OBJECT_DEFINED
