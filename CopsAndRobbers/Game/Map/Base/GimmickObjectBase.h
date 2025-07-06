/*
	@file	GimmickObjectBase.h
	@brief	�M�~�b�N�}�b�v�I�u�W�F�N�g�x�[�X�N���X
*/
#pragma once
#ifndef GIMMICK_OBJECT_BASE_DEFINED
#define GIMMICK_OBJECT_BASE_DEFINED

#include "Game/Interface/IMapObject.h"
#include "Game/Interface/IGimmick.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"

// �O���錾
class CommonResources;

class GimmickObjectBase : public IMapObject, IGimmick 
{
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

   //�������I��������
   bool IsFinished() override { return m_isFinished; }
   //�����I���t���O�����Z�b�g����
   void Reset() override { m_isFinished = false; };


   //IGimmick�ւ̃A�N�Z�T
   IGimmick* AsGimmick() override { return this; }
   //IDevice�ւ̃A�N�Z�T
   IDevice* AsDevice() override { return nullptr; }
public:
   //�R���X�g���N�^
   GimmickObjectBase(
	  const DirectX::SimpleMath::Vector3& initialPosition,
	  const DirectX::SimpleMath::Quaternion& initialQuaternion,
	  const DirectX::SimpleMath::Vector3& scale);
   ~GimmickObjectBase();

	//�������z�֐�
	virtual void Initialize() = 0;
	virtual void Update(const float elapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;
	//�����蔻��̏�����
	virtual void InitializeCollision() = 0;

	//�M�~�b�N���A�N�e�B�u
	void Active() = 0;
	//�M�~�b�N���A�N�e�B�u
	void Deactive() = 0;
protected:
   yamadalib::Graphics* m_graphics;		  //�O���t�B�b�N�X
   CommonResources* m_commonResources;	  // ���ʃ��\�[�X
   Params::ObjectParameter m_parameter;   //�I�u�W�F�N�g�p�����[�^

   bool m_isFinished;				   //�M�~�b�N�����̏I���t���O
};
#endif // GIMMICK_OBJECT_BASE_DEFINED
