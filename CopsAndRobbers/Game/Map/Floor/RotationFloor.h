/*
	@file	RotationFloor .h
	@brief	回転する床クラス
*/
#pragma once
#ifndef ROTATION_FLOOR_DEFINED
#define ROTATION_FLOOR_DEFINED
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"

// 前方宣言
class CommonResources;

class RotationFloor : public GimmickObjectBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 2.5f, 2.5f, 2.5f };			//衝突判定のサイズ
   static constexpr float ROTATION_Y = DirectX::XMConvertToRadians(90.0f);
   static constexpr float MOVE_SPEED = { 0.6f };			//床が回転する速度
public:
    //オブジェクトのタイプを取得する
	Type GetType() const override { return Type::FLOOR; }
	static IMapObject::Type StaticType() { return IMapObject::Type::FLOOR; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//衝突判定を取得する
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }

	//型判別関数
	bool IsGimmick() const override { return true; }
public:
    //コンストラクタ
    RotationFloor(const int& type, const GameParameters::ObjectParameter& param, const int& gimmickID);
	//デストラクタ
	~RotationFloor() override;

	//初期化する
	void Initialize() override;
	//更新する
	void Update(const float elapsedTime) override;
	//描画する
	void Render() override;
	//後始末する
	void Finalize() override;

	//衝突判定の初期化
	void InitializeCollision() override;

	//ギミックをアクティブ
	void Active() override;
	//ギミックを非アクティブ
	void Deactive() override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ

	int m_type;					   //モデルタイプ
	int m_gimickID;				   //ギミックID
	int m_currentStageNumber;	   //現在のステージ番号
	int m_objectStageNumber;	   //オブジェクトが存在するステージ番号

	float m_time;
	float m_moveProgress;		   //移動処理の進行度

	CollisionObject* m_collisionObject;				  //衝突判定

	DirectX::SimpleMath::Quaternion m_floorRotation;		 //レバーハンドルの回転

	DirectX::SimpleMath::Vector3 m_baseExtents;

	DirectX::SimpleMath::Vector3 m_currentPosition;	  //現在の座標
	DirectX::SimpleMath::Vector3 m_startPosition;	  //初期位置（非アクティブ時の位置）
	DirectX::SimpleMath::Vector3 m_movePosition;	  // ギミック起動時に移動する目標位置（アクティブ時）
};
#endif // ROTATION_FLOOR_DEFINED
