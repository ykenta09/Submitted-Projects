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
#include "Libraries/yamadaLib/GameParameter.h"

// �O���錾
class CommonResources;
class GraphScene;
class Enemy;

class EnemyWandering : public IState
{
public:
    //�O���t�̃f�[�^���󂯎��
    void SetGraphDate(std::vector<DirectX::SimpleMath::Vector3>& patrolMap, std::unordered_map<int, GameParameters::PatrolRoute> adjacenyList, int initialNumber);
	//�G�̏������W���擾����
	const DirectX::SimpleMath::Vector3& GetInitialPosition() { return m_patrolRouteMap[m_initalNumber]; }
	//����}�b�v���擾����
	const std::vector<DirectX::SimpleMath::Vector3> GetPatrolMap() { return m_patrolRouteMap; }
	//�ڕW�n�_���擾����
	DirectX::SimpleMath::Vector3& GetWanderTarget() { return m_wanderTarget; }
	//�ړ���̒��_�ԍ����擾����
	int GetRootNumber() { return m_currentRootNumber;  }
public:
	//�R���X�g���N�^
	EnemyWandering(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyWandering();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//��n������
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
	Enemy* m_enemy;							 //�G
	CommonResources* m_commonResources;		 // ���ʃ��\�[�X
	GraphScene* m_graphScene;				 //�O���t�f�[�^
	yamadalib::Graphics* m_graphics;		 //�O���t�B�b�N�X�@�f�o�b�N�p

	// === ����o�H�Ǘ� ===
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRouteMap;						//���񃋁[�g�}�b�v
	std::unordered_map<int, GameParameters::PatrolRoute> m_patrolAdjacencyList;		//�ʂ̃��[�g�}�b�v
	std::vector<int> m_passedRoutNumber;											//�ʂ������[�g�}�b
	int m_currentRootNumber;	 //�ړ��ڕW�̌o�H�ԍ�
	int m_previousRootNumber;	 //�ړ����̌o�H�ԍ�
	int m_initalNumber;			 //�G�̏������W�̒��_�ԍ�
	bool m_noFoundRoute;		 //���������o�H�������邱�Ƃ��ł�����

	// === �ړ���� ===
	DirectX::SimpleMath::Vector3 m_wanderTarget;	// ����̖ڕW�n�_
	float m_moveProgress;						    //�ړ��̐i�s�x

	int m_enemyNumber;			 //�G�ԍ�
};
#endif // ENEMY_WANDERING_DEFINED
