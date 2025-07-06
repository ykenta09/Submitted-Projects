/*
	@file	Key.cpp
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
#include "Game/Map/Key/Key.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
Key::Key(const GameParameters::ObjectParameter& param)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
	m_isExist(true),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_collisionObject(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Key::~Key()
{
}

/// <summary>
/// ����������
/// </summary>
void Key::Initialize()
{
	// ���f����ǂݍ���
	SetModel(yamadalib::Resources::GetInstance()->GetModel("Key"));
	m_dds = yamadalib::Resources::GetInstance()->GetTexture("Key_DDS");
	//���E�{�b�N�X�𐶐�����
	InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Key::Update(const float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	//��]��������
	SetRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, 0.01f));
}


/// <summary>
/// �`�悷��
/// </summary>
void Key::Render()
{
	//���݂��Ȃ��ꍇ�������Ȃ�
	if (!m_isExist) return;

	// ���[���h�s����X�V����
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
		DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	// ���f����`�悷��
	m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), world);

}

/// <summary>
/// ��n������
/// </summary>
void Key::Finalize()
{
	// do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Key::InitializeCollision()
{
   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,						   //�����蔻��ړI
	  CollisionManager::CollisionType::HIT,								   //����̃^�C�v
	  DirectX::BoundingBox(GetPosition(), GetScale() / 2));				   //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::KEY, collision, [this](const CollisionObject* other) { HandleCollision(other); }};

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}

/// <summary>
/// �Փˎ��̃C�x���g����
/// </summary>
/// <param name="other">�ՓˑΏۂ̃R���W�����I�u�W�F�N�g</param>
void Key::HandleCollision(const CollisionObject* other)
{
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::PLAYER)
   {
	  //���̏Փ˔��������
	  LostExist();
	  m_isExist = false;
   }
}


/// <summary>
/// ���̑��݂�����
/// </summary>
void Key::LostExist() const
{
	//�I�u�W�F�N�g�̓o�^����
   CollisionManager::GetInstance().UnregisterStageObject(m_objectStageNumber, m_collisionObject);
}
