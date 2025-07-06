/*
	@file	Wall.h
	@brief	�ǃN���X
*/
#pragma once
#ifndef WALL_DEFINED
#define WALL_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Game/Collision/CollisionManager.h"
#include "Libraries/yamadaLib/GameParameter.h"

class Wall : public ObjectBase
{
public:
	//�I�u�W�F�N�g�̃^�C�v���擾����
	Type GetType() const override { return Type::WALL; }
	static IMapObject::Type StaticType() { return IMapObject::Type::WALL; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//�Փ˔�����擾����
	CollisionObject* GetCollisionObject() const override { return m_collisionObject; }
public:
    //�R���X�g���N�^
    Wall(const int& type, const GameParameters::ObjectParameter& param);
	//�f�X�g���N�^
	~Wall() override;
	//������
	void Initialize() override;
	//�X�V
	void Update(const float elapsedTime) override;
	//�`��
	void Render() override;
	//�㏈��
	void Finalize() override;
	//�����蔻��̏�����
	void InitializeCollision() override;
private:
	int m_type;							     //���f���^�C�v
	int m_currentStageNumber;				 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;				 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�
	CollisionObject* m_collisionObject;		 //�Փ˔���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��
};
#endif // WALL_DEFINED
