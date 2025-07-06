/*
	@file	Light.h
	@brief	ライトクラス
*/
#pragma once
#ifndef LIGHT_DEFINED
#define LIGHT_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"
// 前方宣言
class CommonResources;

class Light : public ObjectBase
{
public:
	//オブジェクトタイプを取得する
	Type GetType() const override { return Type::LIGHT; }
	static IMapObject::Type StaticType() { return IMapObject::Type::LIGHT; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const override { return m_objectStageNumber; }
	//衝突判定を取得する
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
    //コンストラクタ
    Light(CommonResources* resources, const GameParameters::ObjectParameter& param, const DirectX::SimpleMath::Vector3& lightColor, const float& range, const bool& modelDraw);
	//デストラクタ
	~Light() override;

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
   //ライトの設定
   void SettingLight();
private:
    CommonResources* m_commonResources;	// 共通リソース
	CollisionObject* m_collisionObject;		 //衝突判定

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ

	int m_currentStageNumber;						 //現在のステージ番号
	int m_objectStageNumber;						 //オブジェクトが存在するステージ番号

	float m_range;								   //ライトの影響範囲
	DirectX::SimpleMath::Vector3 m_color;		   //ライトのカラー
	bool m_modelDraw;							   //ライトモデルの描画フラグ
};
#endif // LIGHT_DEFINED
