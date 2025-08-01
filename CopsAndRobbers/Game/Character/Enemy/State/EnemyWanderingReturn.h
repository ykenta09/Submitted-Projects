/*
	@file	EnemyWanderingReturn.h
	@brief	敵の徘徊に戻る状態クラス
*/
#ifndef ENEMY_WANDERING_RETURN_DEFINED
#define ENEMY_WANDERING_RETURN_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// 前方宣言
class CommonResources;
class Enemy;

class EnemyWanderingReturn : public IState
{
public:
   static constexpr float SLOW_MOVE_SPEED = { 4.50f };        //敵の遅い移動速度
public:
   //ターゲットの座標を設定する
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_targetPosition = target; }
	//ターゲットの座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_targetPosition; }
public:
	//コンストラクタ
    EnemyWanderingReturn(Enemy* enemy);
	//デストラクタ
	~EnemyWanderingReturn();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();
	//受け取った目標地点に移動する
	void  Move(const float& elapsedTime);
private:
	Enemy* m_enemy;						  //敵
	CommonResources* m_commonResources;	  // 共通リソース
	yamadalib::Graphics* m_graphics;	  //グラフィックス　デバック用

	DirectX::SimpleMath::Vector3 m_targetPosition;	//ターゲットの座標

	float m_lostTime;	//見失った時間
	bool m_isArrived;	//頂点に到達したか
};
#endif // ENEMY_WANDERING_RETURN_DEFINED
