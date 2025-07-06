/*
	@file	Math.h
	@brief	計算クラス
*/	
#pragma once
#ifndef MATH_DEFINED
#define MATH_DEFINED

#include "Game/CommonResources.h"


namespace yamadalib
{
	class Math
	{
	public:
		/// <summary>
		/// 線形補間関数 (テンプレート型)
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="t"></param>
		/// <returns></returns>
		template<typename T>
		static T Lerp(T a, T b, float t)
		{
			return a + static_cast<T>(t * (b - a));
		}
		// 最短経路角度補間（弧度法）
		static float LerpAngle(float from, float to, float t) {
		   const float PI = DirectX::XM_PI;
		   const float TAU = DirectX::XM_2PI;

		   // 角度を0～TAUの範囲に正規化
		   from = fmodf(fmodf(from, TAU) + TAU, TAU);
		   to = fmodf(fmodf(to, TAU) + TAU, TAU);

		   // 最短差分計算
		   float diff = to - from;
		   if (diff > PI) {
			  diff -= TAU; // 左回りが近い
		   }
		   else if (diff < -PI) {
			  diff += TAU; // 右回りが近い
		   }

		   return from + diff * t;
		}		

		/// <summary>
		/// 一定速度で最短経路回転
		/// </summary>
		/// <param name="from">開始角度</param>
		/// <param name="to">終了角度</param>
		/// <param name="maxDelta"></param>
		/// <returns></returns>
		static float MoveAngleConstantSpeed(float from, float to, float maxDelta)
		{
		   const float PI = DirectX::XM_PI;
		   const float TAU = DirectX::XM_2PI;

		   // 0～TAUに正規化
		   from = fmodf(fmodf(from, TAU) + TAU, TAU);
		   to = fmodf(fmodf(to, TAU) + TAU, TAU);

		   // 最短差分
		   float diff = to - from;
		   if (diff > PI) diff -= TAU;
		   else if (diff < -PI) diff += TAU;

		   // 差分がmaxDeltaより大きければmaxDeltaだけ進める
		   if (fabsf(diff) <= maxDelta)
			  return to;
		   else
			  return from + (diff > 0 ? +1 : -1) * maxDelta;
		}

		/// <summary>
		/// Clamp関数
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="value"></param>
		/// <param name="min"></param>
		/// <param name="max"></param>
		/// <returns></returns>
		template <typename T>
		static T Clamp(const T& value, const T& min, const T& max) 
		{
			if (value < min) return min;
			if (value > max) return max;
			return value;
		}

		/// <summary>
		/// クォータニオンからオイラー角に変換
		/// </summary>
		/// <param name="q">クォータニオン</param>
		/// <returns></returns>
		static DirectX::SimpleMath::Vector3 QuaternionToEuler(const DirectX::SimpleMath::Quaternion& q)
		{
			DirectX::SimpleMath::Vector3 angles;

			// Roll (x-axis rotation)
			float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			angles.x = std::atan2(sinr_cosp, cosr_cosp);

			// Pitch (y-axis rotation)
			float sinp = 2 * (q.w * q.y - q.z * q.x);
			if (std::abs(sinp) >= 1)
				angles.y = std::copysign(DirectX::XM_PI / 2, sinp); // use 90 degrees if out of range
			else
				angles.y = std::asin(sinp);

			// Yaw (z-axis rotation)
			float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			angles.z = std::atan2(siny_cosp, cosy_cosp);

			return angles;
		}

		/// <summary>
		/// グリッド回転を考慮したコライダーサイズ調整
		/// </summary>
		/// <param name="baseExtents">ベースのコライダーのサイズ</param>
		/// <param name="rotation">回転角</param>
		/// <returns></returns>
		static DirectX::SimpleMath::Vector3 AdjustExtentsForGridRotation(const DirectX::SimpleMath::Vector3& baseExtents, const DirectX::SimpleMath::Quaternion& rotation)
		{
		   const auto euler = QuaternionToEuler(rotation);
		   const float yawDegrees = DirectX::XMConvertToDegrees(euler.y);
		   const float quantized = std::round(yawDegrees / 45.0f) * 45.0f;

		   if (IsRightAngle(quantized)) {
			  return { baseExtents.z, baseExtents.y, baseExtents.x };
		   }
		   return baseExtents;
		}

		/// <summary>
		/// 角度が直角かの判定(90度単位)
		/// </summary>
		/// <param name="degrees">角度</param>
		/// <returns></returns>
		static bool IsRightAngle(float degrees)
		{
		   const float normalized = std::fmod(std::abs(degrees), 360.0f);
		   return (std::abs(normalized - 90.0f) < 1e-5f) ||
				  (std::abs(normalized - 270.0f) < 1e-5f);
		}
	};
}
#endif // MATH_DEFINED
