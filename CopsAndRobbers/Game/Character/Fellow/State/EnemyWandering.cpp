/*
	@file	EnemyWandering.cpp
	@brief	�G�̒ǐՃN���X
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyWandering.h"
#include <random>
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
EnemyWandering::EnemyWandering(Enemy* enemy)
	:
	m_enemy(enemy),
	m_patrolRouteMap{},					
	m_patrolAdjacencyList{},			
	m_passedRoutNumber{},
	m_graphScene{},
	m_enemyNumber( 0 ),
    m_currentRootNumber( 0 ),
    m_previousRootNumber( 0 ),
    m_moveProgress( 0 ),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_noFoundRoute( false ),
	m_wanderTarget(DirectX::SimpleMath::Vector3::Zero)
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyWandering::~EnemyWandering()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void EnemyWandering::Initialize()
{	
	//����o�H���擾����
	InitialPatrolRoute();
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void EnemyWandering::Update(const float& elapsedTime)
{
	//����n�_�Ɉړ����������珄��ڕW���X�V����
	if (IsArriveDestination())
	{
		m_enemy->ChangeState(m_enemy->GetEnemyWaiting());
		UpDaatePatrolRoute();
	}
	else
	{
		Move(m_wanderTarget, elapsedTime);
		//�ړ������̌v�Z
		MovementRatio();
	}
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void EnemyWandering::Render()
{

#ifdef _DEBUG

#endif // _DEBUG
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void EnemyWandering::Finalize()
{
	// do nothing.
}

//�󂯎�����n�_�Ɉړ�������
void EnemyWandering::Move(const DirectX::SimpleMath::Vector3& target, const float& elpsedTime)
{
	// �^�[�Q�b�g�܂ł̕����x�N�g�����v�Z����
	DirectX::SimpleMath::Vector3 direction = target - m_enemy->GetPosition();
	// ���̋����ɋ߂Â��Ǝ~�܂�
	if (direction.Length() <= Enemy::STOPPING_DISTANCE) return;
	direction.Normalize();
	// �G�̑��x���v�Z����
	DirectX::SimpleMath::Vector3 enemyVelocity = direction * Enemy::MOVE_SPEED * elpsedTime;
	// �G�̍��W���X�V����
	m_enemy->SetPosition(m_enemy->GetPosition() + enemyVelocity);

	// �G�̐i�s�����Ɍ�����
	if (direction.LengthSquared() > 0.0f)  // �����x�N�g�����[���łȂ��ꍇ
	{
	   // �V�����������v�Z����
		m_enemy->SetAngle(atan2(direction.x, direction.z));
	}
}


//����o�H���X�V����
void EnemyWandering::UpDaatePatrolRoute()
{
	//���̌o�H�ԍ���I�����邽�߂̌�⃊�X�g
	const std::vector<int> availableRoutes = m_patrolAdjacencyList[m_enemyNumber].adjacencyList[m_currentRootNumber];
	//���������z��̗v�f�ԍ�
	int count = 0;
	
	//���񂷂�o�H��������Ȃ������ꍇ
	if (!m_noFoundRoute)
	{
		//�ʂ�ʘH�����݂���ꍇ�A�ʂ����o�H���ŏ��̗v�f�ɓo�^����
		m_passedRoutNumber.insert(m_passedRoutNumber.begin(), m_currentRootNumber);
		for (int route : availableRoutes)
		{
			//m_passedRoutNumbe�Ɋ܂܂�Ă��Ȃ��ԍ�������
			if (std::find(m_passedRoutNumber.begin(), m_passedRoutNumber.end(), route) == m_passedRoutNumber.end())
			{
			    //�O��̌o�H�ԍ����X�V����
			    m_previousRootNumber = m_currentRootNumber;
				//���̌o�H�ԍ���אڃ��X�g����擾����
			    m_currentRootNumber = m_patrolAdjacencyList[m_enemyNumber].adjacencyList[m_currentRootNumber][count];
				//�ړ��ڕW�n�_��ݒ肷��
				m_wanderTarget = m_patrolRouteMap[m_currentRootNumber];
				//�ړ������̉�]���v�Z����
				DirectX::SimpleMath::Vector3 direction = m_wanderTarget - m_enemy->GetPosition();
				//direction.Normalize();
				//m_enemy->SetAngle(atan2(direction.x, direction.z));
				//�ڕW�n�_�����������珈�����I��
				return;
			}
			count++;
		}
	}
	//���񂷂�o�H�����݂��Ȃ�������A�ʂ����ʘH�����񂷂�
	m_noFoundRoute = true;
	if (!m_passedRoutNumber.empty())
	{
	    m_currentRootNumber = m_passedRoutNumber.front();
		m_wanderTarget = m_patrolRouteMap[m_currentRootNumber];
		m_passedRoutNumber.erase(m_passedRoutNumber.begin());
	}
	else
	{
		m_noFoundRoute = false;
	}
}

//����o�H�̖ڕW�n�_�ɓ���������
bool EnemyWandering::IsArriveDestination()
{
	DirectX::SimpleMath::Vector3 patrolPosition = DirectX::SimpleMath::Vector3(m_wanderTarget.x, roundf(m_enemy->GetPosition().y), m_wanderTarget.z);
	// patrolPosition �� enemyPosition �̋������v�Z
	float distance = DirectX::SimpleMath::Vector3::Distance(patrolPosition, m_enemy->GetPosition());

	// ������ 1 �ȉ����ǂ������`�F�b�N
	if (distance <= 1.5f)
		return true;

	return false;
}

//�������W�A���񃋁[�g������������
void EnemyWandering::InitialPatrolRoute()
{
   if (m_graphScene == nullptr) return;
	m_patrolRouteMap = m_graphScene->GetPatrolRootMap();					//�o�H�}�b�v�̍��W���擾����
	m_patrolAdjacencyList = m_graphScene->GetAdjacencyList();				//����אڃ��X�g���擾����

	m_initalNumber = m_graphScene->GetInitalVertexNumbeer(m_enemyNumber);		//�G�ԍ��ɑΉ������������W�̒��_�ԍ����擾����
	m_wanderTarget = m_patrolRouteMap[m_initalNumber];							//�����ړ�����擾����
	m_currentRootNumber = m_initalNumber;
	m_previousRootNumber = m_currentRootNumber;
}

//�ړ��������v�Z����
void EnemyWandering::MovementRatio()
{
   //����J�n�n�_�ƖڕW�n�_
   const DirectX::SimpleMath::Vector3 startPosition = m_patrolRouteMap[m_previousRootNumber];
   const DirectX::SimpleMath::Vector3 endPosition = m_wanderTarget;

   // �J�n�n�_����ڕW�n�_�܂ł̑S����
   float totalDistance = DirectX::SimpleMath::Vector3::Distance(startPosition, endPosition);
   if (totalDistance <= 0.0f) // �������[���̏ꍇ�A������0
	  return;

   //�J�n�n�_����ڕW�n�_�܂ł̑S����
   float totalDistatnce = DirectX::SimpleMath::Vector3::Distance(startPosition, endPosition);

   //�J�n�n�_���猻�݈ʒu�܂ł̋���
   float travelDistance = DirectX::SimpleMath::Vector3::Distance(startPosition, m_enemy->GetPosition());

   //�ړ��������v�Z
   m_moveProgress = travelDistance / totalDistatnce;
}

//�O���t�̃f�[�^���󂯎��
void EnemyWandering::SetGraphDate(GraphScene* graph, int number)
{
	m_graphScene = graph;
	m_enemyNumber = number;
}

