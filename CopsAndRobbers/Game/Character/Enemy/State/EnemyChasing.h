/*
	@file	EnemyChasing.h
	@brief	�G�̒ǐՏ�ԃN���X
*/
#ifndef ENEMY_CHASING_DEFINED
#define ENEMY_CHASING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// �O���錾
class CommonResources;
class Enemy;

class EnemyChasing : public IState
{
public:
   static constexpr float LOST_TIME = { 5.0f };      //�������܂ł̎���
public:
   //�^�[�Q�b�g�̍��W��ݒ肷��
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_targetPosition = target; }
	//�^�[�Q�b�g�̍��W���擾����
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_targetPosition; }
public:
	//�R���X�g���N�^
	EnemyChasing(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyChasing();

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
   void Search();
   //�^�[�Q�b�g��������
   void LostTarget(float elapsedTime);

private:
    CommonResources* m_commonResources;    // ���ʃ��\�[�X
	Enemy* m_enemy;						   //�G
    yamadalib::Graphics* m_graphics;       //�O���t�B�b�N�X�@�f�o�b�N�p

	DirectX::SimpleMath::Vector3 m_targetPosition;	//�^�[�Q�b�g�̍��W

	float m_lostTime;	//������������
	bool m_isArrived;	//���_�ɓ��B������
};
#endif // ENEMY_CHASING_DEFINED
