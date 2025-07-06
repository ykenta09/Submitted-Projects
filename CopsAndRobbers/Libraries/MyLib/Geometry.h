#pragma once
#include "Common.h"
#include "SimpleMath.h"

inline DirectX::SimpleMath::Vector2 Vector3ToVector2(const DirectX::SimpleMath::Vector3& vector)
{
   return DirectX::SimpleMath::Vector2(vector.x, vector.z);
}

// 光線と平面の交差判定を行う
inline bool IntersectRayPlane(
	const DirectX::SimpleMath::Ray& ray,
	const DirectX::SimpleMath::Plane& plane,
	DirectX::SimpleMath::Vector3* const intersection
)
{
	/*
	無限平面と交点の式　t = normal * (position - origin) / (normal * direction)
	交点 origin + direction * t

	光線: P = P0 + tV
	平面: P ・N + d = 0
	(P0 + tV)・N + d = 0
	t = -(P0・N + d) / (V・N)
	P = P0 + tV
	*/

	// 平面法線ベクトルと光線の内積を計算する
	float d = plane.Normal().Dot(ray.direction);
	// 光線と平面が平行の場合は交点は存在しない
	if (d == 0.0f)
		return false;
	// 平面の法線ベクトルと光線の原点の間の内積を計算する
	float t = (plane.D() - plane.Normal().Dot(ray.position)) / d;
	// 交差点を計算する
	*intersection = ray.position + ray.direction * t;
	// 光線と平面が交差している
	return true;
}

// 点が円の内部にあるかどうかを判定する
inline bool InsideCircle(const int& radius, const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& point)
{
	// 2点間の距離を計算する
	float distance = (center.x - point.x) * (center.x - point.x) + (center.z - point.z) * (center.z - point.z);
	// 距離と半径の二乗と比較する
	if (distance <= radius * radius)
		return true;
	else
		return false;
}

// 円と線分の交差判定を行う
inline bool IntersectCircleLine(
	const DirectX::SimpleMath::Vector3& center,					// 中心点
	const float& radius,																// 半径
	const DirectX::SimpleMath::Vector3& start,						// 線分の開始
	const DirectX::SimpleMath::Vector3& end							// 線分の終了
)
{
	// 線分の開始地点から円の中心へのベクトル
	DirectX::SimpleMath::Vector2 startToCenter = Vector3ToVector2(center - start);
	// 線分の終了地点から円の中心へのベクトル
	DirectX::SimpleMath::Vector2 endToCenter = Vector3ToVector2(center - end);
	// 線分の開始位置から終了位置へのベクトル
	DirectX::SimpleMath::Vector2 startToEnd = Vector3ToVector2(end - start);
	// 円の中心から線分までの距離を計算する
	float length = Cross2D(startToCenter, startToEnd) / startToEnd.Length();

	if (abs(length) <= radius)
	{
		if (Dot2D(startToCenter, startToEnd) * Dot2D(endToCenter, startToEnd) <= 0)
			return true;
		else if (radius > startToCenter.Length() || radius > endToCenter.Length())
			return true;
	}
	return false;
}

// 線分と線分が交差するかどうかを調べる
inline bool IntersectLines2D
(
   const DirectX::SimpleMath::Vector2& v1,
   const DirectX::SimpleMath::Vector2& v2,
   const DirectX::SimpleMath::Vector2& v3,
   const DirectX::SimpleMath::Vector2& v4)
{
   DirectX::SimpleMath::Vector2 fromV1ToV2 = v2 - v1;
   DirectX::SimpleMath::Vector2 fromV1ToV3 = v3 - v1;
   DirectX::SimpleMath::Vector2 fromV1ToV4 = v4 - v1;
   DirectX::SimpleMath::Vector2 fromV3ToV1 = v1 - v3;
   DirectX::SimpleMath::Vector2 fromV3ToV2 = v2 - v3;
   DirectX::SimpleMath::Vector2 fromV3ToV4 = v4 - v3;

   // 外積の結果が0の場合2つの線分は平行になる
   if (Cross2D(fromV1ToV2, fromV3ToV4) == 0.0f)
	  return false;

   return (Cross2D(fromV1ToV2, fromV1ToV3) * Cross2D(fromV1ToV2, fromV1ToV4) < 0.0f) &&
	  (Cross2D(fromV3ToV4, fromV3ToV1) * Cross2D(fromV3ToV4, fromV3ToV2) < 0.0f);
}

// 線分と線分の交点を計算する
inline  const DirectX::SimpleMath::Vector2 IntersectPointLines2D(
   const DirectX::SimpleMath::Vector2& v1,
   const DirectX::SimpleMath::Vector2& v2,
   const DirectX::SimpleMath::Vector2& v3,
   const DirectX::SimpleMath::Vector2& v4
)
{
   DirectX::SimpleMath::Vector2 fromV1ToV2 = v2 - v1;
   DirectX::SimpleMath::Vector2 fromV1ToV3 = v3 - v1;
   DirectX::SimpleMath::Vector2 fromV3ToV4 = v4 - v3;
   // 線分と線分の交点を計算する
   return v1 + fromV1ToV2 * Cross2D(fromV3ToV4, fromV1ToV3) / Cross2D(fromV3ToV4, fromV1ToV2);
}



