/*
	@file	IComponent.h
	@brief	�R���|�[�l���g�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef ICOMPONENT_DEFINED
#define ICOMPONENT_DEFINED

#include "Game/CommonResources.h"
#include "Game/Collider.h"
#include "Game/Interface/IObserver.h"
#include "Game/Collider.h"

class Player;
class Enemy;
class Fellow;

class IComponent : public IObserver
{
public:
   //�m�[�h�ԍ����擾����
   virtual int GetNodeNumber() const = 0;
   //�p�[�cID���擾����
   virtual int GetPartID() const = 0;
   //�p�[�c�ԍ����擾����
   virtual int GetPartNumber() const = 0;
   //���݂̍��W���擾����
   virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
   //���݂̍��W��ݒ肷��
   virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
   //���݂̉�]�p���擾����
   virtual float GetAngle() const = 0;
   //���݂̉�]�p��ݒ肷��
   virtual void SetAngle(const float& angle) = 0;
   //�X�P�[�����擾����
   virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;
   //���f�����擾����
   virtual DirectX::Model* GetModel() const = 0;

   virtual Player* AsPlayer() = 0;
   virtual Enemy* AsEnemy() = 0;
   virtual Fellow* AsFellow() = 0;

public:
   //���z�f�X�g���N�^
   virtual ~IComponent() = default;
   //������
   virtual void Initialize() = 0;
   //�X�V
   virtual void Update(const float& elapsedTime,
	  const DirectX::SimpleMath::Vector3& position,
	  const float& angle,
	  const DirectX::SimpleMath::Vector3& target) = 0;
   //�`��
   virtual void Render() = 0;
   //�㏈��
   virtual void Finalize() = 0;

   //�L�[�{�[�h�������ꂽ�Ƃ��ɌĂяo�����
   virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};
#endif // IPLAYER_DEFINED
