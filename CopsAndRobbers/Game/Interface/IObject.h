/*
	@file	IObject.h
	@brief	オブジェクトインターフェイスクラス
*/
#ifndef IOBJECT_DEFINED
#define IOBJECT_DEFINED
#pragma once
#include "Libraries/Params/GameParameter.h"


class IObject
{
public:
   //現在の座標を取得する
   virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
   //現在の座標を設定する
   virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
   //回転角を取得する
   virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;
   //回転角を設定する
   virtual void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) = 0;
   //スケール取得する
   virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;
   //スケールを設定する
   virtual void  SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;
   //衝突判定を取得する
   virtual CollisionObject* GetCollisionObject() const = 0;

public:
   virtual void Initialize() = 0;			   //初期化処理
   virtual void Update(const float& elapsedTime) = 0;		   //更新処理
   virtual void Render() = 0;		   //描画
   virtual ~IObject() = default;
};
#endif // IOBJECT_DEFINED
