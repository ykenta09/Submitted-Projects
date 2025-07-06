/*
	@file	Door.h
	@brief	�h�A�N���X
*/
#pragma once
#ifndef DOOR_DEFINED
#define DOOR_DEFINED

#include"Game/Map/Base/ObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"

class Door : public ObjectBase
{	
public:
   static constexpr DirectX::SimpleMath::Vector3 DOOR_OFFSET = { 1.48f, 0.0f, 0.0f };					//�h�A�̏����I�t�Z�b�g
   static constexpr DirectX::SimpleMath::Vector3 COLISION_EXTENTS = { 1.5f, 2.0f, 0.7f };				//�Փ˔���̃T�C�Y
   static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_EXTENTS = { 3.0f, 1.0f, 1.0f };			//�C�x���g�Փ˔���̃T�C�Y
   static constexpr DirectX::SimpleMath::Vector3 EVENT_OFFSET = { 0.0f, 0.0f, 1.0f };					//�C�x���g�Փ˔���̃I�t�Z�b�g

   static constexpr float OPEN_GOAL_SPEED = { 1.8f };				//���̊J�����x
   static constexpr float OPEN_LENGTH = { 3.5f };					//���̊J������
   static constexpr float CLOSE_TIME = { 4.5f };					//�����܂�܂ł̎���
public:
   //�h�A���J���Ă��邩
   bool IsOpen() const { return m_isOpen; }
   //�X�e�[�W��؂�ւ��邩
   bool IsChange() const { return m_isChangeStage; }
   void SetStageChange(bool flag) { m_isChangeStage = flag; }

   //�I�u�W�F�N�g�̃^�C�v���擾����
   Type GetType() const override { return Type::DOOR; }
   static IMapObject::Type StaticType() { return IMapObject::Type:: DOOR; }

   //�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
   int GetObjectStageNumber() const override { return m_objectStageNumber; }
   //�Փ˔�����擾����
   CollisionObject* GetCollisionObject() const { return m_collisionleftObject; }

   //�ړ���̃X�e�[�W�ԍ����擾����
   const int GetNextStageNumber() const { return m_nextStageNumber; }
   //���̃X�e�[�W�̔ԍ����擾����
   const int GetBackStageNumber() const { return m_backStageNumber; }
public:
	//�R���X�g���N�^
    Door(CommonResources* resources, const GameParameters::ObjectParameter& param, const int nextStage, const int backStage);
	//�f�X�g���N�^
	~Door() override;
	//����������
	void Initialize() override;
	//�X�V����
	void Update(const float elapsedTime) override;
	//�`�悷��
	void Render() override;
	//�㏈��������
	void Finalize() override;
	//�Փ˔���̓o�^
	void InitializeCollision() override;

	//�h�A�̐��ʂ̃C�x���g
	void HandleFrontCollisionEvent(const CollisionObject* other);
	//�h�A�̌�둤�̃C�x���g
	void HandleBackCollisionEvent(const CollisionObject* other);
	//�h�A�C�x���g
	void HandleDoorEvent(const CollisionObject* other, int targetStage);
	//�X�e�[�W�f�[�^�����[�h����
	void LoadStage(int stageNumber);
	//���𓮂���
	void MoveDoar(bool isOpening, const float elapsedTime);
	//�h�A�̍��W�𒲐�����
	void UpdatePosition();
private:
    CommonResources* m_commonResources;	// ���ʃ��\�[�X

	DirectX::Model* m_doorModel;	//���f��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��

	//���̍��W
	DirectX::SimpleMath::Vector3 m_doarLeftInitialPosition;		  //�����������W
	DirectX::SimpleMath::Vector3 m_doarLeftPosition;			  //�����̌��݂̍��W
	DirectX::SimpleMath::Vector3 m_doarRightInitialPosition;	  //�E���������W
	DirectX::SimpleMath::Vector3 m_doarRightPosition;			  //�E���̌��݂̍��W
	DirectX::SimpleMath::Vector3 m_frontEventPosition;			  //���ʃC�x���g�Փ˔���̍��W
	DirectX::SimpleMath::Vector3 m_backEventPosition;			  //�����C�x���g�Փ˔���̍��W

	float m_doarMoved;	//�����J��������
	bool m_isOpened;	//�����J����������

	//�Փ˔���
	CollisionObject* m_collisionleftObject;		   //�����̏Փ˔���
	CollisionObject* m_collisionrightObject;	   //�E���̏Փ˔���
	CollisionObject* m_frontEventObject;		   //���ʃC�x���g�Փ˔���
	CollisionObject* m_backEventObject;			   //�����C�x���g�Փ˔���
	
	bool m_isOpen;			  //���̊J���t���O
	bool m_isChangeStage;	  //�X�e�[�W��؂�ւ���t���O

	float m_closeTimer;	//�����܂�܂ł̎���
	float m_time;		//�o�ߎ���

	int m_nextStageNumber;	//�ړ���̃X�e�[�W�ԍ�
	int m_backStageNumber;	//�߂��Ă���X�e�[�W�̔ԍ�
	int m_objectStageNumber;//�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�

};
#endif // DOOR_DEFINED

