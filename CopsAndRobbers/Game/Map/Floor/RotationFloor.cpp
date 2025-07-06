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
#include "Game/Map/Floor/RotationFloor.h"
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
RotationFloor::RotationFloor(const int& type, const GameParameters::ObjectParameter& param, const int& gimmickID)
	:
    GimmickObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_startPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_floorRotation(param.initialRot),
    m_gimickID(gimmickID),
    m_moveProgress(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
RotationFloor::~RotationFloor()
{
}

/// <summary>
/// ����������
/// </summary>
void RotationFloor::Initialize()
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
void RotationFloor::Update(const float elapsedTime)
{
   //���Ԃ̍X�V
   m_time = elapsedTime;

   //�Փ˔���̏��X�V
   if (m_collisionObject)
   {
	  //�M�~�b�N���I�������^�C�~���O�ōX�V
	  if (!m_isFinished) return;
	  m_baseExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, m_floorRotation);
	  m_collisionObject->UpdatePosition(GetPosition(), m_baseExtents);
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void RotationFloor::Render()
{
   // ���[���h�s����X�V����
   DirectX::SimpleMath::Matrix matrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_floorRotation) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   // ���f����`�悷��
   m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), matrix);
}

/// <summary>
/// ��n������
/// </summary>
void RotationFloor::Finalize()
{
	// do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void RotationFloor::InitializeCollision()
{
   //�Փ˔���̃T�C�Y
   //const DirectX::SimpleMath::Vector3 baseExtents = GetScale() / 2.0f;
   m_baseExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, m_floorRotation);

   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::Sensor,
	  CollisionManager::CollisionType::HIT,
	  DirectX::BoundingBox(GetPosition(), m_baseExtents));
   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FLOOR, collision };

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}


/// <summary>
/// �M�~�b�N�A�N�e�B�u
/// </summary>
void RotationFloor::Active()
{
   //�A�j���[�V�����̐i�s�x�̍X�V
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);

   //��]�J�n�n�_�ƏI���n�_��ݒ�
   DirectX::SimpleMath::Quaternion start = GetRotation();
   DirectX::SimpleMath::Quaternion delta = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, ROTATION_Y);
   DirectX::SimpleMath::Quaternion end = start * delta;
   //��]�̊Ԃ��Ԃ���
   m_floorRotation = DirectX::SimpleMath::Quaternion::Slerp(start, end, m_moveProgress);

   //�i�s�x���B������I��
   if (m_moveProgress >= 1.0f)
   {
	  m_moveProgress = 0.0f;
	  m_isFinished = true;
   }
}


/// <summary>
/// �M�~�b�N��A�N�e�B�u
/// </summary>
void RotationFloor::Deactive()
{
   //�A�j���[�V�����̐i�s�x�̍X�V
   m_moveProgress += MOVE_SPEED * m_time;
   m_moveProgress = yamadalib::Math::Clamp(m_moveProgress, 0.0f, 1.0f);

   //��]�J�n�n�_�ƏI���n�_��ݒ�
   auto start = GetRotation();
   auto delta = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, ROTATION_Y);
   auto end = start * delta;
   //��]�̊Ԃ��Ԃ���
   m_floorRotation = DirectX::SimpleMath::Quaternion::Slerp(end, start, m_moveProgress);

   //�i�s�x���B������I��
   if (m_moveProgress >= 1.0f)
   {
	  m_moveProgress = 0.0f;
	  m_isFinished = true;
   }
}

