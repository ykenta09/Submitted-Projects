/*
	@file	DeviceObjectBase.h
	@brief	�f�o�C�X�}�b�v�I�u�W�F�N�g�x�[�X�N���X
*/
#pragma once
#ifndef DEVICE_OBJECT_BASE_DEFINED
#define DEVICE_OBJECT_BASE_DEFINED

#include "Game/Interface/IMapObject.h"
#include "Game/Interface/IDevice.h"
#include "Game/Interface/IObserver.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"

// �O���錾
class CommonResources;

class DeviceObjectBase : public IMapObject, public IDevice, public IObserver
{
public:
   static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_EXTENTS = { 1.5f, 1.0f, 1.0f };				//�C�x���g�Փ˔���̃T�C�Y
public:
   //IMapObject�C���^�[�t�F�C�X����---------------------------------------------
   //���f�����擾����
   const DirectX::Model* GetModel() const override { return m_parameter.model; }
   //���f����ݒ肷��
   void SetModel(const DirectX::Model* model) override { m_parameter.model = model; }

   //���W���擾����
   DirectX::SimpleMath::Vector3 GetPosition() const override { return m_parameter.position; }
   //���W��ݒ肷��
   void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_parameter.position = pos; }

   //��]���擾����
   DirectX::SimpleMath::Quaternion GetRotation() const override { return m_parameter.rotation; }
   //��]��ݒ肷��
   void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) override { m_parameter.rotation = rotation; }

   //�X�P�[�����擾����
   DirectX::SimpleMath::Vector3 GetScale() const override { return m_parameter.scale; }
   //�X�P�[����ݒ肷��
   void SetScale(const DirectX::SimpleMath::Vector3& scale) override { m_parameter.scale = scale; }

   //�I�u�W�F�N�g�^�C�v���擾����
   Type GetType() const = 0;
   //�Փ˔�����擾����
   CollisionObject* GetCollisionObject() const = 0;

   //�^���ʊ֐�
   bool IsGimmick() const { return false; }
   bool IsDevice() const { return false; }

   //IGimmick�ւ̃A�N�Z�T
   IGimmick* AsGimmick() override { return nullptr; }
   //IDevice�ւ̃A�N�Z�T
   IDevice* AsDevice() override { return this; }

   //���u�̑���͈͂ɂ��邩�擾����
   bool GetInsideControlZone() const { return m_isInsideControlZone; }
   //���u�̑���͈͂ɂ��邩��ݒ肷��
   void SetInsideControlZone(bool flag) { m_isInsideControlZone = flag; }

protected:
   //���u���N�������ǂ���
   bool IsTriggered() const { return m_isTriggered; }
   //���u��ON��OFF��
   bool IsEnabled() const { return m_isEnabled;  }

public:
   //�R���X�g���N�^
   DeviceObjectBase(
	  const DirectX::SimpleMath::Vector3& initialPosition,
	  const DirectX::SimpleMath::Quaternion& initialQuaternion,
	  const DirectX::SimpleMath::Vector3& scale);
   //�f�X�g���N�^
   ~DeviceObjectBase();

	//�������z�֐�
	virtual void Initialize() = 0;
	virtual void Update(const float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

	//�Փ˔���̓o�^
	void InitializeCollision() = 0;

	//�L�[�{�[�h���͏���
	void OnKeyPressed(const DirectX::Keyboard::Keys& key);

	//���u�ɃM�~�b�N�̓o�^
	void RegisterGimmick(IGimmick* gimmick) override;
	//�M�~�b�N���N������
	void Trigger(bool flag) override;

   //�o�^���ꂽ�M�~�b�N�������I��������
   void AreAllGimmicksFinished();
   //�o�^�����M�~�b�N�̏I���t���O�����Z�b�g����
   void GimmicksReset();
protected:
   yamadalib::Graphics* m_graphics;		  //�O���t�B�b�N�X
   CommonResources* m_commonResources;	  // ���ʃ��\�[�X
private:
   Params::ObjectParameter m_parameter;   //�I�u�W�F�N�g�p�����[�^
   std::vector<IGimmick*> m_targets;	 //�M�~�b�N���X�g
   bool m_isTriggered;				//���u���N�������ǂ���
   bool m_isEnabled;				//���u��ON��OFF��
   bool m_isInsideControlZone;		//���u�̑���͈͂ɂ��邩
};
#endif // DEVICE_OBJECT_BASE_DEFINED
