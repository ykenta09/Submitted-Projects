/*
	@file	Collider.h
	@brief	AABB当たり判定クラス
*/
#pragma once
#ifndef COLLIDER_DEFIEND
#define COLLIDER_DEFIEND
#include <pch.h>

struct AABB
{
	/// <summary>
	/// 当たり判定の処理
	/// </summary>
	/// <param name="boudingBox">境界ボックス</param>
	/// <param name="opponent">衝突判定を行う対象の境界ボックス</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Collider(DirectX::BoundingBox boudingBox, DirectX::BoundingBox opponent) const
	{
		using namespace DirectX::SimpleMath;

		// 押し戻しベクトル
		Vector3 pushBackVec = Vector3::Zero;

		//AABB用のmin/maxを計算する
		Vector3 aMin = boudingBox.Center - boudingBox.Extents;
		Vector3 aMax = boudingBox.Center + boudingBox.Extents;

		Vector3 bMin = opponent.Center - opponent.Extents;
		Vector3 bMax = opponent.Center + opponent.Extents;

		//各軸の差分を計算する
		float dx1 = bMax.x - aMin.x;
		float dx2 = bMin.x - aMax.x;

		//float dy1 = bMax.y - aMin.y;
		//float dy2 = bMin.y - aMax.y;

		float dz1 = bMax.z - aMin.z;
		float dz2 = bMin.z - aMax.z;

		//各軸の絶対値が小さい方の軸のめり込み量を使う
		float dx = std::abs(dx1) < std::abs(dx2) ? dx1 : dx2;
		//float dy = std::abs(dy1) < std::abs(dy2) ? dy1 : dy2;
		float dz = std::abs(dz1) < std::abs(dz2) ? dz1 : dz2;

		// めり込みが一番小さい軸を押し戻す
		if (abs(dx) /*<= abs(dy) && abs(dx)*/ <= abs(dz))
		{
			pushBackVec.x += dx;
		}
		//else if (abs(dy) <= abs(dx) && abs(dy) <= abs(dz))
		//{
		//	pushBackVec.y += dy;
		//}
		else
		{
			pushBackVec.z += dz;
		}

		//押し戻す量を返す
		return pushBackVec;
	}


	/// <summary>
	/// 衝突判定(高速化)
	/// </summary>
	/// <param name="boundingBox">境界ボックス</param>
	/// <param name="opponent">衝突判定を行う対象の境界ボックス</param>
	/// <returns></returns>
	bool IsCollision(const DirectX::BoundingBox& boundingBox, const DirectX::BoundingBox& opponent) const
	{
	   //各軸の重なりを高速チェック
	   return (std::abs(boundingBox.Center.x - opponent.Center.x) < (boundingBox.Extents.x + boundingBox.Extents.x)) &&
		  (std::abs(boundingBox.Center.z - opponent.Center.z) < (boundingBox.Extents.z + opponent.Extents.z));
	}

	/// <summary>
	/// 押し出し量計算関数（最適化）
	/// </summary>
	/// <param name="boundingBox">境界ボックス</param>
	/// <param name="opponent">衝突判定を行う対象の境界ボックス</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 CalculatePushBackVector(const DirectX::BoundingBox& boundingBox, const DirectX::BoundingBox& opponent) const
	{
	   using namespace DirectX::SimpleMath;

	   Vector3 pushBackVec = Vector3::Zero;

	   // min/maxを計算
	   Vector3 aMin = boundingBox.Center - boundingBox.Extents;
	   Vector3 aMax = boundingBox.Center + boundingBox.Extents;
	   Vector3 bMin = opponent.Center - opponent.Extents;
	   Vector3 bMax = opponent.Center + opponent.Extents;

	   // 各軸の差分計算
	   float dx1 = bMax.x - aMin.x;
	   float dx2 = bMin.x - aMax.x;
	   float dz1 = bMax.z - aMin.z;
	   float dz2 = bMin.z - aMax.z;

	   // 最小の押し出し量を計算
	   float dx = std::abs(dx1) < std::abs(dx2) ? dx1 : dx2;
	   float dz = std::abs(dz1) < std::abs(dz2) ? dz1 : dz2;

	   // 最小の軸で押し戻し
	   if (std::abs(dx) <= std::abs(dz))
	   {
		  pushBackVec.x = dx;
	   }
	   else
	   {
		  pushBackVec.z = dz;
	   }

	   return pushBackVec;
	}

	/// <summary>
	/// 統合メソッド（互換性維持）
	/// </summary>
	/// <param name="boundingBox">境界ボックス</param>
	/// <param name="opponent">衝突判定を行う対象の境界ボックス</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Collider(const DirectX::BoundingBox& boundingBox, const DirectX::BoundingBox& opponent) const
	{
	   // 衝突していない場合はゼロベクトル
	   if (!IsCollision(boundingBox, opponent))
	   {
		  return DirectX::SimpleMath::Vector3::Zero;
	   }

	   // 押し出し量を計算
	   return CalculatePushBackVector(boundingBox, opponent);
	}
};
#endif // COLLIDER_DEFIEND