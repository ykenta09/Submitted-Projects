/*
	@file	LeverDevice.h
	@brief	�{�^���f�o�C�X�N���X
*/
#pragma once
#ifndef LEVER_DEVICE_DEFINED
#define LEVER_DEVICE_DEFINED

#include"Game/Map/Base/DeviceObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"
#include "Game/Interface/IDevice.h"

class LeverDevice : public DeviceObjectBase
{	
public:
   static constexpr DirectX::SimpleMath::Vector3 EVENT_COLISION_OFFSET = { 0.0f, 0.0f, 1.0f };				//�C�x���g�Փ˔���̍��W�I�t�Z�b�g
   static constexpr DirectX::SimpleMath::Vector3 HANDOLE_LOCAL_OFFSET = { 0.0f, 0.0f, 0.0f };				//�C�x���g�Փ˔���̍��W�I�t�Z�b�g
   static constexpr DirectX::SimpleMath::Quaternion HANDLE_ON_ROTATION = { 0.0f, 0.0f, 0.0f, 1.0f };				//�C�x���g�Փ˔���̍��W�I�t�Z�b�g
   static constexpr DirectX::SimpleMath::Quaternion HANDLE_OFF_ROTATION = { 0.5f, 0.0f, 0.0f, 1.0f };				//�C�x���g�Փ˔���̍��W�I�t�Z�b�g
   static constexpr float HANDLE_ANIMETION_SPEED = { 5.0f };				//�n���h���A�j���[�V�����̑��x
public:
   //�I�u�W�F�N�g�̃^�C�v���擾����
   Type GetType() const override { return Type::LEVER; }
   static IMapObject::Type StaticType() { return IMapObject::Type::LEVER; }
   //�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
   int GetObjectStageNumber() const override { return m_objectStageNumber; }

   //�Փ˔�����擾����
   CollisionObject* GetCollisionObject() const { return m_eventObject; }

   //�^���ʊ֐�
   bool IsDevice() const override { return true; }
public:
	//�R���X�g���N�^
    LeverDevice(const GameParameters::ObjectParameter& param, const int leverID);
	//�f�X�g���N�^
	~LeverDevice() override;

	//����������
	void Initialize() override;
	//�X�V����
	void Update(const float elapsedTime) override;
	//�`�悷��
	void Render() override;
	//�㏈��������
	void Finalize() override;

	//�Փ˔���̏����ݒ�
	void InitializeCollision() override;
	//�Փˎ��̃C�x���g����
	void HandleCollision(const CollisionObject* other);
	//�Փ˔��藣�E���̏���
	void HandleCollisionExit(const CollisionObject* other);
private:
	//���o�[�n���h���̃A�j���[�V����
	void AnimateHandle(const float elapsedTime, bool leverFlag);
private:
    DirectX::Model* m_leverStandModel;		  //���f��
    DirectX::Model* m_leverHandleModel;		  //���f��

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_standTexture;	//�y�䃂�f���̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_handleTexture;	//�n���h�����f���̃e�N�X�`��

	CollisionObject* m_eventObject;	//�Փ˔���

	DirectX::SimpleMath::Vector3 m_initialPosition;		 //�������W
	DirectX::SimpleMath::Vector3 m_currentPosition;		 //���݂̍��W
	DirectX::SimpleMath::Vector3 m_eventPosition;		 //�C�x���g�R���W�����̍��W

	DirectX::SimpleMath::Quaternion m_leverHandle;		 //���o�[�n���h���̉�]

	int m_leverID;		//���o�[��ID
	bool m_pushLever;	//���o�[�������邩�ǂ���
	int m_objectStageNumber;//�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�

	float m_handleAnimetionProgress;		 //�n���h���A�j���[�V�����i�s�x
	bool m_isAnimating;						 //�n���h���A�j���[�V�����̊����t���O
};
#endif // LEVER_DEVICE_DEFINED

