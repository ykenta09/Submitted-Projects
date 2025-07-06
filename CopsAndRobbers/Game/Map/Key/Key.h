/*
	@file	Key.h
	@brief	���N���X
*/
#pragma once
#ifndef KEY_DEFINED
#define KEY_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"
// �O���錾
class CommonResources;

class Key : public ObjectBase
{
public:
	//�������݂��Ă��邩�m�F
	bool IsExist() const { return m_isExist; }
	Type GetType() const override { return Type::KEY; }
	static IMapObject::Type StaticType() { return IMapObject::Type::KEY; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//���݂�����
	void LostExist() const;

	//�Փ˔�����擾����
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
   //�R���X�g���N�^
	Key(const GameParameters::ObjectParameter& param);
	//�f�X�g���N�^
	~Key() override;

	//������
	void Initialize() override;
	//�X�V
	void Update(const float elapsedTime) override;
	//�`��
	void Render() override;
	//�㏈��
	void Finalize() override;

	//�Փ˔���̓o�^
	void InitializeCollision() override;
	//�Փˎ��̃C�x���g����
	void HandleCollision(const CollisionObject* other);
private:
	bool m_isExist;							  //�������݂��Ă��邩
	CollisionObject* m_collisionObject;		 //�Փ˔���

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��

	int m_currentStageNumber;						 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;						 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�

};
#endif // KEY_DEFINED
