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

const float EnemyChasing::LOST_TIME = 5.0f;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
EnemyChasing::EnemyChasing(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_lostTime(0.0f),
    m_isArrived(true)
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyChasing::~EnemyChasing()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void EnemyChasing::Initialize()
{	
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void EnemyChasing::Update(const float& elapsedTime)
{
    //�^�[�Q�b�g��ǐՂ���
    Move(elapsedTime);
	if (!m_enemy->InView(false))
	{
		LostTarget(elapsedTime);
	}
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void EnemyChasing::Render()
{
#ifdef _DEBUG
   // �`��J�n
   m_graphics->GetPrimitiveBatch()->Begin();

   std::vector<int> path = m_enemy->GetShortestPath();
   auto vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList();

   //�œK���O�̍ŒZ�o�H
   if (path.size() > 1) // ���_�������Ȃ��ƕ`�悷��������݂��Ȃ�
   {
	  for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i)
	  {
		 // path[i] �� path[i+1] �̊Ԃɐ���`�悷��
		 m_graphics->GetPrimitiveBatch()->DrawLine(
			DirectX::VertexPositionColor(vertexList[path[i]].GetPosition(), DirectX::Colors::White),
			DirectX::VertexPositionColor(vertexList[path[i + 1]].GetPosition(), DirectX::Colors::White));
	  }
   }

   //�œK�������ŒZ�o�H
   std::vector<int> shotPath = m_enemy->GetAStar()->GetShortestPath();
   if (shotPath.size() > 1) // ���_�������Ȃ��ƕ`�悷��������݂��Ȃ�
   {
	  for (int i = 0; i < static_cast<int>(shotPath.size()) - 1; ++i)
	  {
		 // path[i] �� path[i+1] �̊Ԃɐ���`�悷��
		 m_graphics->GetPrimitiveBatch()->DrawLine(
			DirectX::VertexPositionColor(vertexList[shotPath[i]].GetPosition(), DirectX::Colors::Blue),
			DirectX::VertexPositionColor(vertexList[shotPath[i + 1]].GetPosition(), DirectX::Colors::Blue));
	  }
   }

   // �`��I��
   m_graphics->GetPrimitiveBatch()->End();
#endif // _DEBUG
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void EnemyChasing::Finalize()
{
	// do nothing.
}

void EnemyChasing::Move(const float& elapsedTime)
{
   // ���_���X�g���擾����
   const std::vector<CellVertex>& vertexList = m_enemy->GetAStar()->GetVertexList();

   // �E�F�C�|�C���g�C���f�b�N�X���ŒZ�o�H�z��̃T�C�Y��菬�����ꍇ
   if (m_enemy->GetWayPointIndex() < (int)m_enemy->GetShortestPath().size() - 1)
   {
	  m_isArrived = false;

	  DirectX::SimpleMath::Vector3 currentPosition = m_enemy->GetPosition();
	  DirectX::SimpleMath::Vector3 nextPosition = vertexList[m_enemy->GetShortestPath()[m_enemy->GetWayPointIndex() + 1]].GetPosition();

	  // ���̒��_�ւ̕��������߂�
	  DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  // �������v�Z
	  float distanceToNext = direction.Length();

	  // ������0�łȂ��ꍇ�͐��K���i�����x�N�g����P�ʃx�N�g���ɂ���j
	  if (distanceToNext > 0.0f)
	  {
		 direction.Normalize();
	  }

	  // �i�ނׂ����x�x�N�g�����v�Z
	  DirectX::SimpleMath::Vector3 velocity = direction * Enemy::MOVE_SPEED * elapsedTime;

	  // ���ۂɐi�ދ������v�Z�i���̒��_�ɋ߂Â������Ȃ��悤�ɂ���j
	  if (velocity.Length() >= distanceToNext)
	  {
		 // ���̒��_��ʉ߂܂��͓��B����ꍇ
		 m_enemy->SetPosition(nextPosition); // ���̒��_�ɒ��ڈړ�
		 m_enemy->SetWayPointIndex(m_enemy->GetWayPointIndex() + 1); // �E�F�C�|�C���g���X�V
	  }
	  else
	  {
		 // �ʏ�̈ړ�
		 m_enemy->SetPosition(currentPosition + velocity);
	  }

	  // �i�s�����ɉ����ĉ�]��ݒ�
	  m_enemy->SetAngle(atan2(direction.x, direction.z));
   }
}

//�^�[�Q�b�g����������
void EnemyChasing::LostTarget(float elapsedTime)
{
	m_lostTime += elapsedTime;

	if (m_lostTime > EnemyChasing::LOST_TIME)
	{
	   //�G�m�F�ӏ�
		//��Ԃ�ύX����		
	    m_enemy->ChangeState(m_enemy->GetEnemyRouteSearch());
		m_lostTime = 0;
		//�o�H�T�����s��
		m_enemy->GetEnemyRouteSearch()->SetAstar(m_enemy->GetEnemyWandering()->GetWanderTarget());
	}
}
