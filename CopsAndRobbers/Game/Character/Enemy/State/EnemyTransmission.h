/*
	@file	EnemyTransmission .h
	@brief	敵の伝達状態クラス
*/
#ifndef ENEMY_TRANSMISSION_DEFINED
#define ENEMY_TRANSMISSION_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// 前方宣言
class CommonResources;
class GraphScene;
class Enemy;


class EnemyTransmission : public IState
{
public:
   //敵の伝達距離
   static const float TRANSMISSION_DISTANCE;
public:
public:
	//コンストラクタ
    EnemyTransmission(Enemy* enemy);
	//デストラクタ
	~EnemyTransmission();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();

	//伝達する
	void Transmission();
private:
	//敵
	Enemy* m_enemy;
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフデータ
	GraphScene* m_graphScene;
	//グラフィックス　デバック用
	yamadalib::Graphics* m_graphics;
	//プレイヤーの回転行列
	DirectX::SimpleMath::Matrix m_enemyMatrix;

	//ターゲットの座標
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//境界ボックス
	DirectX::BoundingBox m_boundingBox;
};
#endif // ENEMY_TRANSMISSION_DEFINED
