/*
	@file	CollisionObject.h
	@brief	�R���W�����I�u�W�F�N�g�N���X
*/
#pragma once
#ifndef COLLISION_OBJECT_DEFINED
#define COLLISION_OBJECT_DEFINED
#include "Game/Collision/CollisionManager.h"
#include "Game/Collider.h"


class CollisionObject
{
public:
   //�p�����[�^�̎擾
   const CollisionManager::CollisionParameter& GetCollisionParameter() const { return m_param; }
   //���L�҂��擾����
   void* GetOwner() const { return m_owner; }
   //���L�҂�ݒ肷��
   void SetOwner(void* owner) { m_owner = owner; }

public:
   //�R���X�g���N�^
   CollisionObject(const CollisionManager::CollisionParameter& param);

   //�Փ˔���̃`�F�b�N�^�C�v���Ƃɔ�����s��
   bool CollidesWith(const CollisionObject* other) const;

   //AABB�Փ˔���
   bool IntersectsAABB(const CollisionObject* other) const;
   //���̏Փ˔���
   bool IntersectsSphere(const CollisionObject* other) const;
   //�����o���ʌv�Z
   DirectX::SimpleMath::Vector3 CalculatePushBack(const CollisionObject* other) const;

   //�Փˎ��̃C�x���g�������s��
   void HandleCollision(const CollisionObject* other) const;
   //�Փ˔���𗣒E���C�x���g�������s��
   void HandleCollisionExit(const CollisionObject* other) const;
   //�����蔻��̍X�V
   void UpdatePosition(const DirectX::SimpleMath::Vector3 newPosition = DirectX::SimpleMath::Vector3::Zero, const DirectX::SimpleMath::Vector3 extents = DirectX::SimpleMath::Vector3::One);
   //�R���W�����I�u�W�F�N�g�����Z�b�g����
   void Reset(const CollisionManager::CollisionParameter& param);
private:
   CollisionManager::CollisionParameter m_param;	  //�Փ˔���̃p�����[�^
   AABB m_aabb;										  //AABB�̔���
   void* m_owner = nullptr;							  //���L��
   DirectX::SimpleMath::Vector3 m_baseExtents;		  //�������E�{�b�N�X�T�C�Y
};
#endif // COLLISION_OBJECT_DEFINED
