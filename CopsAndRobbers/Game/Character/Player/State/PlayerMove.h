/*
	@file	PlayerMove.h
	@brief	�v���C���[�ړ���ԃN���X
*/
#pragma once
#ifndef PLAYER_MOVE_DEFINED
#define PLAYER_MOVE_DEFINED

#include "Game/Interface/IState.h"

// �O���錾
class Player;

class PlayerMove : public IState
{
public:
    //�x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//�ړ��x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
public:
	//�R���X�g���N�^
	PlayerMove(Player* player);
	//�f�X�g���N�^
	~PlayerMove();
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
   void Move(const float& elapsedTime);
   //�ړ������ɏ������邩�̔���ƈړ��x�N�g���̒���
   void IsFloor();
private:
	Player* m_player;	//�v���C���[
	DirectX::SimpleMath::Quaternion m_rotationPlayerAngle;	// ��]�p
	DirectX::SimpleMath::Quaternion m_rotationPlayerMoveAngle;	// �ړ������̉�]�p
	DirectX::SimpleMath::Vector3 m_velocity;		  //�ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_finalVelocity;	  //�ŏI�I�ȃx�N�g��

	//�v���C���[�̈ړ���]�s��
	DirectX::SimpleMath::Matrix m_playerMoveMatrix;
	DirectX::SimpleMath::Matrix m_playerMatrix;

};
#endif // PLAYER_MOVE_DEFINED
