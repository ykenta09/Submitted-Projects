/*
	@file	MovingFloor.cpp
	@brief	���N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Floor/MovingFloor.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="type">���f���̃^�C�v</param>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="movePos">�����ړ������̍��W</param>
/// <param name="gimmickID">�o�^����M�~�b�N��ID</param>
MovingFloor::MovingFloor(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID)
	:
    GimmickObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_startPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_movePosition(movePos),
    m_gimickID(gimmickID),
    m_moveProgress(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MovingFloor::~MovingFloor()
{
}


/// <summary>
/// ����������
/// </summary>
void MovingFloor::Initialize()
{
   // ���f����ǂݍ���
   switch (m_type)
   {
   case 1:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("MovingFloor01"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("MoveFloor01_DDS");
	  break;
   }

   //�����蔻��̓o�^
   InitializeCollision();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void MovingFloor::Update(const float elapsedTime)
{
   //���Ԃ̍X�V
   m_time = elapsedTime;

   //�Փ˔���̏��X�V
   if (m_collisionObject)
   {
	  m_collisionObject->UpdatePosition(GetPosition());
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void MovingFloor::Render()
{
   // ���[���h�s����X�V����
   DirectX::SimpleMath::Matrix matrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	// ���f����`�悷��
    m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), matrix);
}

/// <summary>
/// ��n������
/// </summary>
void MovingFloor::Finalize()
{
	// do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void MovingFloor::InitializeCollision()
{
   //��]��K�������Փ˔���̃T�C�Y�̒���
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, GetRotation());

   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::Sensor,
	  CollisionManager::CollisionType::HIT,
	  DirectX::BoundingBox(GetPosition(), adjustedExtents));
   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FLOOR, collision };

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}


/// <summary>
/// �M�~�b�N�A�N�e�B�u
/// </summary>
void MovingFloor::Active()
{
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);
   SetPosition(yamadalib::Math::Lerp(m_startPosition, m_movePosition, m_moveProgress));
   if (m_moveProgress >= 1.0f)
   {
	  m_isFinished = true;
	  m_moveProgress = 0.0f;
   }
}


/// <summary>
/// �M�~�b�N��A�N�e�B�u
/// </summary>
void MovingFloor::Deactive()
{
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);
   SetPosition(yamadalib::Math::Lerp(m_movePosition, m_startPosition, m_moveProgress));
   if (m_moveProgress >= 1.0f)
   {
	  m_isFinished = true;
	  m_moveProgress = 0.0f;
   }
}

