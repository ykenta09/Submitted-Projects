/*
	@file	IGameObject.h
	@brief	�Q�[���I�u�W�F�N�g�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#include "pch.h"
#include <Game/Collision/CollisionObject.h>
#ifndef IGAME_OBJECT_DEFINED
#define IGAME_OBJECT_DEFINED

class IGameObject
{
public:
   //���f�����擾����
   virtual const DirectX::Model* GetModel() const = 0;
   //���f����ݒ肷��
   virtual void SetModel(const DirectX::Model* model) = 0;
   //���W���擾����
   virtual const DirectX::SimpleMath::Vector3 GetPosition() const = 0;
   //���W��ݒ肷��
   virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
   //��]���擾����
   virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;
   //��]��ݒ肷��
   virtual void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) = 0;
   //�X�P�[���擾����
   virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;
   //�X�P�[����ݒ肷��
   virtual void  SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;
   //�Փ˔�����擾����
   virtual CollisionObject* GetCollisionObject() const = 0;
};
#endif // IGAME_OBJECT_DEFINED
