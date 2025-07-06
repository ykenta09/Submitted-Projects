/*
	@file	Root.h
	@brief	���[�g�N���X
*/
#pragma once
#ifndef ROOT_DEFINED
#define ROOT_DEFINED
#include <unordered_map>
#include "Game/Interface/IComposite.h"

//�V�[���O���t���[�g�N���X
class Root : public IComposite
{
public:
	//�e���擾����
	IComponent* GetParent() { return m_parent; }

public:
	// IComponent �C���^�[�t�F�[�X�̏������z�֐��̎���
	int GetNodeNumber() const override { return m_nodeNumber; }
	int GetPartID() const override { return m_partID; }
	int GetPartNumber() const override { return m_partNumber; }
	DirectX::SimpleMath::Vector3 GetPosition() const override { return m_currentPosition; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) override { m_currentPosition = position; }
	DirectX::SimpleMath::Vector3 GetScale() const  { return DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f); }
	float GetAngle() const override { return m_currentAngle; }
	void SetAngle(const float& angle) override { m_currentAngle = angle; }
	DirectX::Model* GetModel() const override { return nullptr; }
	IComponent* GetLastAttachedNode() const { return m_nodes.back().get(); }

	//�L�����N�^�[�̃|�C���^���擾����
	Player* AsPlayer() override { return m_player; }
	Enemy* AsEnemy() override { return nullptr; }
	Fellow* AsFellow() override { return nullptr; }

	const std::vector<Enemy*>& GetEnemies() const { return m_enemies; }
	//���Ԃ��擾����
    const std::vector<Fellow*>& GetFellows() const { return m_fellows; }
public:
	//�R���X�g���N�^
	Root(IComponent* parent,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle);
	//�f�X�g���N�^
	~Root();
	//����������
	void Initialize() override;
	//�L�[�{�[�h�ʒm������
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys) override;
	//�X�V����
	void Update(const float& elapsedTime,
		const DirectX::SimpleMath::Vector3& position,
		const float& angle,
		const DirectX::SimpleMath::Vector3& target) override;
	//�L�����N�^�[��ǉ�����
	void Attach(std::unique_ptr<IComponent> playerParts) override;
	//�L�����N�^�[���폜����
	void Detach(std::unique_ptr<IComponent> playerParts) override;
	//�`�悷��
	void Render() override;
	//�㏈��������
	void Finalize() override;
private:
	//�e
	IComponent* m_parent;
	//�v���C���[
	Player* m_player;
	//�G
	std::vector<Enemy*> m_enemies;
	//����
	std::vector<Fellow*> m_fellows;
	//�m�[�h�ԍ�
	int m_nodeNumber;
	//�p�[�c�ԍ�
	int m_partNumber;
	//�p�[�cID
	int m_partID;
	// �����ʒu
	DirectX::SimpleMath::Vector3 m_initialPosition;
	float m_initialAngle;
	// ���݂̈ʒu
	DirectX::SimpleMath::Vector3 m_currentPosition;
	float m_currentAngle;
	// �^�[�Q�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//�m�[�h�z��
	std::vector<std::unique_ptr<IComponent>> m_nodes;
	//�N�H�[�^�j�I��
	DirectX::SimpleMath::Matrix m_worldMatrix;
};
#endif // ROOT_DEFINED

