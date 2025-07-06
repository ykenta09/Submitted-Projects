/*
	@file	GimmickObjectBase.h
	@brief	ギミックマップオブジェクトベースクラス
*/
#pragma once
#ifndef GIMMICK_OBJECT_BASE_DEFINED
#define GIMMICK_OBJECT_BASE_DEFINED

#include "Game/Interface/IMapObject.h"
#include "Game/Interface/IGimmick.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"

// 前方宣言
class CommonResources;

class GimmickObjectBase : public IMapObject, IGimmick 
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
   void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) override { m_parameter.rotation = rotation; }

   //スケールを取得する
   DirectX::SimpleMath::Vector3 GetScale() const override { return m_parameter.scale; }
   //スケールを設定する
   void SetScale(const DirectX::SimpleMath::Vector3& scale) override { m_parameter.scale = scale; }

   //オブジェクトタイプを取得する
   Type GetType() const = 0;
   //衝突判定を取得する
   CollisionObject* GetCollisionObject() const = 0;

   //型判別関数
   bool IsGimmick() const { return false; }
   bool IsDevice() const { return false; }

   //処理が終了したか
   bool IsFinished() override { return m_isFinished; }
   //処理終了フラグをリセットする
   void Reset() override { m_isFinished = false; };


   //IGimmickへのアクセサ
   IGimmick* AsGimmick() override { return this; }
   //IDeviceへのアクセサ
   IDevice* AsDevice() override { return nullptr; }
public:
   //コンストラクタ
   GimmickObjectBase(
	  const DirectX::SimpleMath::Vector3& initialPosition,
	  const DirectX::SimpleMath::Quaternion& initialQuaternion,
	  const DirectX::SimpleMath::Vector3& scale);
   ~GimmickObjectBase();

	//純粋仮想関数
	virtual void Initialize() = 0;
	virtual void Update(const float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;
	//当たり判定の初期化
	virtual void InitializeCollision() = 0;

	//ギミックをアクティブ
	void Active() = 0;
	//ギミックを非アクティブ
	void Deactive() = 0;
protected:
   yamadalib::Graphics* m_graphics;		  //グラフィックス
   CommonResources* m_commonResources;	  // 共通リソース
   Params::ObjectParameter m_parameter;   //オブジェクトパラメータ

   bool m_isFinished;				   //ギミック処理の終了フラグ
};
#endif // GIMMICK_OBJECT_BASE_DEFINED
