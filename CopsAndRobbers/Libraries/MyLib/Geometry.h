#pragma once
#include "Common.h"
#include "SimpleMath.h"

inline DirectX::SimpleMath::Vector2 Vector3ToVector2(const DirectX::SimpleMath::Vector3& vector)
{
   return DirectX::SimpleMath::Vector2(vector.x, vector.z);
}

// �����ƕ��ʂ̌���������s��
inline bool IntersectRayPlane(
	const DirectX::SimpleMath::Ray& ray,
	const DirectX::SimpleMath::Plane& plane,
	DirectX::SimpleMath::Vector3* const intersection
)
{
	/*
	�������ʂƌ�_�̎��@t = normal * (position - origin) / (normal * direction)
	��_ origin + direction * t

	����: P = P0 + tV
	����: P �EN + d = 0
	(P0 + tV)�EN + d = 0
	t = -(P0�EN + d) / (V�EN)
	P = P0 + tV
	*/

	// ���ʖ@���x�N�g���ƌ����̓��ς��v�Z����
	float d = plane.Normal().Dot(ray.direction);
	// �����ƕ��ʂ����s�̏ꍇ�͌�_�͑��݂��Ȃ�
	if (d == 0.0f)
		return false;
	// ���ʂ̖@���x�N�g���ƌ����̌��_�̊Ԃ̓��ς��v�Z����
	float t = (plane.D() - plane.Normal().Dot(ray.position)) / d;
	// �����_���v�Z����
	*intersection = ray.position + ray.direction * t;
	// �����ƕ��ʂ��������Ă���
	return true;
}

// �_���~�̓����ɂ��邩�ǂ����𔻒肷��
inline bool InsideCircle(const int& radius, const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& point)
{
	// 2�_�Ԃ̋������v�Z����
	float distance = (center.x - point.x) * (center.x - point.x) + (center.z - point.z) * (center.z - point.z);
	// �����Ɣ��a�̓��Ɣ�r����
	if (distance <= radius * radius)
		return true;
	else
		return false;
}

// �~�Ɛ����̌���������s��
inline bool IntersectCircleLine(
	const DirectX::SimpleMath::Vector3& center,					// ���S�_
	const float& radius,																// ���a
	const DirectX::SimpleMath::Vector3& start,						// �����̊J�n
	const DirectX::SimpleMath::Vector3& end							// �����̏I��
)
{
	// �����̊J�n�n�_����~�̒��S�ւ̃x�N�g��
	DirectX::SimpleMath::Vector2 startToCenter = Vector3ToVector2(center - start);
	// �����̏I���n�_����~�̒��S�ւ̃x�N�g��
	DirectX::SimpleMath::Vector2 endToCenter = Vector3ToVector2(center - end);
	// �����̊J�n�ʒu����I���ʒu�ւ̃x�N�g��
	DirectX::SimpleMath::Vector2 startToEnd = Vector3ToVector2(end - start);
	// �~�̒��S��������܂ł̋������v�Z����
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

// �����Ɛ������������邩�ǂ����𒲂ׂ�
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

   // �O�ς̌��ʂ�0�̏ꍇ2�̐����͕��s�ɂȂ�
   if (Cross2D(fromV1ToV2, fromV3ToV4) == 0.0f)
	  return false;

   return (Cross2D(fromV1ToV2, fromV1ToV3) * Cross2D(fromV1ToV2, fromV1ToV4) < 0.0f) &&
	  (Cross2D(fromV3ToV4, fromV3ToV1) * Cross2D(fromV3ToV4, fromV3ToV2) < 0.0f);
}

// �����Ɛ����̌�_���v�Z����
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
   // �����Ɛ����̌�_���v�Z����
   return v1 + fromV1ToV2 * Cross2D(fromV3ToV4, fromV1ToV3) / Cross2D(fromV3ToV4, fromV1ToV2);
}



