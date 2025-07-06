/*
	@file	Door.h
	@brief	ドアクラス
*/
#pragma once
#ifndef DOOR_DEFINED
#define DOOR_DEFINED

#include"Game/Map/Base/ObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"

class Door : public ObjectBase
{	
public:
   static constexpr DirectX::SimpleMath::Vector3 DOOR_OFFSET = { 1.48f, 0.0f, 0.0f };					//ドアの初期オフセット
   static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 1.5f, 2.0f, 0.7f };				//衝突判定のサイズ
   static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_EXTENTS = { 3.0f, 1.0f, 1.0f };			//イベント衝突判定のサイズ
   static constexpr DirectX::SimpleMath::Vector3 EVENT_OFFSET = { 0.0f, 0.0f, 1.0f };					//イベント衝突判定のオフセット

   static constexpr float OPEN_GOAL_SPEED = { 1.8f };				//扉の開く速度
   static constexpr float OPEN_LENGTH = { 3.5f };					//扉の開く距離
   static constexpr float CLOSE_TIME = { 4.5f };					//扉が閉まるまでの時間
public:
   //ドアが開いているか
   bool IsOpen() const { return m_isOpen; }
   //ステージを切り替えるか
   bool IsChange() const { return m_isChangeStage; }
   void SetStageChange(bool flag) { m_isChangeStage = flag; }

   //オブジェクトのタイプを取得する
   Type GetType() const override { return Type::DOOR; }
   static IMapObject::Type StaticType() { return IMapObject::Type:: DOOR; }

   //オブジェクトのステージ番号を取得する
   int GetObjectStageNumber() const override { return m_objectStageNumber; }
   //衝突判定を取得する
   CollisionObject* GetCollisionObject() const { return m_collisionleftObject; }

   //移動先のステージ番号を取得する
   const int GetNextStageNumber() const { return m_nextStageNumber; }
   //元のステージの番号を取得する
   const int GetBackStageNumber() const { return m_backStageNumber; }
public:
	//コンストラクタ
    Door(CommonResources* resources, const GameParameters::ObjectParameter& param, const int nextStage, const int backStage);
	//デストラクタ
	~Door() override;
	//初期化する
	void Initialize() override;
	//更新する
	void Update(const float elapsedTime) override;
	//描画する
	void Render() override;
	//後処理をする
	void Finalize() override;
	//衝突判定の登録
	void InitializeCollision() override;

	//ドアの正面のイベント
	void HandleFrontCollisionEvent(const CollisionObject* other);
	//ドアの後ろ側のイベント
	void HandleBackCollisionEvent(const CollisionObject* other);
	//ドアイベント
	void HandleDoorEvent(const CollisionObject* other, int targetStage);
	//ステージデータをロードする
	void LoadStage(int stageNumber);
	//扉を動かす
	void MoveDoar(bool isOpening, const float elapsedTime);
	//ドアの座標を調整する
	void UpdatePosition();
private:
    CommonResources* m_commonResources;	// 共通リソース

	DirectX::Model* m_doorModel;	//モデル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ

	//扉の座標
	DirectX::SimpleMath::Vector3 m_doarLeftInitialPosition;		  //左扉初期座標
	DirectX::SimpleMath::Vector3 m_doarLeftPosition;			  //左扉の現在の座標
	DirectX::SimpleMath::Vector3 m_doarRightInitialPosition;	  //右扉初期座標
	DirectX::SimpleMath::Vector3 m_doarRightPosition;			  //右扉の現在の座標
	DirectX::SimpleMath::Vector3 m_frontEventPosition;			  //正面イベント衝突判定の座標
	DirectX::SimpleMath::Vector3 m_backEventPosition;			  //裏側イベント衝突判定の座標

	float m_doarMoved;	//扉が開いた距離
	bool m_isOpened;	//扉が開ききったか

	//衝突判定
	CollisionObject* m_collisionleftObject;		   //左扉の衝突判定
	CollisionObject* m_collisionrightObject;	   //右扉の衝突判定
	CollisionObject* m_frontEventObject;		   //正面イベント衝突判定
	CollisionObject* m_backEventObject;			   //裏側イベント衝突判定
	
	bool m_isOpen;			  //扉の開くフラグ
	bool m_isChangeStage;	  //ステージを切り替えるフラグ

	float m_closeTimer;	//扉が閉まるまでの時間
	float m_time;		//経過時間

	int m_nextStageNumber;	//移動先のステージ番号
	int m_backStageNumber;	//戻ってくるステージの番号
	int m_objectStageNumber;//オブジェクトが存在するステージ番号

};
#endif // DOOR_DEFINED

