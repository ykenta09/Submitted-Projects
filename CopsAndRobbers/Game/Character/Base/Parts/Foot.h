/*
	@file	Foot.h
	@brief	足クラス
*/
#pragma once
#ifndef FOOT_DEFINED
#define FOOT_DEFINED
#include "Game/Interface/ILeaf.h"

// 前方宣言
class CommonResources;

//足クラスを定義する
class Foot : public ILeaf
{
public:
   static constexpr float FOOT_SWING_INIT_DEG = 3.0f;    // 足振りの初期角度（度）
   static constexpr float FOOT_SWING_LIMIT_DEG = 40.0f;    // 足振りの最大角度（度）
   static constexpr float FOOT_SWING_DECAY = 0.9f;    // 足振りの減衰率
   static constexpr float FOOT_ROT_OFFSET_Y = 0.0f;    // 初期Y回転（度）
public:
	//親を取得する
	IComponent* GetParent() const { return m_parent; };
	//ノード番号を取得する
	int GetNodeNumber() const { return m_nodeNumber; }
	//パーツIDを取得する
	int GetPartID() const { return m_partID; }
	//パーツ番号を取得する
	int GetPartNumber() const { return m_partNumber; }
	//現在の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const override { return m_currentPosition; }
	//現在の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_currentPosition = pos; }
	//スケール取得する
	DirectX::SimpleMath::Vector3 GetScale() const override{ return m_scale; }
	//モデルを取得する
	DirectX::Model* GetModel() const override { return m_model; }
	//モデルを設定する
	void SetPartsModel(DirectX::Model* model) { m_model = model; }
	//現在の回転角を取得する
	float GetAngle() const { return m_currentAngle; }
	//現在の回転角を設定する
	void SetAngle(const float& angle) { m_currentAngle = angle; }

	//キャラクターのポインタを取得する
	Player* AsPlayer() override { return nullptr; }
	Enemy* AsEnemy() override { return nullptr; }
	Fellow* AsFellow() override { return nullptr; }

	//押し出し処理
	void SetPushPos(const DirectX::SimpleMath::Vector3& push) { m_currentPosition += push; }
public:
	//コンストラクタ
	Foot(
		IComponent* parent,
		CommonResources* resources,
		const CharacterBase::CharacterResources& model,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale,
		CharacterBase::PartID partId);
	//デストラクタ
	~Foot();
	//初期化する
	void Initialize();
	//更新する
	inline void Update(const float& elapsedTime,
		const DirectX::SimpleMath::Vector3& position,
		const float& angle,
		const DirectX::SimpleMath::Vector3& target);
	//描画する
	inline void Render();
	//後処理する
	void Finalize();

	//キーボードの通知をする
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys);
	//移動中
	void Move();
	
private:
	yamadalib::Graphics* m_graphics;	//グラフィックス
	IComponent* m_parent;			    //親
	CommonResources* m_commonResources;	// 共通リソース

	DirectX::Model* m_model;	// モデル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_modelDDS;  //モデルのテクスチャ

	DirectX::SimpleMath::Vector3 m_initialPosition;	// 初期位置
	DirectX::SimpleMath::Vector3 m_currentPosition;	// 現在の位置
	float m_initialAngle;	// 初期回転角
	float m_currentAngle;	// 現在の回転角

	int m_nodeNumber;	// ノード番号
	int m_partNumber;	// 部品番号
	int m_partID;		// 部品ID

	DirectX::SimpleMath::Matrix m_playerMatrix;	//プレイヤーの回転行列
	DirectX::SimpleMath::Matrix m_worldMatrix;	//ワールド行列

	DirectX::SimpleMath::Vector3 m_scale;	//スケール

	float m_rotaY;
	float m_direction; // 方向変数をメンバーとして追加

	bool m_isMoving;	//移動中か
};
#endif // FOOT_DEFINED
