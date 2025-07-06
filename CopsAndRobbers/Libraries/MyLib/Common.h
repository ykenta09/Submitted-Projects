#pragma once
#include <random>
#include "SimpleMath.h"

// 2�����̓��ς��v�Z����
inline float Dot2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.x + b.y * a.y;
}

// 2�����̊O�ς��v�Z����
inline float Cross2D(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

// ���`��Ԃ��s��
inline DirectX::SimpleMath::Vector2 Lerp(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& b, float& t)
{
	return a + (b - a) * t;
}

// �������擾����
inline int GetRandomVertex(int maxNumber)
{
	// �񌈒�_�I�ȗ��������N���X
	std::random_device random{};
	// mt19937�N���X
	std::mt19937 mt(random());

	// ���U��l���z�N���X
	std::uniform_int_distribution<int> num(0, maxNumber);
	// ������Ԃ�
	return num(mt);
}
