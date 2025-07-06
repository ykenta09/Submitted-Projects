/*
	@file	PlayerSprint.h
	@brief	プレイヤー高速移動状態クラス
*/
#pragma once
#ifndef PLAYER_SPRINT_DEFINED
#define PLAYER_SPRINT_DEFINED

#include "Game/Interface/IState.h"

// 前方宣言
class player;

class PlayerSprint : public IState
{
public:
   static constexpr float MAX_MOVEMENT_SPEED = { 18.0f };	   // 最大移動速度
   static constexpr float ACCELERATION_DURATION = { 0.2f };	   // 最大速度に到達するまでの加速時間
   static constexpr float MOVEMENT_DURATION = { 0.5f };		   // 高速移動の移動時間
public:
	//高速状態の確認
	bool GetIsSprint() const { return m_isSprint; }
public:
	//コンストラクタ
	PlayerSprint(Player* player);
	//デストラクタ
	~PlayerSprint();
	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render() ;
	//後処理する
	void Finalize();
	//高速移動の準備をする
	void PreSprint(const DirectX::SimpleMath::Vector3& dir);
private:
   //高速移動
   void Sprint(const float& elapsedTime);
   //移動方向からラジアン角度に変換
   float GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec);
   //床があるかの判定をする
	void IsFloor();
private:
	Player* m_player;	//プレイヤー

	float m_sprintStartTime;	//高速移動の開始時間
	float m_moveAngle;	//移動時の回転角

	DirectX::SimpleMath::Vector3 m_sprintDirection;	//高速移動の方向
	DirectX::SimpleMath::Matrix m_playerMatrix;	//プレイヤーの移動回転行列
	
	DirectX::SimpleMath::Vector3 m_sprintVelocity;	//スプリントベクトル
	DirectX::SimpleMath::Vector3 m_finalVelocity;	//ベクトル
	DirectX::SimpleMath::Vector3 m_previousVector;	//プレイヤーの前回の移動時のベクトル

	bool m_isSprint;	//高速移動のフラグ
};
#endif // PLAYER_SPRINT_DEFINED
