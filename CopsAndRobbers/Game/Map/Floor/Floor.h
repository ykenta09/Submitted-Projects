/*
	@file	Floor.h
	@brief	���N���X
*/
#pragma once
#ifndef FLOOR_DEFINED
#define FLOOR_DEFINED
#include "Game/Map/Base/ObjectBase.h"
#include "Libraries/yamadaLib/GameParameter.h"
// �O���錾
class CommonResources;

class Floor : public ObjectBase
{
public:
    //�I�u�W�F�N�g�̃^�C�v���擾����
	Type GetType() const override { return Type::FLOOR; }
	static IMapObject::Type StaticType() { return IMapObject::Type::FLOOR; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const override { return m_objectStageNumber; }

	//�Փ˔�����擾����
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
public:
   //�R���X�g���N�^
    Floor(const int& type, const GameParameters::ObjectParameter& param);
	//�f�X�g���N�^
	~Floor() override;
	
	//������
	void Initialize() override;
	//�X�V
	void Update(const float elapsedTime) override;
	//�`��
	void Render() override;
	//�N���b�h�𐶐��֐�
	DirectX::VertexPositionTexture MakeVPT(float x, float y, float z, float u, float v);
	//�㏈��
	void Finalize() override;
	//�Փ˔���̓o�^
	void InitializeCollision() override;
private:
	int m_type;										  //���f���^�C�v
	int m_currentStageNumber;						 //���݂̃X�e�[�W�ԍ�
	int m_objectStageNumber;						 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�

	CollisionObject* m_collisionObject;			    //�R���W�����I�u�W�F�N�g

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;				//�����f���̃e�N�X�`��
	DirectX::Model* m_model;				 //���f��
};
#endif // FLOOR_DEFINED
