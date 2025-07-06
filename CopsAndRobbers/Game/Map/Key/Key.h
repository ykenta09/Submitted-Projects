/*
	@file	Key.h
	@brief	鍵クラス
*/
#pragma once
#ifndef KEY_DEFINED
#define KEY_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"
// 前方宣言
class CommonResources;

class Key : public ObjectBase
{
public:
	//鍵が存在しているか確認
	bool IsExist() const { return m_isExist; }
	Type GetType() const override { return Type::KEY; }
	static IMapObject::Type StaticType() { return IMapObject::Type::KEY; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//存在を消す
	void LostExist() const;

	//衝突判定を取得する
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
   //コンストラクタ
	Key(const GameParameters::ObjectParameter& param);
	//デストラクタ
	~Key() override;

	//初期化
	void Initialize() override;
	//更新
	void Update(const float elapsedTime) override;
	//描画
	void Render() override;
	//後処理
	void Finalize() override;

	//衝突判定の登録
	void InitializeCollision() override;
	//衝突時のイベント処理
	void HandleCollision(const CollisionObject* other);
private:
	bool m_isExist;							  //鍵が存在しているか
	CollisionObject* m_collisionObject;		 //衝突判定

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ

	int m_currentStageNumber;						 //現在のステージ番号
	int m_objectStageNumber;						 //オブジェクトが存在するステージ番号

};
#endif // KEY_DEFINED
