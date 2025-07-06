/*
	@file	Light.h
	@brief	���C�g�N���X
*/
#pragma once
#ifndef LIGHT_DEFINED
#define LIGHT_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"
// �O���錾
class CommonResources;

class Light : public ObjectBase
{
public:
	//�I�u�W�F�N�g�^�C�v���擾����
	Type GetType() const override { return Type::LIGHT; }
	static IMapObject::Type StaticType() { return IMapObject::Type::LIGHT; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const override { return m_objectStageNumber; }
	//�Փ˔�����擾����
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
    //�R���X�g���N�^
    Light(CommonResources* resources, const GameParameters::ObjectParameter& param, const DirectX::SimpleMath::Vector3& lightColor, const float& range, const bool& modelDraw);
	//�f�X�g���N�^
	~Light() override;

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
   //���C�g�̐ݒ�
   void SettingLight();
private:
    CommonResources* m_commonResources;	// ���ʃ��\�[�X
	CollisionObject* m_collisionObject;		 //�Փ˔���

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��

	int m_currentStageNumber;						 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;						 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�

	float m_range;								   //���C�g�̉e���͈�
	DirectX::SimpleMath::Vector3 m_color;		   //���C�g�̃J���[
	bool m_modelDraw;							   //���C�g���f���̕`��t���O
};
#endif // LIGHT_DEFINED
