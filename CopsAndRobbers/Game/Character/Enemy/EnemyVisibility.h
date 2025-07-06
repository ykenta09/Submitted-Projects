/*
	@file	EnemyVisibility.h
	@brief	敵の視界クラス
*/
#pragma once
#ifndef ENEMY_VISIBILITY_DEFINED
#define ENEMY_VISIBILITY_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Enemy/Enemy.h"

// 前方宣言
class CommonResources;
class Map;

class EnemyVisibility
{
public: 
   static constexpr float FIELD_OF_VIEW = { 120.0f };		   //視界の広さ
   static constexpr float VISIBILITIY_DISTANCE = { 12.0f };	   //視界の距離
   static constexpr int  SEGMENTS = { 16 };					   //視界の広さ
public:
   //コンストラクタ
   EnemyVisibility(Enemy* enemy);
   //デストラクタ
   ~EnemyVisibility();
   //初期化する
	void Initialize();
	//更新する
	void Update();

	//視界を描画する
	void DrawVisibility(Map* map);
	// 視界の角度計算
	void CalculateAngles(int segmentIndex, float& currentAngleStart, float& currentAngleEnd);
	// 視界の向きと大きさの計算
	DirectX::SimpleMath::Vector2 CalculateDirection();
	// 視界のベクトル計算
	void CalculateVectors(float currentAngleStart, float currentAngleEnd, const DirectX::SimpleMath::Vector2& direction, DirectX::SimpleMath::Vector2& m_vectorV2, DirectX::SimpleMath::Vector2& m_vectorV3);
	void DrawViewLines(const DirectX::SimpleMath::Vector2& m_vectorV2, const DirectX::SimpleMath::Vector2& m_vectorV3);
	// 三角形の頂点設定
	void SetTriangleVertices(Map* map, const DirectX::SimpleMath::Vector2& vectorV2, const DirectX::SimpleMath::Vector2& vectorV3, bool& pointFlag1, bool& pointFlag2);
	// オブジェクトのコーナーを計算
	void CalculateObjectCorners(const DirectX::BoundingBox& object, DirectX::SimpleMath::Vector2 corners[4]);
	// 交差判定と三角形の頂点設定
	void CheckIntersection(const DirectX::SimpleMath::Vector2& start, const DirectX::SimpleMath::Vector2& end, const DirectX::SimpleMath::Vector2& objectV1, const DirectX::SimpleMath::Vector2& objectV2, DirectX::SimpleMath::Vector2& trianglePoint, bool& pointFlag);
	// 視界の扇型を描画
	void DrawSector(const DirectX::XMVECTOR& color);
private:
	yamadalib::Graphics* m_graphics;	//グラフィックス
	Enemy* m_enemy;					   //敵

	bool m_isViewHit;	   //視界内に入っているか
	float m_angle;		   //敵の正面角度
	float m_length;		   //視界の最大距離

	// === 視界の座標 ===
	DirectX::SimpleMath::Vector2 m_vectorV1;	//視界の中心座標
	DirectX::SimpleMath::Vector2 m_vectorV2;	//視界の左端座標
	DirectX::SimpleMath::Vector2 m_vectorV3;	//視界の右端座標

	// === 交点 ===
	DirectX::SimpleMath::Vector2 m_intersectionPoint1;		//左端の障害物との交点
	DirectX::SimpleMath::Vector2 m_intersectionPoint2;		//右端の障害物との交点
};
#endif // ENEMY_VISIBILITY_DEFINED
