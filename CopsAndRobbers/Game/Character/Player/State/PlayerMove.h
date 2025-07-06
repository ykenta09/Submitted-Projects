/*
	@file	PlayerMove.h
	@brief	プレイヤー移動状態クラス
*/
#pragma once
#ifndef PLAYER_MOVE_DEFINED
#define PLAYER_MOVE_DEFINED

#include "Game/Interface/IState.h"

// 前方宣言
class Player;

class PlayerMove : public IState
{
public:
    //ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//移動ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
public:
	//コンストラクタ
	PlayerMove(Player* player);
	//デストラクタ
	~PlayerMove();
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
   void Move(const float& elapsedTime);
   //移動方向に床があるかの判定と移動ベクトルの調整
   void IsFloor();
private:
	Player* m_player;	//プレイヤー
	DirectX::SimpleMath::Quaternion m_rotationPlayerAngle;	// 回転角
	DirectX::SimpleMath::Quaternion m_rotationPlayerMoveAngle;	// 移動方向の回転角
	DirectX::SimpleMath::Vector3 m_velocity;		  //移動ベクトル
	DirectX::SimpleMath::Vector3 m_finalVelocity;	  //最終的なベクトル

	//プレイヤーの移動回転行列
	DirectX::SimpleMath::Matrix m_playerMoveMatrix;
	DirectX::SimpleMath::Matrix m_playerMatrix;

};
#endif // PLAYER_MOVE_DEFINED
