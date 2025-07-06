/*
	@file	Goal.h
	@brief	影クラス
*/
#pragma once
#ifndef GOAL_DEFINED
#define GOAL_DEFINED

#include"Game/Map/Base/ObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"

//goalクラス
class Goal : public ObjectBase
{	
public:
	enum class GoalParts : int
	{
		FRAM = 0,
		DOOR = 1,
	};
public:
    static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 7.0f, 5.0f, 0.7f };			//衝突判定のサイズ
	static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_EXTENTS = { 2.25f, 1.0f, 3.0f };	//イベント衝突判定のサイズ
	static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_OFFSET = { 0.0f, 0.0f, -3.5f };	//イベント衝突判定の位置補正
	static constexpr DirectX::SimpleMath::Vector3 GOAL_POSITION_OFFSET = { 4.5f, 0.0f, 0.0f };	   //扉の座標補正
	static constexpr float OPNE_GOAL_SPEED = 1.2f;			//扉の開く速度
    static constexpr float OPEN_LENGTH = 3.5f;			   	//扉の開く距離
public:
   //ゴールしているか取得
   bool IsGaol() const { return m_isGoal; }

   //オブジェクトのタイプを取得する
   Type GetType() const override { return Type::GOAL; }
   static IMapObject::Type StaticType() { return IMapObject::Type::GOAL; }

   //オブジェクトのステージ番号を取得する
   int GetObjectStageNumber() const override { return m_objectStageNumber; }

   //衝突判定を取得する
   CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
	//コンストラクタ
	Goal(const GameParameters::ObjectParameter& param);
	//デストラクタ
	~Goal() override;

	//初期化する
	void Initialize() override;
	//更新する
	void Update(const float elapsedTime) override;
	//描画する
	void Render() override;
	//後処理をする
	void Finalize() override;
	//衝突判定を登録する
	void InitializeCollision() override;
	//衝突時のイベント処理
	void HandleCollisionEvent(const CollisionObject* other);

	//扉を動かす
	void MoveDoar(const float elapsedTime);

private:
	std::unordered_map<GoalParts, DirectX::Model*> m_goalModel;	//モデル
	std::unordered_map<GoalParts, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_dds;  //テクスチャ


	DirectX::SimpleMath::Vector3 m_doarLeftPosition;	//扉の座標
	DirectX::SimpleMath::Vector3 m_doarRightPosition;
	float m_doarMoved;	//扉が開いた距離
	bool m_isOpened;	//扉が開ききったか

	CollisionObject* m_collisionObject;	//衝突判定

	//プレイヤーと仲間のフラグ
	bool m_goalPlayer;
	bool m_goalFellow;
	bool m_isGoal;		   //ゴールフラグ

	float m_time;		//経過時間

	int m_currentStageNumber;						 //現在のステージ番号
	int m_objectStageNumber;						 //オブジェクトが存在するステージ番号
};
#endif // GOAL_DEFINED

