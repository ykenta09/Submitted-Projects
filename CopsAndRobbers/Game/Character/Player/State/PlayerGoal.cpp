/*
	@file	PlayerGoal.cpp
	@brief	�v���C���[�̃S�[����ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerGoal.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="player"></param>
PlayerGoal::PlayerGoal(Player* player)
	:
	m_player( player )
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerGoal::~PlayerGoal()
{
   //��n��������
   Finalize();
}

/// <summary>
/// ����������
/// </summary>
void PlayerGoal::Initialize()
{
   //�����l�̐ݒ�
   m_openTime = OPEN_TIME;
   m_endStateTime = END_STATE_TIME;
}

/// <summary>
///  �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerGoal::Update(const float& elapsedTime)
{
    //���Ԃ̍X�V
    m_openTime -= elapsedTime;
	m_endStateTime -= elapsedTime;
	//�ړ�����
   if (m_openTime <= 0)
   {
	  Move(GOAL_MOVE_POSITION, elapsedTime);
   }

   if (m_endStateTime <= 0)
   {
	  m_player->ChangeState(m_player->GetPlayerIdle());
	  m_player->SetEndGaolEvent(true);
   }
   //�ړ��x�N�g��������������
   m_playerVelocity = DirectX::SimpleMath::Vector3::Zero;
}


/// <summary>
/// �`�悷��
/// </summary>
void PlayerGoal::Render()
{
}

/// <summary>
/// ��n������
/// </summary>
void PlayerGoal::Finalize()
{
}

/// <summary>
/// �ړ��������烉�W�A���p�x�ɕϊ�
/// </summary>
/// <param name="vec">�ړ�����</param>
/// <returns></returns>
float PlayerGoal::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
    //�ړ��̋�����0�Ȃ�p�x��0�ŕԂ�
	if (vec.Length() <= 0) return 0.0f;

	//�ړ���������p�x�����߂�
	float angle = atan2(vec.x, vec.z);
	return angle;
}

/// <summary>
/// �󂯎�����n�_�Ɉړ�������
/// </summary>
/// <param name="target">�^�[�Q�b�g���W</param>
/// <param name="elasedTime">�o�ߎ���</param>
void PlayerGoal::Move(const DirectX::SimpleMath::Vector3& target, const float elasedTime)
{
   // �^�[�Q�b�g�܂ł̕����x�N�g�����v�Z����
   DirectX::SimpleMath::Vector3 direction = target - m_player->GetPosition();
   // ���̋����ɋ߂Â��Ǝ~�܂�
   direction.Normalize();
   //���x���v�Z����
   DirectX::SimpleMath::Vector3 Velocity = direction * Player::MOVE_SPEED * elasedTime;
   // ���W���X�V����
   m_player->SetPosition(m_player->GetPosition() + Velocity);

   // �i�s�����Ɍ�����
   if (direction.LengthSquared() > 0.0f)  // �����x�N�g�����[���łȂ��ꍇ
   {
	  // �V�����������v�Z����
	  m_player->SetAngle(atan2(direction.x, direction.z));
	  m_player->SetMoveAngle(0.0f);

   }
}
