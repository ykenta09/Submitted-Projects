/*
	@file	PlayerTitle.h
	@brief	プレイヤータイトル状態クラス
*/
#pragma once
#ifndef PLAYER_TITLE_DEFINED
#define PLAYER_TITLE_DEFINED

#include "Game/Interface/IState.h"

// 前方宣言
class Player;

class PlayerTitle : public IState
{
public:
   static constexpr DirectX::SimpleMath::Vector3 TARGET_POINT = { 8.5f, 0.0f, 10.5f };			//目標地点
   static constexpr float SPEED_CORRECTION = { 0.3f };			//目標地点
public:
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_playerVelocity; }
	//移動ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_playerVelocity = vel; }
public:
	//コンストラクタ
    PlayerTitle(Player* player);
	//デストラクタ
	~PlayerTitle();
	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render() ;
	//後処理する
	void Finalize();
	//移動ベクトルから角度を求める
	float GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec);
	//目標地点に到達したかどうか
	bool IsTargetPoint();
private:
   //移動処理
   void Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime);
private:
	Player* m_player;	//プレイヤー

	DirectX::SimpleMath::Vector3 m_playerVelocity;	//プレイヤーのベクトル
	DirectX::SimpleMath::Vector3 m_currentTargetPoint;	//現在の目標地点
	DirectX::SimpleMath::Vector3 m_targetPoint1;	//目標地点1
	DirectX::SimpleMath::Vector3 m_targetPoint2;	//目標地点2
};
#endif // PLAYER_TITLE_DEFINED
