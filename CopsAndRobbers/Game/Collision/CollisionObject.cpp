/*
	@file	CollisionManager.cpp
	@brief	�R���W�����I�u�W�F�N�g�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Collision/CollisionManager.h"
#include "CollisionObject.h"
#include "Libraries/yamadaLib/Math.h"


/// <summary>
///  �R���X�g���N�^
/// </summary>
/// <param name="param">�Փ˔���̃p�����[�^</param>
CollisionObject::CollisionObject(const CollisionManager::CollisionParameter& param)
   :
   m_param(param),
   m_baseExtents(param.collision.box.Extents),
   m_aabb{},
   m_owner(param.owner)
{
}

bool CollisionObject::CollidesWith(const CollisionObject* other) const
{
   //�Փ˔���̃`�F�b�N�^�C�v���擾
   auto myType = m_param.collision.checkType;
   auto otherType = other->m_param.collision.checkType;

   //�������݂̂̂̔���
   if (myType == CollisionManager::CollisionCheckType::SphereOnly && otherType == CollisionManager::CollisionCheckType::SphereOnly)
   {
	  return IntersectsSphere(other);
   }

   //�����{�b�N�X�݂̂̔���
   if (myType == CollisionManager::CollisionCheckType::BoxOnly && otherType == CollisionManager::CollisionCheckType::BoxOnly)
   {
	  return IntersectsAABB(other);
   }

   //���̂ƃ{�b�N�X�̔���
   if (myType == CollisionManager::CollisionCheckType::SphereAndBox || otherType == CollisionManager::CollisionCheckType::SphereAndBox)
   {
	  if (IntersectsSphere(other))
	  {
		 return IntersectsAABB(other);
	  }
   }

   return false;
}


/// <summary>
/// AABB�Փ˔���
/// </summary>
/// <param name="other">�Փ˔�����s���Ώ�</param>
/// <returns></returns>
bool CollisionObject::IntersectsAABB(const CollisionObject* other) const
{
   //AABB�̏Փ˔�����s��
   return m_param.collision.box.Intersects(other->m_param.collision.box);
}

/// <summary>
/// ���̂̏Փ˔���
/// </summary>
/// <param name="other">�Փ˔�����s���Ώ�</param>
/// <returns></returns>
bool CollisionObject::IntersectsSphere(const CollisionObject* other) const
{
   //���̂̏Փ˔�����s��
   return m_param.collision.sphere.Intersects(other->m_param.collision.sphere);
}

/// <summary>
/// �����o���ʂ��v�Z����
/// </summary>
/// <param name="other">�߂荞�ݗʂ����߂�R���W�����I�u�W�F�N�g</param>
/// <returns></returns>
DirectX::SimpleMath::Vector3 CollisionObject::CalculatePushBack(const CollisionObject* other) const
{
   //�����o���ʂ̌v�Z���s��
   return m_aabb.CalculatePushBackVector(m_param.collision.box, other->m_param.collision.box);
}


/// <summary>
/// �Փˎ��̃C�x���g�������s��
/// </summary>
/// <param name="other">�Փ˂����Ώ�</param>
void CollisionObject::HandleCollision(const CollisionObject* other) const 
{
   //�Փ˃C�x���g���Ȃ��ꍇ�͍s��Ȃ�
   if (m_param.NotifyEvent)
   {
	  m_param.NotifyEvent(other);
   }
}

/// <summary>
/// �Փ˔���𗣒E���C�x���g�������s��
/// </summary>
/// <param name="other">���E�����Ώ�</param>
void CollisionObject::HandleCollisionExit(const CollisionObject* other) const
{
   //�Փ˔��藣�E�C�x���g���Ȃ��ꍇ�͍s��Ȃ�
   if (m_param.NotifyExitEvent)
   {
	  m_param.NotifyExitEvent(other);
   }
}

/// <summary>
/// �Փ˔���̍X�V
/// </summary>
/// <param name="newPosition">�Փ˔���̍X�V���W</param>
void CollisionObject::UpdatePosition(const DirectX::SimpleMath::Vector3 newPosition, const DirectX::SimpleMath::Vector3 extents)
{
   //�R���W�����I�u�W�F�N�g�̃p�����[�^���X�V
   m_param.collision.box.Center = newPosition;
   m_param.collision.sphere.Center = newPosition;
   //�f�t�H���g�����̂܂܂Ȃ珈�������Ȃ�
   if (extents == DirectX::SimpleMath::Vector3::One) return;
   m_param.collision.box.Extents = extents;
}


/// <summary>
/// �R���W�����I�u�W�F�N�g�̃p�����[�^�����Z�b�g����
/// </summary>
/// <param name="param">�Փ˔���̃p�����[�^</param>
void CollisionObject::Reset(const CollisionManager::CollisionParameter& param)
{
   //�p�����[�^���Z�b�g
   m_param = param;
   m_aabb = {};

   //���E�{�b�N�X�Ƌ��E���̍��W���Z�b�g
   m_param.collision.box.Center = param.collision.box.Center;
   m_param.collision.sphere.Center = param.collision.sphere.Center;
}


