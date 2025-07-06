/*
	@file	PlayerTitle.h
	@brief	�v���C���[�^�C�g����ԃN���X
*/
#pragma once
#ifndef PLAYER_TITLE_DEFINED
#define PLAYER_TITLE_DEFINED

#include "Game/Interface/IState.h"

// �O���錾
class Player;

class PlayerTitle : public IState
{
public:
   static constexpr DirectX::SimpleMath::Vector3 TARGET_POINT = { 8.5f, 0.0f, 10.5f };			//�ڕW�n�_
   static constexpr float SPEED_CORRECTION = { 0.3f };			//�ڕW�n�_
public:
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_playerVelocity; }
	//�ړ��x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_playerVelocity = vel; }
public:
	//�R���X�g���N�^
    PlayerTitle(Player* player);
	//�f�X�g���N�^
	~PlayerTitle();
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
	//�ڕW�n�_�ɓ��B�������ǂ���
	bool IsTargetPoint();
private:
   //�ړ�����
   void Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime);
private:
	Player* m_player;	//�v���C���[

	DirectX::SimpleMath::Vector3 m_playerVelocity;	//�v���C���[�̃x�N�g��
	DirectX::SimpleMath::Vector3 m_currentTargetPoint;	//���݂̖ڕW�n�_
	DirectX::SimpleMath::Vector3 m_targetPoint1;	//�ڕW�n�_1
	DirectX::SimpleMath::Vector3 m_targetPoint2;	//�ڕW�n�_2
};
#endif // PLAYER_TITLE_DEFINED
