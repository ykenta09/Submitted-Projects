/*
	@file	MovingWall.h
	@brief	動く壁クラス
*/
#pragma once
#ifndef MOVING_WALL_DEFINED
#define MOVING_WALL_DEFINED
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Game/Collision/CollisionManager.h"
#include "Libraries/yamadaLib/GameParameter.h"

class MovingWall : public GimmickObjectBase
{
public:
   static constexpr float MOVE_SPEED = { 0.4f };			//床が動く速度
public:
	//オブジェクトのタイプを取得する
	Type GetType() const override { return Type::WALL; }
	static IMapObject::Type StaticType() { return IMapObject::Type::WALL; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//衝突判定を取得する
	CollisionObject* GetCollisionObject() const override { return m_collisionObject; }
public:
    //コンストラクタ
    MovingWall(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID);
	//デストラクタ
	~MovingWall() override;
	//初期化
	void Initialize() override;
	//更新
	void Update(const float elapsedTime) override;
	//描画
	void Render() override;
	//後処理
	void Finalize() override;
	//当たり判定の初期化
	void InitializeCollision() override;

	//ギミックをアクティブ
	void Active() override;
	//ギミックを非アクティブ
	void Deactive() override;

private:
	int m_type;							     //モデルタイプ
	int m_currentStageNumber;				 //現在のステージ番号
	int m_objectStageNumber;				 //オブジェクトが存在するステージ番号
	int m_gimickID;							 //ギミックID

	CollisionObject* m_collisionObject;		 //衝突判定
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ

	float m_time;
	float m_moveProgress;		   //移動処理の進行度

	DirectX::SimpleMath::Vector3 m_currentPosition;	  //現在の座標
	DirectX::SimpleMath::Vector3 m_startPosition;	  //初期位置（非アクティブ時の位置）
	DirectX::SimpleMath::Vector3 m_movePosition;	  // ギミック起動時に移動する目標位置（アクティブ時）

};
#endif // MOVING_WALL_DEFINED
