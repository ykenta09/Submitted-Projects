/*
	@file	PlayerSprint.cpp
	@brief	�v���C���[�̍����ړ���ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Player/State/PlayerSprint.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Astar/AStarGraph.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="player">�v���C���[�{�̂̃|�C���^</param>
PlayerSprint::PlayerSprint(Player* player)
	:
	m_player( player ),
    m_sprintStartTime(0.0f),
	m_moveAngle( 0.0f ),
    m_sprintVelocity(DirectX::SimpleMath::Vector3::Zero),
	m_isSprint(false)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerSprint::~PlayerSprint()
{
   //��n��������
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void PlayerSprint::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerSprint::Update(const float& elapsedTime)
{	
    //�����ړ������ǂ���
	if (m_isSprint)
	{
	    //�����ړ��̏���
		Sprint(elapsedTime);
	}
	else
	{
	    //�A�C�h����Ԃɖ߂�
		m_player->ChangeState(m_player->GetPlayerIdle());
	}
}

/// <summary>
/// �`�悷��
/// </summary>
void PlayerSprint::Render()
{
}

/// <summary>
/// ��n������
/// </summary>
void PlayerSprint::Finalize()
{
}

/// <summary>
/// �����ړ�
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayerSprint::Sprint(const float& elapsedTime)
{
    //�J�����̌��Ă�����������]�s����쐬
	m_playerMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(m_player->GetCameraAngle());

	// ����s���̎��Ԃ��X�V
	m_sprintStartTime += elapsedTime;

	if (m_sprintStartTime < ACCELERATION_DURATION)
	{
	   // �������F�����l���v�Z�i���`�����j
	   float acceleration = MAX_MOVEMENT_SPEED * (m_sprintStartTime / ACCELERATION_DURATION);
	   m_sprintVelocity = DirectX::SimpleMath::Vector3::Transform(m_sprintDirection * acceleration, m_playerMatrix);
	   m_sprintVelocity *= elapsedTime;
	   IsFloor();
	   m_player->SetPosition(m_player->GetPosition() + m_finalVelocity);
	}
	else
	{
	   // ��葬�x�ňړ�
	   m_sprintVelocity = DirectX::SimpleMath::Vector3::Transform(m_sprintDirection * MAX_MOVEMENT_SPEED, m_playerMatrix);
	   m_sprintVelocity *= elapsedTime;
	   IsFloor();
	   m_player->SetPosition(m_player->GetPosition() + m_finalVelocity);

	   // �]���肪�I��������t���O�����Z�b�g
	   if (m_sprintStartTime > MOVEMENT_DURATION)
	   {
		  m_isSprint = false;
		  m_sprintStartTime = 0.0f;
	   }
	}
}

/// <summary>
/// �ړ��������烉�W�A���p�x�ɕϊ�
/// </summary>
/// <param name="vec">�ړ�����</param>
/// <returns></returns>
float PlayerSprint::GetAngleFromVector(const DirectX::SimpleMath::Vector3& vec)
{
   //�ړ��̋�����0�Ȃ�p�x��0�ŕԂ�
   if (vec.Length() <= 0) return 0.0f;

   //�ړ���������p�x�����߂�
   float angle = atan2(vec.x, vec.z);
   return angle;
}

/// <summary>
/// �����ړ��̏���������
/// </summary>
/// <param name="dir">�����ړ��ňړ��������</param>
void PlayerSprint::PreSprint(const DirectX::SimpleMath::Vector3& dir)
{
	m_isSprint = true;
	m_sprintStartTime = 0.0f;
	m_sprintDirection = dir;
	m_sprintDirection.Normalize();
}


/// <summary>
/// ���̔���ƈړ��ʂ̒���
/// </summary>
void PlayerSprint::IsFloor()
{
   auto* graph = GameManager::GetInstance()->GetGraph();
   //�ړ���̍��W(x,z�����Ɓj
   DirectX::SimpleMath::Vector3 nextMoveX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_sprintVelocity.x, 0.0f, 0.0f);
   DirectX::SimpleMath::Vector3 nextMoveZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_sprintVelocity.z);
   // x�������̔���
   DirectX::SimpleMath::Vector3 nextX = m_player->GetPosition() + DirectX::SimpleMath::Vector3(m_sprintVelocity.x, 0.0f, 0.0f);
   bool floorX = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextX);

   // z�������̔���
   DirectX::SimpleMath::Vector3 nextZ = m_player->GetPosition() + DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_sprintVelocity.z);
   bool floorZ = graph->IsFloorCell(m_player->GetObjectStageNumber(), nextZ);

   // �ړ��x�N�g���𒲐�
   m_finalVelocity = DirectX::SimpleMath::Vector3::Zero;
   if (floorX) m_finalVelocity.x = m_sprintVelocity.x;
   if (floorZ) m_finalVelocity.z = m_sprintVelocity.z;
}
