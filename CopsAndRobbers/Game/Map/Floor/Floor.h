/*
	@file	Floor.h
	@brief	床クラス
*/
#pragma once
#ifndef FLOOR_DEFINED
#define FLOOR_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"
// 前方宣言
class CommonResources;

class Floor : public ObjectBase
{
public:
    //オブジェクトのタイプを取得する
	Type GetType() const override { return Type::FLOOR; }
	static IMapObject::Type StaticType() { return IMapObject::Type::FLOOR; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//衝突判定を取得する
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
   //コンストラクタ
    Floor(const int& type, const GameParameters::ObjectParameter& param);
	//デストラクタ
	~Floor() override;
	
	//初期化
	void Initialize() override;
	//更新
	void Update(const float elapsedTime) override;
	//描画
	void Render() override;
	//クワッドを生成関数
	DirectX::VertexPositionTexture MakeVPT(float x, float y, float z, float u, float v);
	//後処理
	void Finalize() override;
	//衝突判定の登録
	void InitializeCollision() override;
private:
	int m_type;										  //モデルタイプ
	int m_currentStageNumber;						 //現在のステージ番号
	int m_objectStageNumber;						 //オブジェクトが存在するステージ番号

	CollisionObject* m_collisionObject;			    //コリジョンオブジェクト

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;				//床モデルのテクスチャ
	DirectX::Model* m_model;				 //モデル
};
#endif // FLOOR_DEFINED
