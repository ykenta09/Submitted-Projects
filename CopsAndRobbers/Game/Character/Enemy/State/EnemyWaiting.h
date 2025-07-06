/*
	@file	EnemyWaiting.h
	@brief	敵の徘徊状態クラス
*/
#ifndef ENEMY_WAITING_DEFINED
#define ENEMY_WAITING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>

// 前方宣言
class CommonResources;
class Enemy;

class EnemyWaiting : public IState
{
public:
	//コンストラクタ
	EnemyWaiting(Enemy* enemy);
	//デストラクタ
	~EnemyWaiting();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();
private:
	//次の移動先方向に回転
	void RotateToNextDestination(const float elapsedTime);
private:
	Enemy* m_enemy;						  //敵
	CommonResources* m_commonResources;	  // 共通リソース
	yamadalib::Graphics* m_graphics;	  //グラフィックス　

	float m_waitTime; 	// 待機時間を保持する変数
	bool m_isRotationComplete;	//回転が終了したかのフラグ
};
#endif // ENEMY_WAITING_DEFINED
