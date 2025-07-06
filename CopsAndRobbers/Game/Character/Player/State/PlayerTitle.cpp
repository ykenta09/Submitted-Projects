/*
	@file	PlayerTitle.cpp
	@brief	�v���C���[�̃^�C�g����ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerTitle.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="player">�v���C���[�{�̂̃|�C���^</param>
PlayerTitle::PlayerTitle(Player* player)
	:
	m_player( player )
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerTitle::~PlayerTitle()
{
   //��n��������
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void PlayerTitle::Initialize()
{
   //�ڕW�n�_�̐ݒ�
   m_targetPoint1 = TARGET_POINT;
   m_targetPoint2 = m_player->GetInitialPosition();
   //�����^�[�Q�b�g�n�_�̐ݒ�
   m_currentTargetPoint = m_targetPoint1;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerTitle::Update(const float& elapsedTime)
{
   //�ړ�����
   Move(m_currentTargetPoint, elapsedTime);

   //�ڕW�n�_�ɓ��B������
   if (IsTargetPoint())
   {
	  //�ڕW�n�_��؂�ւ���
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
   m_playerVelocity = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// �`�悷��
/// </summary>
void PlayerTitle::Render()
{
}

/// <summary>
/// ��n������
/// </summary>
void PlayerTitle::Finalize()
{
}

/// <summary>
/// �ړ��������烉�W�A���p�x�ɕϊ�
/// </summary>
/// <param name="vec">�ړ�����</param>
/// <returns></returns>
float PlayerTitle::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
    //�ړ��̋�����0�Ȃ�p�x��0�ŕԂ�
	if (vec.Length() <= 0) return 0.0f;

	//�ړ���������p�x�����߂�
	float angle = atan2(vec.x, vec.z);
	return angle;
}


/// <summary>
/// �ڕW�n�_�ɓ��B�������ǂ���
/// </summary>
/// <returns></returns>
bool PlayerTitle::IsTargetPoint()
{
   //�ڕW�l�Ƃ̋������Βl�ŋ��߂�
   float pos = std::abs(m_player->GetPosition().x - m_currentTargetPoint.x);
   //�ڕW�l�Ƃ̋������߂��Ȃ����瓞�B�����Ƃ���
   if (pos <= 1.0f)
   {
	  return true;
   }
   return false;
}

/// <summary>
/// �󂯎�����n�_�Ɉړ�������
/// </summary>
/// <param name="target">�^�[�Q�b�g���W</param>
/// <param name="elasedTime">�o�ߎ���</param>
void PlayerTitle::Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime)
{
   // �^�[�Q�b�g�܂ł̕����x�N�g�����v�Z����
   DirectX::SimpleMath::Vector3 initialPos = m_player->GetInitialPosition();
   DirectX::SimpleMath::Vector3 player = m_player->GetPosition();
   DirectX::SimpleMath::Vector3 direction = target - m_player->GetPosition();
   // ���̋����ɋ߂Â��Ǝ~�܂�
   direction.Normalize();
   //���x���v�Z����
   DirectX::SimpleMath::Vector3 Velocity = direction * Player::MOVE_SPEED * SPEED_CORRECTION * elasedTime;
   // ���W���X�V����
   m_player->SetPosition(m_player->GetPosition() + Velocity);

   // �i�s�����Ɍ�����
   if (direction.LengthSquared() > 0.0f)  // �����x�N�g�����[���łȂ��ꍇ
   {
	  // �V�����������v�Z����
	  m_player->SetAngle(atan2(direction.x, direction.z) + m_player->GetInitialAngle());
	  m_player->SetMoveAngle(0.0f);
   }
}
