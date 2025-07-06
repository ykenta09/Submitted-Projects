/*
	@file	MovingWall.cpp
	@brief	�����ǃN���X
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Wall/MovingWall.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Render/CustomModelRenderer.h"
#include "Game/Collision/CollisionObject.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="type">���f���̃^�C�v</param>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="movePos">�ړ���ɍ��W</param>
/// <param name="gimmickID">�M�~�b�NID</param>
MovingWall::MovingWall(const int& type, const GameParameters::ObjectParameter& param, DirectX::SimpleMath::Vector3 movePos, const int& gimmickID)
	:
    GimmickObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_startPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_movePosition(movePos),
    m_gimickID(gimmickID),
    m_moveProgress(0.0f),
    m_collisionObject(nullptr)
{
}

/// <summary>
///�f�X�g���N�^
/// </summary>
MovingWall::~MovingWall()
{
   Finalize();
}

/// <summary>
/// ����������
/// </summary>
void MovingWall::Initialize()
{
	// ���f����ǂݍ���
   switch (m_type)
   {
   case 1:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_01"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall01_DDS");
	  break;
   case 2:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_02"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall02_DDS");
	  break;
   case 3:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_03"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall03_DDS");
	  break;
   case 4:
	  SetModel(yamadalib::Resources::GetInstance()->GetModel("Wall_04"));
	  m_dds = yamadalib::Resources::GetInstance()->GetTexture("Wall04_DDS");
	  break;
   }
	//�Փ˔����o�^����
	InitializeCollision();
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void MovingWall::Update(const float elapsedTime)
{
   m_time = elapsedTime;
   if (m_collisionObject)
   {
	  m_collisionObject->UpdatePosition(GetPosition());
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void MovingWall::Render()
{
   // ���[���h�s����X�V����
   DirectX::SimpleMath::Matrix matrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   //���f����`�悷��
   m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), matrix);

}

/// <summary>
/// ��n������
/// </summary>
void MovingWall::Finalize()
{
	// do nothing.
}

/// <summary>
/// �����蔻��̏�����
/// </summary>
void MovingWall::InitializeCollision()
{
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(GetScale() / 2.0f, GetRotation());

   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //�����蔻��̖ړI
	  CollisionManager::CollisionType::PUSH,						 //����̃^�C�v
	  DirectX::BoundingBox(GetPosition(), adjustedExtents));			 //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::WALL, collision };
   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}

/// <summary>
/// �M�~�b�N�A�N�e�B�u
/// </summary>
void MovingWall::Active()
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
void MovingWall::Deactive()
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
