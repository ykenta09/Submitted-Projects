/*
	@file	MovingWall.h
	@brief	�����ǃN���X
*/
#pragma once
#ifndef MOVING_WALL_DEFINED
#define MOVING_WALL_DEFINED
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Game/Collision/CollisionManager.h"
#include "Libraries/yamadaLib/GameParameter.h"

class MovingWall : public GimmickObjectBase
{
public:
   static constexpr float MOVE_SPEED = { 0.4f };			//�����������x
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
    MovingWall(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID);
	//�f�X�g���N�^
	~MovingWall() override;
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

	//�M�~�b�N���A�N�e�B�u
	void Active() override;
	//�M�~�b�N���A�N�e�B�u
	void Deactive() override;

private:
	int m_type;							     //���f���^�C�v
	int m_currentStageNumber;				 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;				 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�
	int m_gimickID;							 //�M�~�b�NID

	CollisionObject* m_collisionObject;		 //�Փ˔���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��

	float m_time;
	float m_moveProgress;		   //�ړ������̐i�s�x

	DirectX::SimpleMath::Vector3 m_currentPosition;	  //���݂̍��W
	DirectX::SimpleMath::Vector3 m_startPosition;	  //�����ʒu�i��A�N�e�B�u���̈ʒu�j
	DirectX::SimpleMath::Vector3 m_movePosition;	  // �M�~�b�N�N�����Ɉړ�����ڕW�ʒu�i�A�N�e�B�u���j

};
#endif // MOVING_WALL_DEFINED
