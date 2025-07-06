/*
	@file	ObjectPool.h
	@brief	オブジェクトプールクラス
*/
#pragma once
#ifndef OBJECT_POOL_DEFIEND
#define OBJECT_POOL_DEFIEND
#include <pch.h>
#include "Game/Collision/CollisionManager.h"
#include "Game/Collision/CollisionObject.h"

class ObjectPool
{
public:
   std::shared_ptr<CollisionObject> GetObject(const CollisionManager::CollisionParameter& param)
   {
	  if (m_pool.empty())
	  {
		 auto obj = m_pool.back();
		 m_pool.pop_back();
		 obj->Reset(param);
		 return obj;
	  }
	  return std::make_shared<CollisionObject>(param);
   }

   void ReturnObject(std::shared_ptr<CollisionObject> obj) { m_pool.push_back(obj); }

private:
   std::vector<std::shared_ptr<CollisionObject>> m_pool;
   
};
#endif // OBJECT_POOL_DEFIEND