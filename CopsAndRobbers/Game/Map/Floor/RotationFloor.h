/*
	@file	RotationFloor .h
	@brief	��]���鏰�N���X
*/
#pragma once
#ifndef ROTATION_FLOOR_DEFINED
#define ROTATION_FLOOR_DEFINED
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"

// �O���錾
class CommonResources;

class RotationFloor : public GimmickObjectBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 2.5f, 2.5f, 2.5f };			//�Փ˔���̃T�C�Y
   static constexpr float ROTATION_Y = DirectX::XMConvertToRadians(90.0f);
   static constexpr float MOVE_SPEED = { 0.6f };			//������]���鑬�x
public:
    //�I�u�W�F�N�g�̃^�C�v���擾����
	Type GetType() const override { return Type::FLOOR; }
	static IMapObject::Type StaticType() { return IMapObject::Type::FLOOR; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//�Փ˔�����擾����
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }

	//�^���ʊ֐�
	bool IsGimmick() const override { return true; }
public:
    //�R���X�g���N�^
    RotationFloor(const int& type, const GameParameters::ObjectParameter& param, const int& gimmickID);
	//�f�X�g���N�^
	~RotationFloor() override;

	//����������
	void Initialize() override;
	//�X�V����
	void Update(const float elapsedTime) override;
	//�`�悷��
	void Render() override;
	//��n������
	void Finalize() override;

	//�Փ˔���̏�����
	void InitializeCollision() override;

	//�M�~�b�N���A�N�e�B�u
	void Active() override;
	//�M�~�b�N���A�N�e�B�u
	void Deactive() override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��

	int m_type;					   //���f���^�C�v
	int m_gimickID;				   //�M�~�b�NID
	int m_currentStageNumber;	   //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;	   //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�

	float m_time;
	float m_moveProgress;		   //�ړ������̐i�s�x

	CollisionObject* m_collisionObject;				  //�Փ˔���

	DirectX::SimpleMath::Quaternion m_floorRotation;		 //���o�[�n���h���̉�]

	DirectX::SimpleMath::Vector3 m_baseExtents;

	DirectX::SimpleMath::Vector3 m_currentPosition;	  //���݂̍��W
	DirectX::SimpleMath::Vector3 m_startPosition;	  //�����ʒu�i��A�N�e�B�u���̈ʒu�j
	DirectX::SimpleMath::Vector3 m_movePosition;	  // �M�~�b�N�N�����Ɉړ�����ڕW�ʒu�i�A�N�e�B�u���j
};
#endif // ROTATION_FLOOR_DEFINED
