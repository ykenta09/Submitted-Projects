/*
	@file	LeverDevice.h
	@brief	ボタンデバイスクラス
*/
#pragma once
#ifndef LEVER_DEVICE_DEFINED
#define LEVER_DEVICE_DEFINED

#include"Game/Map/Base/DeviceObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"
#include "Game/Interface/IDevice.h"

class LeverDevice : public DeviceObjectBase
{	
public:
   static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_OFFSET = { 0.0f, 0.0f, 1.0f };				//イベント衝突判定の座標オフセット
   static constexpr DirectX::SimpleMath::Vector3 HANDOLE_LOCAL_OFFSET = { 0.0f, 0.0f, 0.0f };				//イベント衝突判定の座標オフセット
   static constexpr DirectX::SimpleMath::Quaternion HANDLE_ON_ROTATION = { 0.0f, 0.0f, 0.0f, 1.0f };				//イベント衝突判定の座標オフセット
   static constexpr DirectX::SimpleMath::Quaternion HANDLE_OFF_ROTATION = { 0.5f, 0.0f, 0.0f, 1.0f };				//イベント衝突判定の座標オフセット
   static constexpr float HANDLE_ANIMETION_SPEED = { 5.0f };				//ハンドルアニメーションの速度
public:
   //オブジェクトのタイプを取得する
   Type GetType() const override { return Type::LEVER; }
   static IMapObject::Type StaticType() { return IMapObject::Type::LEVER; }
   //オブジェクトのステージ番号を取得する
   int GetObjectStageNumber() const override { return m_objectStageNumber; }

   //衝突判定を取得する
   CollisionObject* GetCollisionObject() const { return m_eventObject; }

   //型判別関数
   bool IsDevice() const override { return true; }
public:
	//コンストラクタ
    LeverDevice(const GameParameters::ObjectParameter& param, const int leverID);
	//デストラクタ
	~LeverDevice() override;

	//初期化する
	void Initialize() override;
	//更新する
	void Update(const float elapsedTime) override;
	//描画する
	void Render() override;
	//後処理をする
	void Finalize() override;

	//衝突判定の初期設定
	void InitializeCollision() override;
	//衝突時のイベント処理
	void HandleCollision(const CollisionObject* other);
	//衝突判定離脱時の処理
	void HandleCollisionExit(const CollisionObject* other);
private:
	//レバーハンドルのアニメーション
	void AnimateHandle(const float elapsedTime, bool leverFlag);
private:
    DirectX::Model* m_leverStandModel;		  //モデル
    DirectX::Model* m_leverHandleModel;		  //モデル

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_standTexture;	//土台モデルのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_handleTexture;	//ハンドルモデルのテクスチャ

	CollisionObject* m_eventObject;	//衝突判定

	DirectX::SimpleMath::Vector3 m_initialPosition;		 //初期座標
	DirectX::SimpleMath::Vector3 m_currentPosition;		 //現在の座標
	DirectX::SimpleMath::Vector3 m_eventPosition;		 //イベントコリジョンの座標

	DirectX::SimpleMath::Quaternion m_leverHandle;		 //レバーハンドルの回転

	int m_leverID;		//レバーのID
	bool m_pushLever;	//レバーを押せるかどうか
	int m_objectStageNumber;//オブジェクトが存在するステージ番号

	float m_handleAnimetionProgress;		 //ハンドルアニメーション進行度
	bool m_isAnimating;						 //ハンドルアニメーションの完了フラグ
};
#endif // LEVER_DEVICE_DEFINED

