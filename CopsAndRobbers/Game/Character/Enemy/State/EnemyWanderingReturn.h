/*
	@file	EnemyWanderingReturn.h
	@brief	�G�̜p�j�ɖ߂��ԃN���X
*/
#ifndef ENEMY_WANDERING_RETURN_DEFINED
#define ENEMY_WANDERING_RETURN_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// �O���錾
class CommonResources;
class Enemy;

class EnemyWanderingReturn : public IState
{
public:
   static constexpr float SLOW_MOVE_SPEED = { 4.50f };        //�G�̒x���ړ����x
public:
   //�^�[�Q�b�g�̍��W��ݒ肷��
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_targetPosition = target; }
	//�^�[�Q�b�g�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_targetPosition; }
public:
	//�R���X�g���N�^
    EnemyWanderingReturn(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyWanderingReturn();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();
	//�󂯎�����ڕW�n�_�Ɉړ�����
	void  Move(const float& elapsedTime);
private:
	Enemy* m_enemy;						  //�G
	CommonResources* m_commonResources;	  // ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;	  //�O���t�B�b�N�X�@�f�o�b�N�p

	DirectX::SimpleMath::Vector3 m_targetPosition;	//�^�[�Q�b�g�̍��W

	float m_lostTime;	//������������
	bool m_isArrived;	//���_�ɓ��B������
};
#endif // ENEMY_WANDERING_RETURN_DEFINED
