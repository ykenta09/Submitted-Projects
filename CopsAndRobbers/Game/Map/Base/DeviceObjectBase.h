/*
	@file	DeviceObjectBase.h
	@brief	デバイスマップオブジェクトベースクラス
*/
#pragma once
#ifndef DEVICE_OBJECT_BASE_DEFINED
#define DEVICE_OBJECT_BASE_DEFINED

#include "Game/Interface/IMapObject.h"
#include "Game/Interface/IDevice.h"
#include "Game/Interface/IObserver.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"

// 前方宣言
class CommonResources;

class DeviceObjectBase : public IMapObject, public IDevice, public IObserver
{
public:
   static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_EXTENTS = { 1.5f, 1.0f, 1.0f };				//イベント衝突判定のサイズ
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

   //IGimmickへのアクセサ
   IGimmick* AsGimmick() override { return nullptr; }
   //IDeviceへのアクセサ
   IDevice* AsDevice() override { return this; }

   //装置の操作範囲にいるか取得する
   bool GetInsideControlZone() const { return m_isInsideControlZone; }
   //装置の操作範囲にいるかを設定する
   void SetInsideControlZone(bool flag) { m_isInsideControlZone = flag; }

protected:
   //装置が起動中かどうか
   bool IsTriggered() const { return m_isTriggered; }
   //装置がONかOFFか
   bool IsEnabled() const { return m_isEnabled;  }

public:
   //コンストラクタ
   DeviceObjectBase(
	  const DirectX::SimpleMath::Vector3& initialPosition,
	  const DirectX::SimpleMath::Quaternion& initialQuaternion,
	  const DirectX::SimpleMath::Vector3& scale);
   //デストラクタ
   ~DeviceObjectBase();

	//純粋仮想関数
	virtual void Initialize() = 0;
	virtual void Update(const float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

	//衝突判定の登録
	void InitializeCollision() = 0;

	//キーボード入力処理
	void OnKeyPressed(const DirectX::Keyboard::Keys& key);

	//装置にギミックの登録
	void RegisterGimmick(IGimmick* gimmick) override;
	//ギミックを起動する
	void Trigger(bool flag) override;

   //登録されたギミック処理が終了したか
   void AreAllGimmicksFinished();
   //登録したギミックの終了フラグをリセットする
   void GimmicksReset();
protected:
   yamadalib::Graphics* m_graphics;		  //グラフィックス
   CommonResources* m_commonResources;	  // 共通リソース
private:
   Params::ObjectParameter m_parameter;   //オブジェクトパラメータ
   std::vector<IGimmick*> m_targets;	 //ギミックリスト
   bool m_isTriggered;				//装置が起動中かどうか
   bool m_isEnabled;				//装置がONかOFFか
   bool m_isInsideControlZone;		//装置の操作範囲にいるか
};
#endif // DEVICE_OBJECT_BASE_DEFINED
