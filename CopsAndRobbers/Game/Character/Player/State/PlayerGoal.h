/*
	@file	PlayerGoal.h
	@brief	プレイヤーゴール状態クラス
*/
#pragma once
#ifndef PLAYER_GOAL_DEFINED
#define PLAYER_GOAL_DEFINED

#include "Game/Interface/IState.h"

// 前方宣言
class Player;

class PlayerGoal : public IState
{
public:
   static constexpr DirectX::SimpleMath::Vector3 GOAL_MOVE_POSITION = { 0.0f, 0.0f, 70.0f };	   //ゴール後の移動で向かう座標
   static constexpr float OPEN_TIME = { 3.0f };	   //扉が開ききるまでの時間
   static constexpr float END_STATE_TIME = { 5.0f };	   //ステート終了時間
public:
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_playerVelocity; }
	//移動ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_playerVelocity = vel; }
public:
	//コンストラクタ
    PlayerGoal(Player* player);
	//デストラクタ
	~PlayerGoal();
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
private:
   //移動処理
   void Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime);
private:
	Player* m_player;	//プレイヤー

	DirectX::SimpleMath::Vector3 m_playerVelocity;	//プレイヤーのベクトル
	DirectX::SimpleMath::Vector3 m_goalMovePosition;	//ゴール後の移動で向かう座標

	float m_openTime;	//扉が開ききるまでの時間管理
	float m_endStateTime;	//ステート終了時間
};
#endif // PLAYER_GOAL_DEFINED
