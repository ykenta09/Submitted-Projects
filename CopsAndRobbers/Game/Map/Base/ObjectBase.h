/*
	@file	ObjectBase.h
	@brief	マップオブジェクトベースクラス
*/
#pragma once
#ifndef OBJECT_BASE_DEFINED
#define OBJECT_BASE_DEFINED

#include "Game/Interface/IMapObject.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"

// 前方宣言
class CommonResources;

class ObjectBase : public virtual IMapObject 
{
public:
   //IMapObjectインターフェイス実装---------------------------------------------
   //モデルを取得する
   const DirectX::Model* GetModel() const override { return m_parameter.model; }
   //モデルを設定する
   void SetModel(const DirectX::Model* model) override { m_parameter.model = model; }

   //座標を取得する
   DirectX::SimpleMath::Vector3 GetPosition() const override { return m_parameter.position; }
   //座標を設定する
   void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_parameter.position = pos; }

   //回転を取得する
   DirectX::SimpleMath::Quaternion GetRotation() const override { return m_parameter.rotation; }
   //回転を設定する
   void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) override { m_parameter.rotation *= rotation; }

   //スケールを取得する
   DirectX::SimpleMath::Vector3 GetScale() const override { return m_parameter.scale; }
   //スケールを設定する
   void SetScale(const DirectX::SimpleMath::Vector3& scale) override { m_parameter.scale = scale; }

   //オブジェクトタイプを取得する
   Type GetType() const = 0;

   //オブジェクトのステージ番号を取得する
   int GetObjectStageNumber() const = 0;

   //衝突判定を取得する
   CollisionObject* GetCollisionObject() const = 0;

   //型判別関数
   bool IsGimmick() const { return false; }
   bool IsDevice() const { return false; }

   //IGimmickへのアクセサ
   IGimmick* AsGimmick() override { return nullptr; }
   //IDeviceへのアクセサ
   IDevice* AsDevice() override { return nullptr; }
public:
   //コンストラクタ
   ObjectBase(
	  const DirectX::SimpleMath::Vector3& initialPosition,
	  const DirectX::SimpleMath::Quaternion& initialQuaternion,
	  const DirectX::SimpleMath::Vector3& scale);
   ~ObjectBase();


	//純粋仮想関数
	virtual void Initialize() = 0;
	virtual void Update(const float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;
	//当たり判定の初期化
	virtual void InitializeCollision() = 0;

protected:
   yamadalib::Graphics* m_graphics;		  //グラフィックス
   CommonResources* m_commonResources;	  // 共通リソース
   Params::ObjectParameter m_parameter;   //オブジェクトパラメータ
};
#endif // OBJECT_BASE_DEFINED
