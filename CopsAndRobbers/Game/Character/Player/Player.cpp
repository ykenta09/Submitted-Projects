/*
	@file	Player.cpp
	@brief	�v���C���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Player/Player.h"
#include "Game/Factory/CharacterFactory.h"
#include "Game/Character/Player/MouseControl.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Map/Key/Key.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="data">�L�����N�^�[�f�[�^</param>
Player::Player(IComponent* parent, CommonResources* resources, const GameParameters::CharacterData data, const int& stageNumber)
	:
	CharacterBase(parent, resources, data.position, data.rotation, data.scale),
    m_commonResources(resources),
	m_controlMouse{},
    m_nextMoveAngle(0.0f),
    m_previousVector(DirectX::SimpleMath::Vector3::Forward),
    m_sprintCoolTime(SPRINT_COOLTIME),
    m_isHit{ false },
    m_isGoal{false},
	m_hasKey(false),
    m_isGoalEvent(false),
    m_isStartEvent(true),
    m_player(this)
{
    m_objectStageNumber = stageNumber;
    //�L�����N�^�[�̃^�C�v��ݒ肷��
    SetCharacterType(CharacterType::Player);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void Player::Initialize()
{
    //���f������K�p����
    ApplyModelAndTexture();
	
	//�v���C���[�̃p�[�c�𐶐�
	CharacterBase::Initialize();
	//�}�E�X�R���g���[���𐶐�
	m_controlMouse = std::make_unique<MouseControl>(m_commonResources);
	//�X�^�~�i�𐶐�����
	m_stamina = std::make_unique<Stamina>();
	m_stamina->Initialize();

	//�v���C���[�̃A�C�h����Ԃ𐶐��A������
	m_playerIdle = std::make_unique<PlayerIdle>(this);
	m_playerIdle->Initialize();
	//�v���C���[�̈ړ���Ԃ𐶐��A������
	m_playerMove = std::make_unique<PlayerMove>(this);
	m_playerMove->Initialize();
	//�v���C���[�̉����Ԃ𐶐��A������
	m_playerSprint = std::make_unique<PlayerSprint>(this);
	m_playerSprint->Initialize();
	//�v���C���[�̃S�[�����
	m_playerGoal = std::make_unique<PlayerGoal>(this);
	m_playerGoal->Initialize();
	//�v���C���[�̃^�C�g�����
	m_playerTitle = std::make_unique<PlayerTitle>(this);
	m_playerTitle->Initialize();

	m_currentPosition = m_initialPosition;

	//������Ԃ�ݒ肷��
	m_currentState = m_playerIdle.get();
	//�Փ˔���̏�����
	InitializeCollision();
	//�Q�[���f�[�^���擾���Đݒ肷��
	SetGameData();
}

/// <summary>
///  �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�^�[�Q�b�g���W�̍X�V</param>
void Player::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
    UNREFERENCED_PARAMETER(angle);
	m_time += elapsedTime;
	//�N�[���^�C�������炷
	m_sprintCoolTime -= elapsedTime;
	m_playerVelocity = DirectX::SimpleMath::Vector3::Vector3::Zero;

	//�}�E�X�̍X�V�ƃ}�E�X�̑����ړ��ʂ��X�V
	if (!m_isStartEvent)
	{
	   m_controlMouse->Update(elapsedTime);
	}

	//���_�̉�]�ʂ��擾����
	m_cameraAngle = m_controlMouse->GetYaw();

	//���݂̏�Ԃ��X�V����
	m_currentState->Update(elapsedTime);

	//���݂̏����X�V
	m_currentPosition += position;

	//�����蔻��̍X�V
	if (m_collisionObject)
	{
	   m_collisionObject->UpdatePosition(m_currentPosition + COLLISION_OFFSET);
	}

	//�X�^�~�i���X�V
	m_stamina->Update(elapsedTime);

	//�S�[����ԂɕύX
	if (m_isGoal)
	{
	   ChangeState(m_playerGoal.get());
	}
	// �v���C���[�p�[�c���X�V����
	CharacterBase::Update(elapsedTime, m_currentPosition, m_cameraAngle + m_nextMoveAngle, target);
}

/// <summary>
/// �`�悷��
/// </summary>
inline void Player::Render()
{
	//���݂̏�Ԃ�`�悷��
	m_currentState->Render();

	//���ʏ����̕`��
	CharacterBase::Render();

#ifdef _DEBUG

	// �f�o�b�O�����uDebugString�v�ŕ\������
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("PlayerCurrentPosition, %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	debugString->AddString("PlayerCurrentAngle, %f", DirectX::XMConvertToDegrees(GetAngle()));
	debugString->AddString("HasKey, %d", m_keyNum);
	debugString->AddString("PlayerCurrentStage, %d", m_objectStageNumber);

	if (m_currentState == GetPlayerIdle())
	{
		debugString->AddString("PlayerState : IDLE");
	}
	else if (m_currentState == GetPlayerMove())
	{
		debugString->AddString("PlayerState: MOVE");
	}
	else if(m_currentState == GetPlayerSprint())
	{
		debugString->AddString("PlayerState : Sprint");
	}
	else if (m_currentState == GetPlayerGoal())
	{
	   debugString->AddString("PlayerState : Goal");
	}

	//���E�{�b�N�X�̃f�o�b�N�`��
    //m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
    //[this](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
    //{
   	//	 m_graphics->DrawBoundingSphere(m_collisionObject->GetCollisionParameter().collision.sphere);
   	//	 m_graphics->DrawBoundingBox(m_collisionObject->GetCollisionParameter().collision.box);
    //});

#endif // _DEBUG
}

/// <summary>
///  ��n������
/// </summary>
void Player::Finalize()
{
   m_playerGoal.reset();
   m_playerIdle.reset();
   m_playerMove.reset();
   m_playerSprint.reset();
   m_playerTitle.reset();
   m_controlMouse.reset();
   m_stamina.reset();
   m_commonResources->GetBlackBoard()->SetHasKeyNum(m_keyNum);
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Player::InitializeCollision()
{
   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,						//�����蔻��ړI
	  CollisionManager::CollisionType::HIT,								//����̃^�C�v
	  DirectX::BoundingBox(m_currentPosition , COLLISION_EXTENTS));	   //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::PLAYER, collision, [this](const CollisionObject* other) { HandleCollision(other); } };

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param);
}

/// <summary>
/// �Q�[���f�[�^��ݒ肷��
/// </summary>
void Player::SetGameData()
{
   m_keyNum = m_commonResources->GetBlackBoard()->GetHasKeyNum();
}

/// <summary>
///  �Փˎ��̏���
/// </summary>
/// <param name="other">�ՓˑΏۂ̃R���W�����I�u�W�F�N�g</param>
void Player::HandleCollision(const CollisionObject* other)
{
   //�ՓˑΏۂ��G�̏ꍇ
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::ENEMY)
   {
	  m_isHit = true;
   }
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::KEY)
   {
	  const Key* key = static_cast<const Key*>(other->GetOwner());
	  if (key)
	  {
		 m_hasKey = true;  //������������
		 m_keyNum++;	   //�������Ă��錮�̐��𑝂₷
		 m_commonResources->GetBlackBoard()->SetHasKeyNum(m_keyNum);
	  }
   }
   //�R���W�����^�C�v�������o���ł͂Ȃ��ꍇ���������Ȃ�
   if (other->GetCollisionParameter().collision.collisionType != CollisionManager::CollisionType::PUSH) return;
   PushPosition(m_currentPosition + m_collisionObject->CalculatePushBack(other));
}

/// <summary>
/// �Փ˂����Ƃ��Ƀv���C���[�������o��
/// </summary>
/// <param name="position">�����o�����W</param>
void Player::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
    //���W���X�V����
	m_currentPosition = position;
	//�����蔻��̍X�V
	m_collisionObject->UpdatePosition(m_currentPosition + COLLISION_OFFSET);
}

/// <summary>
/// ���݂̏�Ԃ�ύX����
/// </summary>
/// <param name="newState">�V�������</param>
void Player::ChangeState(IState* newState)
{
    //�V������ԂɕύX
	m_currentState = newState;
	//������]������
	if(m_applyInitialRotation) LostInitialRotation();
}

/// <summary>
/// �������Ă��錮���g�p����
/// </summary>
void Player::UseKey()
{
   //�������Ă��錮�̐������炷
   m_keyNum -= 1;
   m_commonResources->GetBlackBoard()->SetHasKeyNum(m_keyNum);
   if (m_keyNum > 0) return;
   m_hasKey = false;	//�������Ă��錮��0�Ȃ�t���O��ύX
}

/// <summary>
/// ���f������K�p����
/// </summary>
void Player::ApplyModelAndTexture()
{
   CharacterBase::CharacterResources resource;
   resource.head = yamadalib::Resources::GetInstance()->GetModel("PlayerHead");
   resource.body = yamadalib::Resources::GetInstance()->GetModel("PlayerBody");
   resource.arm = yamadalib::Resources::GetInstance()->GetModel("PlayerArm");
   resource.foot = yamadalib::Resources::GetInstance()->GetModel("PlayerFoot");
   resource.headDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerHead_DDS");
   resource.bodyDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerBody_DDS");
   resource.armDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerArm_DDS");
   resource.footDDS = yamadalib::Resources::GetInstance()->GetTexture("PlayerFoot_DDS");
   //�x�[�X�N���X�Ƀ��f������K�p
   CharacterBase::SetPartModels(resource);
}

/// <summary>
/// �ώ@�҂ɒʒm����
/// </summary>
/// <param name="keys">�����Ă���L�[</param>
void Player::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	//�ړ��L�[�̏���
	switch (keys)
	{
	case DirectX::Keyboard::W:// �O�i�x�N�g�������Z
	    m_playerVelocity += DirectX::SimpleMath::Vector3::Forward;
		break;
	case DirectX::Keyboard::S:// ��ރx�N�g�������Z
		m_playerVelocity += DirectX::SimpleMath::Vector3::Backward;
		break;
	case DirectX::Keyboard::A:// ���ړ��x�N�g�������Z
		m_playerVelocity += DirectX::SimpleMath::Vector3::Left;
		break;
	case DirectX::Keyboard::D:// �E�ړ��x�N�g�������Z
		m_playerVelocity += DirectX::SimpleMath::Vector3::Right;
		break;
	case DirectX::Keyboard::LeftShift:// �_�b�V���E�������
		if (m_sprintCoolTime <= 0.0f && !m_stamina->IsAvoidance() || m_playerSprint->GetIsSprint()) break;//��𒆂Ȃ珈�����Ȃ�
		ChangeState(m_playerSprint.get());			  // �X�v�����g��ԂɑJ��
		m_playerSprint->PreSprint(m_previousVector);  // �X�v�����g�O�̈ړ��x�N�g�����L�^
		m_stamina->ConsumeStamina();				  //��𕪂̃X�^�~�i�������
		break;
	}

	// ���炩�̈ړ����͂��������ꍇ
	if (m_playerVelocity.Length() > 0.0f)
	{
	    // �O�t���[���̈ړ��x�N�g����ۑ�
		m_previousVector = m_playerVelocity;
		// �X�v�����g���łȂ���Βʏ�ړ���Ԃ�
		if (!m_playerSprint->GetIsSprint())
		{
		   m_player->SetMoveAngle(m_playerMove->GetAngleFromVector(m_playerVelocity));		//�ړ��x�N�g������p�x�����߂�
		   m_playerMove->SetVelocity(m_playerVelocity);
		   ChangeState(m_playerMove.get());
		}
	}
}