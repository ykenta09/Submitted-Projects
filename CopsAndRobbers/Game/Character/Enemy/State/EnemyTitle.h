/*
	@file	EnemyTitle.h
	@brief	敵のタイトル状態クラス
*/
#pragma once
#ifndef ENEMY_TITLE_DEFINED
#define ENEMY_TITLE_DEFINED

#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;
class Enemy;

class EnemyTitle : public IState
{
public:
   static constexpr DirectX::SimpleMath::Vector3 TARGET_POINT1 = { 8.5f, 0.0f, 10.5f };     //目標地点の座標1
   static constexpr DirectX::SimpleMath::Vector3 TARGET_POINT2 = { -8.5f, 0.0f, 10.5f };    //目標地点の座標2
   static constexpr float TITLE_MOVE_SPEED = { 1.95f };    //タイトルの敵移動速度
public:
   //移動ベクトルを取得する
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//移動ベクトルを設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
public:
	//コンストラクタ
   EnemyTitle(Enemy* enemy);
	//デストラクタ
	~EnemyTitle();
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
    CommonResources* m_commonResources;	  // 共通リソース
	Enemy* m_enemy;						  //敵
	yamadalib::Graphics* m_graphics;	  //グラフィックス

	DirectX::SimpleMath::Vector3 m_velocity;			 //ベクトル
	DirectX::SimpleMath::Vector3 m_currentTargetPoint;	 //現在の目標地点

	// === 移動する目標地点 ===
	DirectX::SimpleMath::Vector3 m_targetPoint1;		 //目標地点1
	DirectX::SimpleMath::Vector3 m_targetPoint2;		 //目標地点2
};
#endif // ENEMY_TITLE_DEFINED
