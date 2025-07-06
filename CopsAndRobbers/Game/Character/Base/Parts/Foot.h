/*
	@file	Foot.h
	@brief	���N���X
*/
#pragma once
#ifndef FOOT_DEFINED
#define FOOT_DEFINED
#include "Game/Interface/ILeaf.h"

// �O���錾
class CommonResources;

//���N���X���`����
class Foot : public ILeaf
{
public:
   static constexpr float FOOT_SWING_INIT_DEG = 3.0f;    // ���U��̏����p�x�i�x�j
   static constexpr float FOOT_SWING_LIMIT_DEG = 40.0f;    // ���U��̍ő�p�x�i�x�j
   static constexpr float FOOT_SWING_DECAY = 0.9f;    // ���U��̌�����
   static constexpr float FOOT_ROT_OFFSET_Y = 0.0f;    // ����Y��]�i�x�j
public:
	//�e���擾����
	IComponent* GetParent() const { return m_parent; };
	//�m�[�h�ԍ����擾����
	int GetNodeNumber() const { return m_nodeNumber; }
	//�p�[�cID���擾����
	int GetPartID() const { return m_partID; }
	//�p�[�c�ԍ����擾����
	int GetPartNumber() const { return m_partNumber; }
	//���݂̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const override { return m_currentPosition; }
	//���݂̍��W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) override { m_currentPosition = pos; }
	//�X�P�[���擾����
	DirectX::SimpleMath::Vector3 GetScale() const override{ return m_scale; }
	//���f�����擾����
	DirectX::Model* GetModel() const override { return m_model; }
	//���f����ݒ肷��
	void SetPartsModel(DirectX::Model* model) { m_model = model; }
	//���݂̉�]�p���擾����
	float GetAngle() const { return m_currentAngle; }
	//���݂̉�]�p��ݒ肷��
	void SetAngle(const float& angle) { m_currentAngle = angle; }

	//�L�����N�^�[�̃|�C���^���擾����
	Player* AsPlayer() override { return nullptr; }
	Enemy* AsEnemy() override { return nullptr; }
	Fellow* AsFellow() override { return nullptr; }

	//�����o������
	void SetPushPos(const DirectX::SimpleMath::Vector3& push) { m_currentPosition += push; }
public:
	//�R���X�g���N�^
	Foot(
		IComponent* parent,
		CommonResources* resources,
		const CharacterBase::CharacterResources& model,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale,
		CharacterBase::PartID partId);
	//�f�X�g���N�^
	~Foot();
	//����������
	void Initialize();
	//�X�V����
	inline void Update(const float& elapsedTime,
		const DirectX::SimpleMath::Vector3& position,
		const float& angle,
		const DirectX::SimpleMath::Vector3& target);
	//�`�悷��
	inline void Render();
	//�㏈������
	void Finalize();

	//�L�[�{�[�h�̒ʒm������
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys);
	//�ړ���
	void Move();
	
private:
	yamadalib::Graphics* m_graphics;	//�O���t�B�b�N�X
	IComponent* m_parent;			    //�e
	CommonResources* m_commonResources;	// ���ʃ��\�[�X

	DirectX::Model* m_model;	// ���f��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_modelDDS;  //���f���̃e�N�X�`��

	DirectX::SimpleMath::Vector3 m_initialPosition;	// �����ʒu
	DirectX::SimpleMath::Vector3 m_currentPosition;	// ���݂̈ʒu
	float m_initialAngle;	// ������]�p
	float m_currentAngle;	// ���݂̉�]�p

	int m_nodeNumber;	// �m�[�h�ԍ�
	int m_partNumber;	// ���i�ԍ�
	int m_partID;		// ���iID

	DirectX::SimpleMath::Matrix m_playerMatrix;	//�v���C���[�̉�]�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;	//���[���h�s��

	DirectX::SimpleMath::Vector3 m_scale;	//�X�P�[��

	float m_rotaY;
	float m_direction; // �����ϐ��������o�[�Ƃ��Ēǉ�

	bool m_isMoving;	//�ړ�����
};
#endif // FOOT_DEFINED
