/*
	@file	Prison.h
	@brief	�S���N���X
*/
#pragma once
#ifndef PRISON_DEFINED
#define PRISON_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include <unordered_map>
#include "Libraries/yamadaLib/GameParameter.h"

class Player;

class Prison : public ObjectBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 PRISON_BASE_EXTENTS = { 3.75f, 5.0f, 2.0f };	   //���˔���̃T�C�Y
   static constexpr DirectX::SimpleMath::Vector3 EVENT_BASE_EXTENTS = { 2.5f, 0.5f, 2.5f };		   //
   static constexpr DirectX::SimpleMath::Vector3 EVENT_TRIGGER_OFFSET = { 0.0f, 0.0f, 3.0f };
   static constexpr float ROTATION_QUANTIZE_UNIT = 45.0f;
   static constexpr float OPEN_SPEED = 1.5f;			   	//���̊J�����x
   static constexpr float OPEN_LENGTH = 4.0f;			   	//�����J������
public:
	enum class PrisonParts
	{
		ROOM = 0,
		DOAR = 1
	};
public:
	//�S���̔����J���Ă��邩
	bool GetOpenPrison() { return m_isOpen; }
	//�S�����J����������
	bool GetOpenEndPrison() { return m_isOepnEnd; }

	//�I�u�W�F�N�g�̃^�C�v���擾����
	Type GetType() const override { return Type::PRISON; }
	static IMapObject::Type StaticType() { return IMapObject::Type::PRISON; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//�Փ˔�����擾����
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
    //�R���X�g���N�^
	Prison(const int& type, const GameParameters::ObjectParameter& param);
	//�f�X�g���N�^
	~Prison() override;
	//������
	void Initialize() override;
	//�X�V
	void Update(const float elapsedTime) override;
	//�`��
	void Render() override;
	//�㏈��
	void Finalize() override;
    //�����蔻��̓o�^
	void InitializeCollision() override;
	//�Փˎ��̃C�x���g����
	void HandleCollisionEvent(const CollisionObject* other);

	//���𓮂���
	void MoveDoar(const float elapsedTime);
	//�S���̌����J����
	void OpenPrison();
private:
	std::unordered_map<PrisonParts, DirectX::Model*> m_prisonModel;	//���f��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomdds;  //�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_doordds;  //�e�N�X�`��


	DirectX::SimpleMath::Vector3 m_doarPosition;	//���̍��W
	CollisionObject* m_collisionObject;			    //�����J����C�x���g

	float m_doarMoved;	//���̈ړ���
	bool m_isOpen;		//�S�����J���Ă����Ԃ�
	bool m_isOepnEnd;	//�S�����J����������Ԃ�

	int m_currentStageNumber;						 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;						 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�
};
#endif // PRISON_DEFINED
