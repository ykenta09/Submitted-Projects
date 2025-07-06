/*
	@file	PlayerSprint.h
	@brief	�v���C���[�����ړ���ԃN���X
*/
#pragma once
#ifndef PLAYER_SPRINT_DEFINED
#define PLAYER_SPRINT_DEFINED

#include "Game/Interface/IState.h"

// �O���錾
class player;

class PlayerSprint : public IState
{
public:
   static constexpr float MAX_MOVEMENT_SPEED = { 18.0f };	   // �ő�ړ����x
   static constexpr float ACCELERATION_DURATION = { 0.2f };	   // �ő呬�x�ɓ��B����܂ł̉�������
   static constexpr float MOVEMENT_DURATION = { 0.5f };		   // �����ړ��̈ړ�����
public:
	//������Ԃ̊m�F
	bool GetIsSprint() const { return m_isSprint; }
public:
	//�R���X�g���N�^
	PlayerSprint(Player* player);
	//�f�X�g���N�^
	~PlayerSprint();
	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render() ;
	//�㏈������
	void Finalize();
	//�����ړ��̏���������
	void PreSprint(const DirectX::SimpleMath::Vector3& dir);
private:
   //�����ړ�
   void Sprint(const float& elapsedTime);
   //�ړ��������烉�W�A���p�x�ɕϊ�
   float GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec);
   //�������邩�̔��������
	void IsFloor();
private:
	Player* m_player;	//�v���C���[

	float m_sprintStartTime;	//�����ړ��̊J�n����
	float m_moveAngle;	//�ړ����̉�]�p

	DirectX::SimpleMath::Vector3 m_sprintDirection;	//�����ړ��̕���
	DirectX::SimpleMath::Matrix m_playerMatrix;	//�v���C���[�̈ړ���]�s��
	
	DirectX::SimpleMath::Vector3 m_sprintVelocity;	//�X�v�����g�x�N�g��
	DirectX::SimpleMath::Vector3 m_finalVelocity;	//�x�N�g��
	DirectX::SimpleMath::Vector3 m_previousVector;	//�v���C���[�̑O��̈ړ����̃x�N�g��

	bool m_isSprint;	//�����ړ��̃t���O
};
#endif // PLAYER_SPRINT_DEFINED
