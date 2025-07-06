/*
	@file	EnemyWandering.h
	@brief	�G�̜p�j��ԃN���X
*/
#ifndef ENEMY_WANDERING_DEFINED
#define ENEMY_WANDERING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// �O���錾
class CommonResources;
class GraphScene;
class Enemy;


class EnemyWandering : public IState
{
public:
	void SetGraphDate(GraphScene* graph, int number);
	//�G�̏������W���擾����
	const DirectX::SimpleMath::Vector3& GetInitialPosition() { return m_patrolRouteMap[m_initalNumber]; }
	//�ڕW�n�_���擾����
	DirectX::SimpleMath::Vector3& GetWanderTarget() { return m_wanderTarget; }
	//�ړ���̒��_�ԍ����擾����
	int GetRootNumber() { return m_currentRootNumber;  }
	//���݂̒��_�ԍ����擾����
	int GetCurrentNumber();
public:
	//�R���X�g���N�^
	EnemyWandering(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyWandering();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
	//�󂯎�����ڕW�n�_�Ɉړ�����
	void  Move(const DirectX::SimpleMath::Vector3& target, const float& elapsedTime);
	//����o�H�̍X�V
	void UpDaatePatrolRoute();
	//������W�ɓ���������
	bool IsArriveDestination();
	//���񃋁[�g������������
	void InitialPatrolRoute();
	//�ړ��������v�Z����
	void MovementRatio();
private:
	//�G
	Enemy* m_enemy;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�f�[�^
	GraphScene* m_graphScene;
	//�O���t�B�b�N�X�@�f�o�b�N�p
	yamadalib::Graphics* m_graphics;

	//�^�[�Q�b�g�̍��W
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//�G�̏��񃋁[�g�}�b�v
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRouteMap;
	//�G�̌ʂ̃��[�g�}�b�v
	std::unordered_map<int, Graph::PatrolRoute> m_patrolAdjacencyList;
	//�G���ʂ������[�g�}�b�v
	std::vector<int> m_passedRoutNumber;
	//�ړ��ڕW�̌o�H�ԍ�
	int m_currentRootNumber;
	//�ړ����̌o�H�ԍ�
	int m_previousRootNumber;
	//���������o�H�������邱�Ƃ��ł�����
	bool m_noFoundRoute;
	//�G�ԍ�
	int m_enemyNumber;
	//�G�̏������W�̒��_�ԍ�
	int m_initalNumber;
	// ����̖ڕW�n�_
	DirectX::SimpleMath::Vector3 m_wanderTarget;
	//�ړ��̐i�s�x
	float m_moveProgress;
};
#endif // ENEMY_WANDERING_DEFINED
