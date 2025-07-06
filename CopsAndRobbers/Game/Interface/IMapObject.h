/*
	@file	IMapObject.h
	@brief	マップオブジェクトのインターフェイスクラス
*/
#ifndef IMAP_OBJECT_DEFINED
#define IMAP_OBJECT_DEFINED
#pragma once
#include "Game/CommonResources.h"
#include "Game/Collision/CollisionManager.h"
#include "Libraries/Params/GameParameter.h"

class IGimmick;
class IDevice;

class IMapObject
{
public:
    //オブジェクトのタイプ
    enum class Type { WALL, FLOOR, PRISON, KEY, GOAL, DOOR, LEVER, LIGHT };

    //モデルを取得する
    virtual const DirectX::Model* GetModel() const = 0;
    //モデルを設定する
    virtual	void SetModel(const DirectX::Model* model) = 0;

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

	//オブジェクトタイプを取得する
	virtual Type GetType() const = 0;

	//オブジェクトのステージ番号を取得する
	virtual int GetObjectStageNumber() const = 0;

	//衝突判定を取得する:
	virtual CollisionObject* GetCollisionObject() const = 0;

	//型判別関数
	virtual bool IsGimmick() const = 0;
	virtual bool IsDevice() const = 0;

	//IGimmickへのアクセサ
	virtual IGimmick* AsGimmick() = 0;
	//IDeviceへのアクセサ
	virtual IDevice* AsDevice() = 0;
public:
	//仮想デストラクタ
	virtual ~IMapObject() = default;
	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update(const float elapsedTime) = 0;
	//描画
	virtual void Render() = 0;
	//後処理
	virtual void Finalize() = 0;
	//当たり判定の初期化
	virtual void InitializeCollision() = 0;
};
#endif // IMAP_OBJECT_DEFINED
