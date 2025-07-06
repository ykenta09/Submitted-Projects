/*
	@file	Prison.cpp
	@brief	�S���N���X
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Prison/Prison.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/Character/Player/Player.h"
#include "Game/Render/CustomModelRenderer.h"
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="type">�I�u�W�F�N�g�̃^�C�v</param>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
Prison::Prison(const int& type, const GameParameters::ObjectParameter& param)
   :
   ObjectBase(param.initialPos, param.initialRot, param.scale),
   m_doarPosition(param.initialPos),
   m_doarMoved(0.0f),
   m_isOpen(false),
   m_isOepnEnd(false),
   m_objectStageNumber(param.stageParam.objectNumber),
   m_collisionObject(nullptr)
{
   UNREFERENCED_PARAMETER(type);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Prison::~Prison()
{
}


/// <summary>
/// ����������
/// </summary>
void Prison::Initialize()
{
	// ���f����ǂݍ���
	m_prisonModel[PrisonParts::ROOM] = yamadalib::Resources::GetInstance()->GetModel("Prison_Room");
	m_prisonModel[PrisonParts::DOAR] = yamadalib::Resources::GetInstance()->GetModel("Prison_Door");
	m_roomdds = yamadalib::Resources::GetInstance()->GetTexture("PrisonRoom_DDS");
	m_doordds = yamadalib::Resources::GetInstance()->GetTexture("PrisonDoor_DDS");

	//���E�{�b�N�X�𐶐�����
	InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Prison::Update(const float elapsedTime)
{
	if (m_isOpen)
	{
		MoveDoar(elapsedTime);
	}
}

/// <summary>
/// �`�悷��
/// </summary>
void Prison::Render()
{
	// ���[���h�s����X�V����
   DirectX::SimpleMath::Matrix prisonRoomMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   DirectX::SimpleMath::Matrix prisonDoarMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(m_doarPosition);

   // ���f����`�悷��
   m_graphics->GetCustomModelRenderer()->Draw(*m_prisonModel[PrisonParts::ROOM], m_roomdds.Get(), prisonRoomMatrix);
   m_graphics->GetCustomModelRenderer()->Draw(*m_prisonModel[PrisonParts::DOAR], m_doordds.Get(), prisonDoarMatrix);
}

/// <summary>
/// ��n������
/// </summary>
void Prison::Finalize()
{
	// do nothing.
}


/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Prison::InitializeCollision()
{
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(PRISON_BASE_EXTENTS, GetRotation());
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_BASE_EXTENTS, GetRotation());

   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,										  //�����蔻��ړI
	  CollisionManager::CollisionType::PUSH,											  //����̃^�C�v
	  DirectX::BoundingBox(GetPosition(), adjustedExtents));							  //���E�{�b�N�X
   CollisionManager::Collision eventCollision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::EVENT_TRIGGER,								  //�����蔻��̖ړI
	  CollisionManager::CollisionType::HIT,												  //����̃^�C�v
	  DirectX::BoundingBox(GetPosition() + EVENT_BASE_EXTENTS, eventAdjustedExtents));	  //���E�{�b�N�X
	  
   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::PRISON, collision };
   //�C�x���g�p�̏Փ˔���p�����[�^
   CollisionManager::CollisionParameter eventParam = { this, CollisionManager::ObjectType::PRISON, eventCollision, [this](const CollisionObject* other) { HandleCollisionEvent(other); }};

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
   CollisionManager::GetInstance().RegisterObject(eventParam, m_objectStageNumber);
}



/// <summary>
/// �C�x���g�Փ˔���
/// </summary>
/// <param name="other">�ՓˑΏ�</param>
void Prison::HandleCollisionEvent(const CollisionObject* other)
{
   if (other->GetCollisionParameter().objectID != CollisionManager::ObjectType::PLAYER) return;
   
   Player* player = static_cast<Player*>(other->GetOwner());
   if (!m_isOpen && player->HasKey())
   {
	  player->UseKey();	//�����g�p���ĘS�����J����
	  m_isOpen = true;
   }
}

/// <summary>
/// ���𓮂���
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Prison::MoveDoar(const float elapsedTime)
{
   //���[�J���̈ړ��ʂ��v�Z
   const float localMoveAmount = OPEN_SPEED * elapsedTime;

   m_doarMoved += localMoveAmount;
   m_doarMoved = yamadalib::Math::Clamp(m_doarMoved, 0.0f, OPEN_LENGTH);

   //��]��K�p
   const DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
   //���[�J�����W�n�̈ړ��x�N�g�����쐬
   DirectX::SimpleMath::Vector3 localMovement(m_doarMoved, 0.0f, 0.0f);
   //���[���h���W�ɕϊ�
   const DirectX::SimpleMath::Vector3 worldOfset = DirectX::SimpleMath::Vector3::Transform(localMovement, rotationMatrix);
   //���W���X�V
   m_doarPosition = GetPosition() + worldOfset;

   //�I���t���O
   if (m_doarMoved >= OPEN_LENGTH)
   {
	  m_isOepnEnd = true;
   }
}

/// <summary>
/// �S�����J����
/// </summary>
void Prison::OpenPrison()
{
	m_isOpen = true;
}
