/*
	@file	IObject.h
	@brief	�I�u�W�F�N�g�C���^�[�t�F�C�X�N���X
*/
#ifndef IOBJECT_DEFINED
#define IOBJECT_DEFINED
#pragma once
#include "Libraries/Params/GameParameter.h"


class IObject
{
public:
   //���݂̍��W���擾����
   virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
   //���݂̍��W��ݒ肷��
   virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
   //��]�p���擾����
   virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;
   //��]�p��ݒ肷��
   virtual void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) = 0;
   //�X�P�[���擾����
   virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;
   //�X�P�[����ݒ肷��
   virtual void  SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;
   //�Փ˔�����擾����
   virtual CollisionObject* GetCollisionObject() const = 0;

public:
   virtual void Initialize() = 0;			   //����������
   virtual void Update(const float& elapsedTime) = 0;		   //�X�V����
   virtual void Render() = 0;		   //�`��
   virtual ~IObject() = default;
};
#endif // IOBJECT_DEFINED
