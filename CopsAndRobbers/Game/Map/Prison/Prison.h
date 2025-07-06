/*
	@file	Prison.h
	@brief	牢屋クラス
*/
#pragma once
#ifndef PRISON_DEFINED
#define PRISON_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"

class Player;

class Prison : public ObjectBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 PRISON_BASE_EXTENTS = { 3.75f, 5.0f, 2.0f };	   //諸突判定のサイズ
   static constexpr DirectX::SimpleMath::Vector3 EVENT_BASE_EXTENTS = { 2.5f, 0.5f, 2.5f };		   //
   static constexpr DirectX::SimpleMath::Vector3 EVENT_TRIGGER_OFFSET = { 0.0f, 0.0f, 3.0f };
   static constexpr float ROTATION_QUANTIZE_UNIT = 45.0f;
   static constexpr float OPEN_SPEED = 1.5f;			   	//扉の開く速度
   static constexpr float OPEN_LENGTH = 4.0f;			   	//扉が開く距離
public:
	enum class PrisonParts
	{
		ROOM = 0,
		DOAR = 1
	};
public:
	//牢屋の扉が開いているか
	bool GetOpenPrison() { return m_isOpen; }
	//牢屋が開ききったか
	bool GetOpenEndPrison() { return m_isOepnEnd; }

	//オブジェクトのタイプを取得する
	Type GetType() const override { return Type::PRISON; }
	static IMapObject::Type StaticType() { return IMapObject::Type::PRISON; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//衝突判定を取得する
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
    //コンストラクタ
	Prison(const int& type, const GameParameters::ObjectParameter& param);
	//デストラクタ
	~Prison() override;
	//初期化
	void Initialize() override;
	//更新
	void Update(const float elapsedTime) override;
	//描画
	void Render() override;
	//後処理
	void Finalize() override;
    //当たり判定の登録
	void InitializeCollision() override;
	//衝突時のイベント処理
	void HandleCollisionEvent(const CollisionObject* other);

	//扉を動かす
	void MoveDoar(const float elapsedTime);
	//牢屋の鍵を開ける
	void OpenPrison();
private:
	std::unordered_map<PrisonParts, DirectX::Model*> m_prisonModel;	//モデル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomdds;  //テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_doordds;  //テクスチャ


	DirectX::SimpleMath::Vector3 m_doarPosition;	//扉の座標
	CollisionObject* m_collisionObject;			    //扉を開けるイベント

	float m_doarMoved;	//扉の移動量
	bool m_isOpen;		//牢屋が開いている状態か
	bool m_isOepnEnd;	//牢屋が開ききった状態か

	int m_currentStageNumber;						 //現在のステージ番号
	int m_objectStageNumber;						 //オブジェクトが存在するステージ番号
};
#endif // PRISON_DEFINED
