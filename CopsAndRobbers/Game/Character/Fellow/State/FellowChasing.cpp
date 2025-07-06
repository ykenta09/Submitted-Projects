/*
	@file	FellowChasing.cpp
	@brief	���Ԃ̒ǐՃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <Libraries/yamadaLib/Resources.h>

#include "Game/Screen.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Fellow/State/FellowChasing.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fellow">�{�̂̃|�C���^</param>
FellowChasing::FellowChasing(Fellow* fellow)
	:
	m_fellow(fellow),
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_isArrived(true),
    m_isGoal(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FellowChasing::~FellowChasing()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void FellowChasing::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void FellowChasing::Update(const float& elapsedTime)
{
   //�ĒT�����K�v���ǂ���
   Search();

   if (!m_isGoal)
   {
	  m_targetPosition = m_fellow->GetTargetPosition();
	  //�^�[�Q�b�g��ǐՂ���
	  Move(elapsedTime);
   }
   //�ړI�n�ɒB������A�C�h����ԂɕύX
   if (m_isArrived)
   {
	  m_fellow->ChangeState(m_fellow->GetFellowIdle());
   }
}


/// <summary>
/// �`�悷��
/// </summary>
void FellowChasing::Render()
{

#ifdef _DEBUG
#endif // _DEBUG
}

/// <summary>
/// ��n������
/// </summary>
void FellowChasing::Finalize()
{
	// do nothing.
}

/// <summary>
/// �󂯎�����n�_�Ɉړ�������
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void FellowChasing::Move(const float& elapsedTime)
{
   //�p�X���擾����
   const auto& path = m_fellow->GetMultiStagePath();
   int wayPoint = m_fellow->GetWayPointIndex();

   if (m_fellow->GetWayPointIndex() < (int)path.size() - 1)
   {
	  m_isArrived = false;
	  //���݂̃X�e�[�W�E�m�[�h
	  int nextStage = path[wayPoint + 1].stage;
	  int nextNode = path[wayPoint+ 1].node;

	  //���_���X�g����CellVertex���擾����
	  const auto& nextVertexList = m_fellow->GetFellowRouteSearch()->GetGrid()->GetVertexList(nextStage);

	  //�͈̓`�F�b�N
	  if (nextNode < 0 || nextNode >= nextVertexList.size()) return;

	  //�O���t������W���擾����
	  const CellVertex& nextVertex = nextVertexList[nextNode];
	  DirectX::SimpleMath::Vector3 currentPosition = m_fellow->GetPosition();
	  DirectX::SimpleMath::Vector3 nextPosition = nextVertex.GetPosition();

	  //�^�[�Q�b�g�ւ̕����Ƌ������v�Z
	  DirectX::SimpleMath::Vector3 targetDirection = m_targetPosition - currentPosition;
	  float distanceToTarget = targetDirection.Length();

	  //���������p�����[�^�̐ݒ�
	  const float decelerationStartDistance = Fellow::STOPPING_DISTANCE * 3.0f;

	  //�����Ɋ�Â������x�v�Z
	  float speed = Fellow::MOVE_SPEED;
	  //�^�[�Q�b�g�̋����������̂����鋗���͈͂Ȃ猸������������
	  if (distanceToTarget < Fellow::STOPPING_DISTANCE)
	  {
		 float t = (distanceToTarget - Fellow::STOPPING_DISTANCE) / (decelerationStartDistance - Fellow::STOPPING_DISTANCE);
		 speed *= std::max(t, 0.0f); // 0�����ɂȂ�Ȃ��悤��	  
	  }

	  //��~�����ȓ��Ȃ�ړ����Ȃ�
	  if (distanceToTarget <= Fellow::STOPPING_DISTANCE) return;

	  //���̃m�[�h����
	  DirectX::SimpleMath::Vector3 direction = nextPosition - currentPosition;
	  float distanceToNext = direction.Length();
	  direction.Normalize();
	  DirectX::SimpleMath::Vector3 velocity = direction * Fellow::MOVE_SPEED * elapsedTime;

	  if (distanceToNext < velocity.Length())
	  {
		 m_fellow->SetPosition(nextPosition);
		 m_fellow->SetWayPointIndex(m_fellow->GetWayPointIndex() + 1);
	  }
	  else
	  {
		 m_fellow->SetPosition(currentPosition + velocity);
	  }

	  // �i�s�����ɉ����ĉ�]��ݒ�
	  m_fellow->SetAngle(atan2(direction.x, direction.z));
   }
   else 
   {
	  m_isArrived = true;
   }
}


/// <summary>
/// �󂯎�����n�_�Ɉړ�������
/// </summary>
/// <param name="target">�ړ���̍��W</param>
/// <param name="elapsedTime">�o�ߎ���</param>
void FellowChasing::TargetMove(const DirectX::SimpleMath::Vector3& target, const float& elapsedTime)
{
   // �G�̌��݂̍��W���v�Z
   DirectX::SimpleMath::Vector3 position = m_fellow->GetPosition();
   // �^�[�Q�b�g�܂ł̕����x�N�g�����v�Z����
   DirectX::SimpleMath::Vector3 direction = target - position;
   // ���̋����ɋ߂Â��Ǝ~�܂�
   if (direction.Length() <= Fellow::STOPPING_DISTANCE) return;
   direction.Normalize();
   // �G�̑��x���v�Z����
   DirectX::SimpleMath::Vector3 enemyVelocity = direction * Fellow::MOVE_SPEED * elapsedTime;
   // �G�̍��W���X�V����
   m_fellow->SetPosition(m_fellow->GetPosition() + enemyVelocity);

   // �G�̐i�s�����Ɍ�����
   if (direction.LengthSquared() > 0.0f)  // �����x�N�g�����[���łȂ��ꍇ
   {
	  // �V�����������v�Z����
	  m_fellow->SetAngle(atan2(direction.x, direction.z));
   }
}


/// <summary>
/// ���G
/// </summary>
void FellowChasing::Search()
{
   if (m_fellow->GetFellowRouteSearch()->GetGrid()->GetVertexFromPosition(m_fellow->GetFellowRouteSearch()->GetTargetStageNumber(), m_targetPosition) == nullptr) return;
   //�^�[�Q�b�g�̍��W���ς�����Ƃ��ĒT��
   auto cell = m_fellow->GetFellowRouteSearch()->GetTargetCell();
   auto vertexNumber = cell->GetVertexNumber();

   if (vertexNumber != m_fellow->GetFellowRouteSearch()->GetGrid()->GetVertexFromPosition(m_fellow->GetFellowRouteSearch()->GetTargetStageNumber(), m_targetPosition)->GetVertexNumber())
   {
	  m_fellow->ChangeState(m_fellow->GetFellowRouteSearch());
	  m_fellow->GetFellowRouteSearch()->SetSearchTime(0);
	  m_fellow->GetFellowRouteSearch()->SetAstar(m_targetPosition);
   }
}
