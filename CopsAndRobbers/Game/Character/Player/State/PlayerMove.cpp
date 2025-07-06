/*
	@file	PlayerMove.cpp
	@brief	�v���C���[�̈ړ���ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerMove.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Astar/AStarGraph.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="player">�v���C���[�{�̂̃|�C���^</param>
PlayerMove::PlayerMove(Player* player)
	:
	m_player( player ),
	m_playerMoveMatrix( DirectX::SimpleMath::Matrix::Identity )
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerMove::~PlayerMove()
{
    //��n��������
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void PlayerMove::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerMove::Update(const float& elapsedTime)
{
	//�ړ�����
	Move(elapsedTime);

	//�~�܂��Ă���ꍇ�A��Ԃ�ύX
	if (m_velocity.Length() <= 0.0f)
	{
		m_player->ChangeState(m_player->GetPlayerIdle());
	}
	//�ړ��x�N�g��������������
	m_velocity = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// �`�悷��
/// </summary>
void PlayerMove::Render()
{
}

/// <summary>
/// ��n������
/// </summary>
void PlayerMove::Finalize()
{
}

/// <summary>
/// �ړ��������烉�W�A���p�x�ɕϊ�
/// </summary>
/// <param name="vec">�ړ�����</param>
/// <returns></returns>
float PlayerMove::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
    //�ړ��̋�����0�Ȃ�p�x��0�ŕԂ�
	if (vec.Length() <= 0) return 0.0f;

	//�ړ���������p�x�����߂�
	float angle = atan2(vec.x, vec.z);
	return angle;
}


/// <summary>
/// �ړ�����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerMove::Move(const float& elapsedTime)
{
   // �ړ����Ȃ琳�K�������č��W�ɉ��Z
   if (m_velocity.LengthSquared() > 0.0f)
   {
	  //�ړ��ʂ𐳋K���ƃX�P�[�����O
	  m_velocity.Normalize();
	  m_velocity *= Player::MOVE_SPEED * elapsedTime;

	  // �J�����̌����ɍ��킹�Ĉړ��x�N�g������]
	  m_player->SetMoveAngle(GetAngleFromVector(m_velocity));
	  m_playerMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_player->GetCameraAngle()); 
	  m_velocity = DirectX::SimpleMath::Vector3::Transform(m_velocity, m_playerMatrix);

	  //���̔���ƈړ��ʂ̒���
	  IsFloor();

	  m_player->SetPosition(m_player->GetPosition() + m_finalVelocity);
   }
}

/// <summary>
/// ���̔���ƈړ��ʂ̒���
/// </summary>
void PlayerMove::IsFloor()
{
   auto* graph = GameManager::GetInstance()->GetGraph();
   //�ړ���̍��W(x,z�����Ɓj
   DirectX::SimpleMath::Vector3 nextMoveX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_velocity.x, 0.0f, 0.0f);
   DirectX::SimpleMath::Vector3 nextMoveZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_velocity.z);
   // x�������̔���
   DirectX::SimpleMath::Vector3 nextX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_velocity.x, 0.0f, 0.0f);
   bool floorX = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextX);

   // z�������̔���
   DirectX::SimpleMath::Vector3 nextZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_velocity.z);
   bool floorZ = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextZ);

   // �ړ��x�N�g���𒲐�
   m_finalVelocity = DirectX::SimpleMath::Vector3::Zero;
   if (floorX) m_finalVelocity.x = m_velocity.x;
   if (floorZ) m_finalVelocity.z = m_velocity.z;
}
