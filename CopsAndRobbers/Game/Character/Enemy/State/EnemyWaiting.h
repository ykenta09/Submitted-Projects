/*
	@file	EnemyWaiting.h
	@brief	�G�̜p�j��ԃN���X
*/
#ifndef ENEMY_WAITING_DEFINED
#define ENEMY_WAITING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>

// �O���錾
class CommonResources;
class Enemy;

class EnemyWaiting : public IState
{
public:
	//�R���X�g���N�^
	EnemyWaiting(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyWaiting();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();
private:
	//���̈ړ�������ɉ�]
	void RotateToNextDestination(const float elapsedTime);
private:
	Enemy* m_enemy;						  //�G
	CommonResources* m_commonResources;	  // ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;	  //�O���t�B�b�N�X�@

	float m_waitTime; 	// �ҋ@���Ԃ�ێ�����ϐ�
	bool m_isRotationComplete;	//��]���I���������̃t���O
};
#endif // ENEMY_WAITING_DEFINED
