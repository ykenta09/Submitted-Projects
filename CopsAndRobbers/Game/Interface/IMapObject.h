/*
	@file	IMapObject.h
	@brief	�}�b�v�I�u�W�F�N�g�̃C���^�[�t�F�C�X�N���X
*/
#ifndef IMAP_OBJECT_DEFINED
#define IMAP_OBJECT_DEFINED
#pragma once
#include "Game/CommonResources.h"
#include "Game/Collision/CollisionManager.h"
#include "Libraries/Params/GameParameter.h"

class IGimmick;
class IDevice;

class IMapObject
{
public:
    //�I�u�W�F�N�g�̃^�C�v
    enum class Type { WALL, FLOOR, PRISON, KEY, GOAL, DOOR, LEVER, LIGHT };

    //���f�����擾����
    virtual const DirectX::Model* GetModel() const = 0;
    //���f����ݒ肷��
    virtual	void SetModel(const DirectX::Model* model) = 0;

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

	//�I�u�W�F�N�g�^�C�v���擾����
	virtual Type GetType() const = 0;

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	virtual int GetObjectStageNumber() const = 0;

	//�Փ˔�����擾����:
	virtual CollisionObject* GetCollisionObject() const = 0;

	//�^���ʊ֐�
	virtual bool IsGimmick() const = 0;
	virtual bool IsDevice() const = 0;

	//IGimmick�ւ̃A�N�Z�T
	virtual IGimmick* AsGimmick() = 0;
	//IDevice�ւ̃A�N�Z�T
	virtual IDevice* AsDevice() = 0;
public:
	//���z�f�X�g���N�^
	virtual ~IMapObject() = default;
	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update(const float elapsedTime) = 0;
	//�`��
	virtual void Render() = 0;
	//�㏈��
	virtual void Finalize() = 0;
	//�����蔻��̏�����
	virtual void InitializeCollision() = 0;
};
#endif // IMAP_OBJECT_DEFINED
