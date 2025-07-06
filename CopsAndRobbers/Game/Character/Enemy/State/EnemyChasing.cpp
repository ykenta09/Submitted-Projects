/*
	@file	EnemyChasing.cpp
	@brief	�G�̒ǐՃN���X
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyChasing.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemy">�G�̃|�C���^</param>
EnemyChasing::EnemyChasing(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_lostTime(0.0f),
    m_isArrived(true)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyChasing::~EnemyChasing()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
void EnemyChasing::Initialize()
{	
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyChasing::Update(const float& elapsedTime)
{
    ////�ĒT�����������
    //Search();

    //�^�[�Q�b�g��ǐՂ���
    Move(elapsedTime);
	if (!m_enemy->InView(false))
	{
		LostTarget(elapsedTime);
	}
	if (m_isArrived)
	{
	   m_enemy->ChangeState(m_enemy->GetEnemyWaiting());
	}
}


/// <summary>
/// �`�悷��
/// </summary>
void EnemyChasing::Render()
{
#ifdef _DEBUG
   //// �`��J�n
   //m_graphics->GetPrimitiveBatch()->Begin();

   //std::vector<int> path = m_enemy->GetShortestPath();
   //auto vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(m_enemy->GetObjectStageNumber());

   ////�œK���O�̍ŒZ�o�H
   //if (path.size() > 1) // ���_�������Ȃ��ƕ`�悷��������݂��Ȃ�
   //{
	  //for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i)
	  //{
		 //// path[i] �� path[i+1] �̊Ԃɐ���`�悷��
		 //m_graphics->GetPrimitiveBatch()->DrawLine(
			//DirectX::VertexPositionColor(vertexList[path[i]].GetPosition(), DirectX::Colors::White),
			//DirectX::VertexPositionColor(vertexList[path[i + 1]].GetPosition(), DirectX::Colors::White));
	  //}
   //}

   ////�œK�������ŒZ�o�H
   //std::vector<int> shotPath = m_enemy->GetAStar()->GetShortestPath();
   //if (shotPath.size() > 1) // ���_�������Ȃ��ƕ`�悷��������݂��Ȃ�
   //{
	  //for (int i = 0; i < static_cast<int>(shotPath.size()) - 1; ++i)
	  //{
		 //// path[i] �� path[i+1] �̊Ԃɐ���`�悷��
		 //m_graphics->GetPrimitiveBatch()->DrawLine(
			//DirectX::VertexPositionColor(vertexList[shotPath[i]].GetPosition(), DirectX::Colors::Blue),
			//DirectX::VertexPositionColor(vertexList[shotPath[i + 1]].GetPosition(), DirectX::Colors::Blue));
	  //}
   //}

   //// �`��I��
   //m_graphics->GetPrimitiveBatch()->End();
#endif // _DEBUG
}


/// <summary>
/// ��n������
/// </summary>
void EnemyChasing::Finalize()
{
	// do nothing.
}


/// <summary>
/// �󂯎�����ڕW�n�_�Ɉړ�����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyChasing::Move(const float& elapsedTime)
{
   //�p�X���擾����
   const auto& path = m_enemy->GetMultiStagePath();
   int wayPoint = m_enemy->GetWayPointIndex();

   if (m_enemy->GetWayPointIndex() < (int)path.size() - 1)
   {
	  m_isArrived = false;
	  //���݂̃X�e�[�W�E�m�[�h
	  int nextStage = path[wayPoint + 1].stage;
	  int nextNode = path[wayPoint + 1].node;

	  //���_���X�g����CellVertex���擾����
	  const auto& nextVertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(nextStage);

	  //�͈̓`�F�b�N
	  if (nextNode < 0 || nextNode >= nextVertexList.size()) return;

	  //�O���t������W���擾����
	  const CellVertex& nextVertex = nextVertexList[nextNode];
	  DirectX::SimpleMath::Vector3 currentPosition = m_enemy->GetPosition();
	  DirectX::SimpleMath::Vector3 nextPosition = nextVertex.GetPosition();

	  //�^�[�Q�b�g�ւ̕����Ƌ������v�Z
	  DirectX::SimpleMath::Vector3 targetDirection = m_targetPosition - currentPosition;
	  float distanceToTarget = targetDirection.Length();

	  //���������p�����[�^
	  const float decelerationStartDistance = Enemy::STOPPING_DISTANCE * 3.0f;
	  //�����Ɋ�Â������x�v�Z
	  float speed = Enemy::MOVE_SPEED;
	  if (distanceToTarget < Enemy::STOPPING_DISTANCE)
	  {
		 float t = (distanceToTarget - Enemy::STOPPING_DISTANCE) / (decelerationStartDistance - Enemy::STOPPING_DISTANCE);
		 speed *= std::max(t, 0.0f); // 0�����ɂȂ�Ȃ��悤��	  
	  }
	  //��~�����ȓ��Ȃ�ړ����Ȃ�
	  if (distanceToTarget <= Enemy::STOPPING_DISTANCE) return;

	  //���̃m�[�h����
	  DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  float distanceToNext = direction.Length();
	  direction.Normalize();
	  DirectX::SimpleMath::Vector3 velocity = direction * Enemy::MOVE_SPEED * elapsedTime;

	  if (distanceToNext < velocity.Length())
	  {
		 m_enemy->SetPosition(nextPosition);
		 m_enemy->SetWayPointIndex(m_enemy->GetWayPointIndex() + 1);
	  }
	  else
	  {
		 m_enemy->SetPosition(currentPosition + velocity);
	  }

	  // �i�s�����ɉ����ĉ�]��ݒ�
	  m_enemy->SetAngle(atan2(direction.x, direction.z));
   }
   else
   {
	  m_isArrived = true;
   }

   //// ���_���X�g���擾����
   //const std::vector<CellVertex>& vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(m_enemy->GetObjectStageNumber());

   //// �E�F�C�|�C���g�C���f�b�N�X���ŒZ�o�H�z��̃T�C�Y��菬�����ꍇ
   //if (m_enemy->GetWayPointIndex() < (int)m_enemy->GetShortestPath().size() - 1)
   //{
	  //m_isArrived = false;

	  //DirectX::SimpleMath::Vector3 currentPosition = m_enemy->GetPosition();
	  //DirectX::SimpleMath::Vector3 nextPosition = vertexList[m_enemy->GetShortestPath()[m_enemy->GetWayPointIndex() + 1]].GetPosition();

	  //// ���̒��_�ւ̕��������߂�
	  //DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  //// �������v�Z
	  //float distanceToNext = direction.Length();

	  //// ������0�łȂ��ꍇ�͐��K���i�����x�N�g����P�ʃx�N�g���ɂ���j
	  //if (distanceToNext > 0.0f)
	  //{
		 //direction.Normalize();
	  //}

	  //// �i�ނׂ����x�x�N�g�����v�Z
	  //DirectX::SimpleMath::Vector3 velocity = direction * Enemy::MOVE_SPEED * elapsedTime;

	  //// ���ۂɐi�ދ������v�Z�i���̒��_�ɋ߂Â������Ȃ��悤�ɂ���j
	  //if (velocity.Length() >= distanceToNext)
	  //{
		 //// ���̒��_��ʉ߂܂��͓��B����ꍇ
		 //m_enemy->SetPosition(nextPosition); // ���̒��_�ɒ��ڈړ�
		 //m_enemy->SetWayPointIndex(m_enemy->GetWayPointIndex() + 1); // �E�F�C�|�C���g���X�V
	  //}
	  //else
	  //{
		 //// �ʏ�̈ړ�
		 //m_enemy->SetPosition(currentPosition + velocity);
	  //}

	  //// �i�s�����ɉ����ĉ�]��ݒ�
	  //m_enemy->SetAngle(atan2(direction.x, direction.z));
   //}
}

/// <summary>
/// �^�[�Q�b�g����������
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyChasing::LostTarget(float elapsedTime)
{
	m_lostTime += elapsedTime;

	if (m_lostTime > EnemyChasing::LOST_TIME)
	{
		//��Ԃ�ύX����		
	    m_enemy->ChangeState(m_enemy->GetEnemyRouteSearch());
		m_lostTime = 0;
		//�o�H�T�����s��
		m_enemy->GetEnemyRouteSearch()->SetAstar(m_enemy->GetEnemyWandering()->GetWanderTarget());
	}
}

/// <summary>
/// ���G
/// </summary>
void EnemyChasing::Search()
{
   if (m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexFromPosition(m_enemy->GetEnemyRouteSearch()->GetTargetStageNumber(), m_targetPosition) == nullptr) return;
   //�^�[�Q�b�g�̍��W���ς�����Ƃ��ĒT��
   auto cell = m_enemy->GetEnemyRouteSearch()->GetTargetCell();
   auto vertexNumber = cell->GetVertexNumber();

   if (vertexNumber != m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexFromPosition(m_enemy->GetEnemyRouteSearch()->GetTargetStageNumber(), m_targetPosition)->GetVertexNumber())
   {
	  m_enemy->ChangeState(m_enemy->GetEnemyRouteSearch());
	  m_enemy->GetEnemyRouteSearch()->SetSearchTime(EnemyRouteSearch::TIME_TO_SEARCH);
	  m_enemy->GetEnemyRouteSearch()->SetAstar(m_targetPosition);
   }
}
