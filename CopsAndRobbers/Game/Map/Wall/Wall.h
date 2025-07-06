/*
	@file	Wall.h
	@brief	壁クラス
*/
#pragma once
#ifndef WALL_DEFINED
#define WALL_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Game/Collision/CollisionManager.h"
#include "Libraries/yamadaLib/GameParameter.h"

class Wall : public ObjectBase
{
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
    Wall(const int& type, const GameParameters::ObjectParameter& param);
	//デストラクタ
	~Wall() override;
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
private:
	int m_type;							     //モデルタイプ
	int m_currentStageNumber;				 //現在のステージ番号
	int m_objectStageNumber;				 //オブジェクトが存在するステージ番号
	CollisionObject* m_collisionObject;		 //衝突判定
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ
};
#endif // WALL_DEFINED
