/*
	@file	Goal.h
	@brief	�e�N���X
*/
#pragma once
#ifndef GOAL_DEFINED
#define GOAL_DEFINED

#include"Game/Map/Base/ObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"

//goal�N���X
class Goal : public ObjectBase
{	
public:
	enum class GoalParts : int
	{
		FRAM = 0,
		DOOR = 1,
	};
public:
    static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 7.0f, 5.0f, 0.7f };			//�Փ˔���̃T�C�Y
	static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_EXTENTS = { 2.25f, 1.0f, 3.0f };	//�C�x���g�Փ˔���̃T�C�Y
	static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_OFFSET = { 0.0f, 0.0f, -3.5f };	//�C�x���g�Փ˔���̈ʒu�␳
	static constexpr DirectX::SimpleMath::Vector3 GOAL_POSITION_OFFSET = { 4.5f, 0.0f, 0.0f };	   //���̍��W�␳
	static constexpr float OPNE_GOAL_SPEED = 1.2f;			//���̊J�����x
    static constexpr float OPEN_LENGTH = 3.5f;			   	//���̊J������
public:
   //�S�[�����Ă��邩�擾
   bool IsGaol() const { return m_isGoal; }

   //�I�u�W�F�N�g�̃^�C�v���擾����
   Type GetType() const override { return Type::GOAL; }
   static IMapObject::Type StaticType() { return IMapObject::Type::GOAL; }

   //�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
   int GetObjectStageNumber() const override { return m_objectStageNumber; }

   //�Փ˔�����擾����
   CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
	//�R���X�g���N�^
	Goal(const GameParameters::ObjectParameter& param);
	//�f�X�g���N�^
	~Goal() override;

	//����������
	void Initialize() override;
	//�X�V����
	void Update(const float elapsedTime) override;
	//�`�悷��
	void Render() override;
	//�㏈��������
	void Finalize() override;
	//�Փ˔����o�^����
	void InitializeCollision() override;
	//�Փˎ��̃C�x���g����
	void HandleCollisionEvent(const CollisionObject* other);

	//���𓮂���
	void MoveDoar(const float elapsedTime);

private:
	std::unordered_map<GoalParts, DirectX::Model*> m_goalModel;	//���f��
	std::unordered_map<GoalParts, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_dds;  //�e�N�X�`��


	DirectX::SimpleMath::Vector3 m_doarLeftPosition;	//���̍��W
	DirectX::SimpleMath::Vector3 m_doarRightPosition;
	float m_doarMoved;	//�����J��������
	bool m_isOpened;	//�����J����������

	CollisionObject* m_collisionObject;	//�Փ˔���

	//�v���C���[�ƒ��Ԃ̃t���O
	bool m_goalPlayer;
	bool m_goalFellow;
	bool m_isGoal;		   //�S�[���t���O

	float m_time;		//�o�ߎ���

	int m_currentStageNumber;						 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;						 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�
};
#endif // GOAL_DEFINED

