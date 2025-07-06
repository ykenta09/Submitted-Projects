/*
	@file	EnemyTitle.h
	@brief	�G�̃^�C�g����ԃN���X
*/
#pragma once
#ifndef ENEMY_TITLE_DEFINED
#define ENEMY_TITLE_DEFINED

#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;
class Enemy;

class EnemyTitle : public IState
{
public:
   static constexpr DirectX::SimpleMath::Vector3 TARGET_POINT1 = { 8.5f, 0.0f, 10.5f };     //�ڕW�n�_�̍��W1
   static constexpr DirectX::SimpleMath::Vector3 TARGET_POINT2 = { -8.5f, 0.0f, 10.5f };    //�ڕW�n�_�̍��W2
   static constexpr float TITLE_MOVE_SPEED = { 1.95f };    //�^�C�g���̓G�ړ����x
public:
   //�ړ��x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//�ړ��x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }
public:
	//�R���X�g���N�^
   EnemyTitle(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyTitle();
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
    CommonResources* m_commonResources;	  // ���ʃ��\�[�X
	Enemy* m_enemy;						  //�G
	yamadalib::Graphics* m_graphics;	  //�O���t�B�b�N�X

	DirectX::SimpleMath::Vector3 m_velocity;			 //�x�N�g��
	DirectX::SimpleMath::Vector3 m_currentTargetPoint;	 //���݂̖ڕW�n�_

	// === �ړ�����ڕW�n�_ ===
	DirectX::SimpleMath::Vector3 m_targetPoint1;		 //�ڕW�n�_1
	DirectX::SimpleMath::Vector3 m_targetPoint2;		 //�ڕW�n�_2
};
#endif // ENEMY_TITLE_DEFINED
