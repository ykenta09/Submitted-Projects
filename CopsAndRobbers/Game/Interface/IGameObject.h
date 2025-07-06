/*
	@file	IGameObject.h
	@brief	ゲームオブジェクトのインターフェイスクラス
*/
#pragma once
#include "pch.h"
#include <Game/Collision/CollisionObject.h>
#ifndef IGAME_OBJECT_DEFINED
#define IGAME_OBJECT_DEFINED

class IGameObject
{
public:
   //モデルを取得する
   virtual const DirectX::Model* GetModel() const = 0;
   //モデルを設定する
   virtual void SetModel(const DirectX::Model* model) = 0;
   //座標を取得する
   virtual const DirectX::SimpleMath::Vector3 GetPosition() const = 0;
   //座標を設定する
   virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
   //回転を取得する
   virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;
   //回転を設定する
   virtual void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) = 0;
   //スケール取得する
   virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;
   //スケールを設定する
   virtual void  SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;
   //衝突判定を取得する
   virtual CollisionObject* GetCollisionObject() const = 0;
};
#endif // IGAME_OBJECT_DEFINED
