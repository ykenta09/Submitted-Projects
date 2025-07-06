/*
	@file	EnemyTitle.cpp
	@brief	�G�̃^�C�g����ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Enemy/State/EnemyTitle.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemy">�G�̃|�C���^</param>
EnemyTitle::EnemyTitle(Enemy* enemy)
	:
    m_enemy( enemy ),
    m_targetPoint1(TARGET_POINT1),
    m_targetPoint2(TARGET_POINT2)
{
    //�O���t�B�b�N�X�̃C���X�^���X���擾
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyTitle::~EnemyTitle()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
void EnemyTitle::Initialize()
{
   //�����^�[�Q�b�g�n�_�̐ݒ�
   m_currentTargetPoint = m_targetPoint1;
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyTitle::Update(const float& elapsedTime)
{
	//�ړ�����
   Move(m_currentTargetPoint, elapsedTime);

   //�ڕW�n�_�ɒB���Ă�����ڕW�n�_��؂�ւ���
   if (IsTargetPoint())
   {
	  if (m_currentTargetPoint != m_targetPoint1)
	  {
		 m_currentTargetPoint = m_targetPoint1;
	  }
	  else
	  {
		 m_currentTargetPoint = m_targetPoint2;
	  }
   }

   //�ړ��x�N�g��������������
   m_velocity = DirectX::SimpleMath::Vector3::Zero;
}


/// <summary>
/// �`�悷��
/// </summary>
void EnemyTitle::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// ��n������
/// </summary>
void EnemyTitle::Finalize()
{
}


/// <summary>
/// �ړ��������烉�W�A���p�x�ɕϊ�
/// </summary>
/// <param name="vec">�ړ�����</param>
/// <returns></returns>
float EnemyTitle::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
	if (vec.Length() <= 0) return 0.0f;

	//XZ���ʂŊp�x�����߂�
	float angle = atan2(vec.x, -vec.z);
	return angle;
}

/// <summary>
/// �ڕW�n�_�ɒB���Ă��邩�ǂ���
/// </summary>
/// <returns></returns>
bool EnemyTitle::IsTargetPoint()
{
   float pos =std::abs(m_enemy->GetPosition().x - m_currentTargetPoint.x);
   if (pos <= 1.0f)
   {
	  return true;
   }
   return false;
}


/// <summary>
/// �󂯎�����n�_�Ɉړ�������
/// </summary>
/// <param name="target">�ړ��ڕW�̍��W</param>
/// <param name="elasedTime">�o�ߎ���</param>
void EnemyTitle::Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime)
{
   // �^�[�Q�b�g�܂ł̕����x�N�g�����v�Z����
   DirectX::SimpleMath::Vector3 initialPos = m_enemy->GetInitialPosition();
   DirectX::SimpleMath::Vector3 player = m_enemy->GetPosition();
   DirectX::SimpleMath::Vector3 direction = target - m_enemy->GetPosition();
   // ���̋����ɋ߂Â��Ǝ~�܂�
   direction.Normalize();
   //���x���v�Z����
   DirectX::SimpleMath::Vector3 Velocity = direction * EnemyTitle::TITLE_MOVE_SPEED * elasedTime;
   // ���W���X�V����
   m_enemy->SetPosition(m_enemy->GetPosition() + Velocity);

   // �i�s�����Ɍ�����
   if (direction.LengthSquared() > 0.0f)  // �����x�N�g�����[���łȂ��ꍇ
   {
	  // �V�����������v�Z����
	  m_enemy->SetAngle(atan2(direction.x, direction.z) + m_enemy->GetInitialAngle());
	  m_enemy->SetMoveAngle(0.0f);
   }
}
