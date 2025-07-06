/*
	@file	Collider.h
	@brief	AABB�����蔻��N���X
*/
#pragma once
#ifndef COLLIDER_DEFIEND
#define COLLIDER_DEFIEND
#include <pch.h>

struct AABB
{
	/// <summary>
	/// �����蔻��̏���
	/// </summary>
	/// <param name="boudingBox">���E�{�b�N�X</param>
	/// <param name="opponent">�Փ˔�����s���Ώۂ̋��E�{�b�N�X</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Collider(DirectX::BoundingBox boudingBox, DirectX::BoundingBox opponent) const
	{
		using namespace DirectX::SimpleMath;

		// �����߂��x�N�g��
		Vector3 pushBackVec = Vector3::Zero;

		//AABB�p��min/max���v�Z����
		Vector3 aMin = boudingBox.Center - boudingBox.Extents;
		Vector3 aMax = boudingBox.Center + boudingBox.Extents;

		Vector3 bMin = opponent.Center - opponent.Extents;
		Vector3 bMax = opponent.Center + opponent.Extents;

		//�e���̍������v�Z����
		float dx1 = bMax.x - aMin.x;
		float dx2 = bMin.x - aMax.x;

		//float dy1 = bMax.y - aMin.y;
		//float dy2 = bMin.y - aMax.y;

		float dz1 = bMax.z - aMin.z;
		float dz2 = bMin.z - aMax.z;

		//�e���̐�Βl�����������̎��̂߂荞�ݗʂ��g��
		float dx = std::abs(dx1) < std::abs(dx2) ? dx1 : dx2;
		//float dy = std::abs(dy1) < std::abs(dy2) ? dy1 : dy2;
		float dz = std::abs(dz1) < std::abs(dz2) ? dz1 : dz2;

		// �߂荞�݂���ԏ��������������߂�
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

		//�����߂��ʂ�Ԃ�
		return pushBackVec;
	}


	/// <summary>
	/// �Փ˔���(������)
	/// </summary>
	/// <param name="boundingBox">���E�{�b�N�X</param>
	/// <param name="opponent">�Փ˔�����s���Ώۂ̋��E�{�b�N�X</param>
	/// <returns></returns>
	bool IsCollision(const DirectX::BoundingBox& boundingBox, const DirectX::BoundingBox& opponent) const
	{
	   //�e���̏d�Ȃ�������`�F�b�N
	   return (std::abs(boundingBox.Center.x - opponent.Center.x) < (boundingBox.Extents.x + boundingBox.Extents.x)) &&
		  (std::abs(boundingBox.Center.z - opponent.Center.z) < (boundingBox.Extents.z + opponent.Extents.z));
	}

	/// <summary>
	/// �����o���ʌv�Z�֐��i�œK���j
	/// </summary>
	/// <param name="boundingBox">���E�{�b�N�X</param>
	/// <param name="opponent">�Փ˔�����s���Ώۂ̋��E�{�b�N�X</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 CalculatePushBackVector(const DirectX::BoundingBox& boundingBox, const DirectX::BoundingBox& opponent) const
	{
	   using namespace DirectX::SimpleMath;

	   Vector3 pushBackVec = Vector3::Zero;

	   // min/max���v�Z
	   Vector3 aMin = boundingBox.Center - boundingBox.Extents;
	   Vector3 aMax = boundingBox.Center + boundingBox.Extents;
	   Vector3 bMin = opponent.Center - opponent.Extents;
	   Vector3 bMax = opponent.Center + opponent.Extents;

	   // �e���̍����v�Z
	   float dx1 = bMax.x - aMin.x;
	   float dx2 = bMin.x - aMax.x;
	   float dz1 = bMax.z - aMin.z;
	   float dz2 = bMin.z - aMax.z;

	   // �ŏ��̉����o���ʂ��v�Z
	   float dx = std::abs(dx1) < std::abs(dx2) ? dx1 : dx2;
	   float dz = std::abs(dz1) < std::abs(dz2) ? dz1 : dz2;

	   // �ŏ��̎��ŉ����߂�
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
	/// �������\�b�h�i�݊����ێ��j
	/// </summary>
	/// <param name="boundingBox">���E�{�b�N�X</param>
	/// <param name="opponent">�Փ˔�����s���Ώۂ̋��E�{�b�N�X</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Collider(const DirectX::BoundingBox& boundingBox, const DirectX::BoundingBox& opponent) const
	{
	   // �Փ˂��Ă��Ȃ��ꍇ�̓[���x�N�g��
	   if (!IsCollision(boundingBox, opponent))
	   {
		  return DirectX::SimpleMath::Vector3::Zero;
	   }

	   // �����o���ʂ��v�Z
	   return CalculatePushBackVector(boundingBox, opponent);
	}
};
#endif // COLLIDER_DEFIEND