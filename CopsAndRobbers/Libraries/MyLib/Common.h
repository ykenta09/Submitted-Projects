#pragma once
#include <random>
#include "SimpleMath.h"

// 2次元の内積を計算する
inline float Dot2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.x + b.y * a.y;
}

// 2次元の外積を計算する
inline float Cross2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

// 線形補間を行う
inline DirectX::SimpleMath::Vector2 Lerp(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b, float& t)
{
	return a + (b - a) * t;
}

// 乱数を取得する
inline int GetRandomVertex(int maxNumber)
{
	// 非決定論的な乱数生成クラス
	std::random_device random{};
	// mt19937クラス
	std::mt19937 mt(random());

	// 離散一様分布クラス
	std::uniform_int_distribution<int> num(0, maxNumber);
	// 乱数を返す
	return num(mt);
}
