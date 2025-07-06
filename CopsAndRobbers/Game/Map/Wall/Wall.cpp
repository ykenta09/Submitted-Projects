/*
	@file	Wall.cpp
	@brief	�ǃN���X
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Wall/Wall.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Render/CustomModelRenderer.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="type">�I�u�W�F�N�g�^�C�v</param>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
Wall::Wall(const int& type, const GameParameters::ObjectParameter& param)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_collisionObject(nullptr)
{
}

/// <summary>
///�f�X�g���N�^
/// </summary>
Wall::~Wall()
{
   Finalize();
}

/// <summary>
/// ����������
/// </summary>
void Wall::Initialize()
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
void Wall::Update(const float elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
}

/// <summary>
/// �`�悷��
/// </summary>
void Wall::Render()
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
void Wall::Finalize()
{
	// do nothing.
}

/// <summary>
/// �����蔻��̏�����
/// </summary>
void Wall::InitializeCollision()
{
   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //�����蔻��̖ړI
	  CollisionManager::CollisionType::PUSH,						 //����̃^�C�v
	  DirectX::BoundingBox(GetPosition(), GetScale() / 2));			 //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::WALL, collision };
   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}
