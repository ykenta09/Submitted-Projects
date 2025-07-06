/*
	@file	EnemyWanderingReturn.cpp
	@brief	�G�̜p�j���[�g�ɖ߂�N���X
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyWanderingReturn.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemy">�G�̃|�C���^</param>
EnemyWanderingReturn::EnemyWanderingReturn(Enemy* enemy)
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
EnemyWanderingReturn::~EnemyWanderingReturn()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
void  EnemyWanderingReturn::Initialize()
{	
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void  EnemyWanderingReturn::Update(const float& elapsedTime)
{
   //�^�[�Q�b�g��ǐՂ���
   Move(elapsedTime);
}


/// <summary>
/// �`�悷��
/// </summary>
void  EnemyWanderingReturn::Render()
{
}


/// <summary>
/// ��n������
/// </summary>
void EnemyWanderingReturn::Finalize()
{
	// do nothing.
}


/// <summary>
/// �w�肵���ڕW���W�܂ňړ�
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyWanderingReturn::Move(const float& elapsedTime)
{
   // ���_���X�g���擾����
   const std::vector<CellVertex>& vertexList = m_enemy->GetEnemyRouteSearch()->GetGrid()->GetVertexList(m_enemy->GetObjectStageNumber());

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
	  DirectX::SimpleMath::Vector3 velocity = direction * EnemyWanderingReturn::SLOW_MOVE_SPEED * elapsedTime;

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
   else
   {
	  //�G�m�F�ӏ�
	  m_isArrived = true;
	  m_enemy->ChangeState(m_enemy->GetEnemyWandering());
   }
}
