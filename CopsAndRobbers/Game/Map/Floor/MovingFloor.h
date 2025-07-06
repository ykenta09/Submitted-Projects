/*
	@file	MovingFloor.h
	@brief	動く床クラス
*/
#pragma once
#ifndef MOVING_FLOOR_DEFINED
#define MOVING_FLOOR_DEFINED
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"

// 前方宣言
class CommonResources;

class MovingFloor : public GimmickObjectBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 0.5f, 2.5f, 0.5f };			//衝突判定のサイズ
   static constexpr float MOVE_SPEED = { 0.4f };			//床が動く速度
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
    MovingFloor(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID);
	//デストラクタ
	~MovingFloor() override;

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

	DirectX::SimpleMath::Vector3 m_currentPosition;	  //現在の座標
	DirectX::SimpleMath::Vector3 m_startPosition;	  //初期位置（非アクティブ時の位置）
	DirectX::SimpleMath::Vector3 m_movePosition;	  // ギミック起動時に移動する目標位置（アクティブ時）
};
#endif // MOVING_FLOOR_DEFINED
