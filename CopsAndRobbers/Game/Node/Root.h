/*
	@file	Root.h
	@brief	ルートクラス
*/
#pragma once
#ifndef ROOT_DEFINED
#define ROOT_DEFINED
#include <unordered_map>
#include "Game/Interface/IComposite.h"

//シーングラフルートクラス
class Root : public IComposite
{
public:
	//親を取得する
	IComponent* GetParent() { return m_parent; }

public:
	// IComponent インターフェースの純粋仮想関数の実装
	int GetNodeNumber() const override { return m_nodeNumber; }
	int GetPartID() const override { return m_partID; }
	int GetPartNumber() const override { return m_partNumber; }
	DirectX::SimpleMath::Vector3 GetPosition() const override { return m_currentPosition; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) override { m_currentPosition = position; }
	DirectX::SimpleMath::Vector3 GetScale() const  { return DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f); }
	float GetAngle() const override { return m_currentAngle; }
	void SetAngle(const float& angle) override { m_currentAngle = angle; }
	DirectX::Model* GetModel() const override { return nullptr; }
	IComponent* GetLastAttachedNode() const { return m_nodes.back().get(); }

	//キャラクターのポインタを取得する
	Player* AsPlayer() override { return m_player; }
	Enemy* AsEnemy() override { return nullptr; }
	Fellow* AsFellow() override { return nullptr; }

	const std::vector<Enemy*>& GetEnemies() const { return m_enemies; }
	//仲間を取得する
    const std::vector<Fellow*>& GetFellows() const { return m_fellows; }
public:
	//コンストラクタ
	Root(IComponent* parent,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle);
	//デストラクタ
	~Root();
	//初期化する
	void Initialize() override;
	//キーボード通知をする
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys) override;
	//更新する
	void Update(const float& elapsedTime,
		const DirectX::SimpleMath::Vector3& position,
		const float& angle,
		const DirectX::SimpleMath::Vector3& target) override;
	//キャラクターを追加する
	void Attach(std::unique_ptr<IComponent> playerParts) override;
	//キャラクターを削除する
	void Detach(std::unique_ptr<IComponent> playerParts) override;
	//描画する
	void Render() override;
	//後処理をする
	void Finalize() override;
private:
	//親
	IComponent* m_parent;
	//プレイヤー
	Player* m_player;
	//敵
	std::vector<Enemy*> m_enemies;
	//仲間
	std::vector<Fellow*> m_fellows;
	//ノード番号
	int m_nodeNumber;
	//パーツ番号
	int m_partNumber;
	//パーツID
	int m_partID;
	// 初期位置
	DirectX::SimpleMath::Vector3 m_initialPosition;
	float m_initialAngle;
	// 現在の位置
	DirectX::SimpleMath::Vector3 m_currentPosition;
	float m_currentAngle;
	// ターゲットの位置
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//ノード配列
	std::vector<std::unique_ptr<IComponent>> m_nodes;
	//クォータニオン
	DirectX::SimpleMath::Matrix m_worldMatrix;
};
#endif // ROOT_DEFINED

