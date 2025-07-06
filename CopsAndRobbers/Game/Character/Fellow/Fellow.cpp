/*
	@file	Fellow.cpp
	@brief	���ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/MyLib/BlackBoard.h"


/// <summary>
///  �R���X�g���N�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="map">�}�b�v</param>
/// <param name="data">�L�����N�^�[�f�[�^</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
Fellow::Fellow(IComponent* parent, CommonResources* resources, Map* map, const GameParameters::CharacterData data, const int& stageNumber)
	:
	CharacterBase(parent, resources, data.position, data.rotation, data.scale),
    m_commonResources(resources),
    m_map(map),
    m_fellow(this)
{
   m_objectStageNumber = stageNumber;
   //�L�����N�^�[�^�C�v��ݒ�
   SetCharacterType(CharacterType::Fellow);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Fellow::~Fellow()
{
	Finalize();
}


/// <summary>
/// /����������
/// </summary>
void Fellow::Initialize()
{
   //���f������K�p����
   ApplyModelAndTexture();

   //�p�[�c�𐶐�
   CharacterBase::Initialize();

   //���ԑҋ@��Ԃ𐶐��A������
   m_fellowIdle = std::make_unique<FellowIdle>(this, m_map);
   m_fellowIdle->Initialize();
   //���ԒǐՏ�Ԃ𐶐��A������
   m_fellowChasing = std::make_unique<FellowChasing>(this);
   m_fellowChasing->Initialize();
   //���ԒT����Ԃ𐶐��A������
   m_fellowSearch = std::make_unique<FellowRouteSearch>(this, m_map, m_objectStageNumber);
   m_fellowSearch->Initialize();

   if (m_map)
   {   //AStar�̐����A������
	  m_multiStageAStar = std::make_unique<MultiStageAStar>(m_fellowSearch->GetGrid(), m_map->GetStageTransition());
   }
   
   //������Ԃ�ݒ�
   m_currentState = m_fellowIdle.get();
   //�����蔻��̐ݒ�
   InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�^�[�Q�b�g���W</param>
void Fellow::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
    //���Ԃ̍X�V
    m_time = elapsedTime;

	//���݂̏�Ԃ��X�V����
	m_currentState->Update(elapsedTime);

	//�����X�V����
	m_currentPosition += position;
	m_currentAngle += angle;
	m_targetPosition = target;

	//�����蔻��̍X�V
	m_collisionObject->UpdatePosition(m_currentPosition);

	//�S�[���t���O��true�Ȃ�ڕW�̈ʒu�܂ňړ�����
	if (m_fellow->GetFellowChasing()->GeteGoalFlag())
	{
	   m_fellow->GetFellowChasing()->TargetMove(target, elapsedTime);
	}

	//�ǐՂ���^�[�Q�b�g�̐ݒ�
	m_fellowSearch->SetTargetStageNumber(m_commonResources->GetBlackBoard()->GetStageNumber());

	//�p�[�c�̍X�V
	CharacterBase::Update(elapsedTime, m_currentPosition, m_currentAngle, m_targetPosition);
}


/// <summary>
/// �`�悷��
/// </summary>
void Fellow::Render()
{
	//���݂̏�Ԃ�`�悷��
	m_currentState->Render();

	CharacterBase::Render();
#ifdef _DEBUG
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("FellowCurrentPosition, %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	//debugString->AddString("FellowCurrentStage, %d", m_objectStageNumber);


	if (m_currentState == GetFellowChasing())
	{
	   debugString->AddString("FellowState : Chasing");
	}
	else if (m_currentState == GetFellowIdle())
	{
	   debugString->AddString("FellowState: Idle");
	}
	else if (m_currentState == GetFellowRouteSearch())
	{
	   debugString->AddString("FellowState : Search");
	}
#endif // _DEBUG
}

/// <summary>
/// ��n������
/// </summary>
void Fellow::Finalize()
{
	m_fellowIdle.reset();
	m_fellowChasing.reset();
	m_fellowSearch.reset();
	//m_astar.reset();
}

/// <summary>
/// �����蔻��̓o�^
/// </summary>
void Fellow::InitializeCollision()
{
   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //�����蔻��ړI
	  CollisionManager::CollisionType::PUSH,						 //����̃^�C�v
	  DirectX::BoundingBox(m_currentPosition, m_scale / 2.0f));		 //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FELLOW, collision, [this](const CollisionObject* other) { HandleCollision(other); }};

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param);
}

/// <summary>
/// �Փˎ��̏���
/// </summary>
/// <param name="other"></param>
void Fellow::HandleCollision(const CollisionObject* other)
{
   //�S���ƃS�[���̓����蔻����s��Ȃ�
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::PRISON) return;	
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::GOAL) return;
   
   if (other->GetCollisionParameter().collision.collisionType != CollisionManager::CollisionType::PUSH) return;
   //�����o������
   DirectX::SimpleMath::Vector3 pushBack = m_collisionObject->CalculatePushBack(other);
   PushPosition(m_currentPosition + pushBack);
}

/// <summary>
/// ��Ԃ�ύX����
/// </summary>
/// <param name="newState">�ύX�X�e�[�g</param>
void Fellow::ChangeState(IState* newState)
{
	m_currentState = newState;
	//������]���Ȃ���
	if(m_applyInitialRotation) LostInitialRotation();
}

/// <summary>
/// �����o������
/// </summary>
/// <param name="position">�����o����</param>
void Fellow::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
   m_currentPosition = position;
   //�����蔻��̍X�V
   m_collisionObject->UpdatePosition(m_currentPosition);
}

/// <summary>
/// ���f������K�p����
/// </summary>
void Fellow::ApplyModelAndTexture()
{
   CharacterBase::CharacterResources resource;
   resource.head = yamadalib::Resources::GetInstance()->GetModel("FellowHead");
   resource.body = yamadalib::Resources::GetInstance()->GetModel("FellowBody");
   resource.arm = yamadalib::Resources::GetInstance()->GetModel("FellowArm");
   resource.foot = yamadalib::Resources::GetInstance()->GetModel("FellowFoot");
   resource.headDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowHead_DDS");
   resource.bodyDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowBody_DDS");
   resource.armDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowArm_DDS");
   resource.footDDS = yamadalib::Resources::GetInstance()->GetTexture("FellowFoot_DDS");
   //�x�[�X�N���X�Ƀ��f������K�p
   CharacterBase::SetPartModels(resource);
}
