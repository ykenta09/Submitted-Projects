/*
	@file	PlayerGoal.h
	@brief	�v���C���[�S�[����ԃN���X
*/
#pragma once
#ifndef PLAYER_GOAL_DEFINED
#define PLAYER_GOAL_DEFINED

#include "Game/Interface/IState.h"

// �O���錾
class Player;

class PlayerGoal : public IState
{
public:
   static constexpr DirectX::SimpleMath::Vector3 GOAL_MOVE_POSITION = { 0.0f, 0.0f, 70.0f };	   //�S�[����̈ړ��Ō��������W
   static constexpr float OPEN_TIME = { 3.0f };	   //�����J������܂ł̎���
   static constexpr float END_STATE_TIME = { 5.0f };	   //�X�e�[�g�I������
public:
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_playerVelocity; }
	//�ړ��x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_playerVelocity = vel; }
public:
	//�R���X�g���N�^
    PlayerGoal(Player* player);
	//�f�X�g���N�^
	~PlayerGoal();
	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render() ;
	//�㏈������
	void Finalize();
	//�ړ��x�N�g������p�x�����߂�
	float GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec);
private:
   //�ړ�����
   void Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime);
private:
	Player* m_player;	//�v���C���[

	DirectX::SimpleMath::Vector3 m_playerVelocity;	//�v���C���[�̃x�N�g��
	DirectX::SimpleMath::Vector3 m_goalMovePosition;	//�S�[����̈ړ��Ō��������W

	float m_openTime;	//�����J������܂ł̎��ԊǗ�
	float m_endStateTime;	//�X�e�[�g�I������
};
#endif // PLAYER_GOAL_DEFINED
