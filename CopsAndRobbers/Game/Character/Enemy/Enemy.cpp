/*
	@file	Enemy.cpp
	@brief	�G�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Enemy/EnemyVisibility.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"
#include "Game/Map/Map.h"
#include "Game/Collision/CollisionObject.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="map">�}�b�v</param>
/// <param name="data">�G�̃p�����[�^</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
Enemy::Enemy(
   IComponent* parent, 
   CommonResources* resources, 
   Map* map, 
   const GameParameters::EnemyData data, 
   const int& stageNumber)
   :
   CharacterBase(parent, resources, data.charaData.position, data.charaData.rotation, data.charaData.scale),
   m_map(map),
   m_patrolRootMap(data.patrolRouteMap),
   m_adjacencyList(data.patrolAdjacencyList),
   m_initialVertexNumber(data.initalNumber),
   m_enemyNumber(data.enemyNumber),
   m_enemyDirection(DirectX::SimpleMath::Vector3::Zero),
   m_enemy(this),
   m_nextAngle(0.0f),
   m_viewType(true)
{
   m_objectStageNumber = stageNumber;
   //�L�����N�^�[�^�C�v�̐ݒ�
   SetCharacterType(CharacterType::Enemy);
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Enemy::~Enemy()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void Enemy::Initialize()
{
    //���f������K�p����
    ApplyModelAndTexture();
	//�G�̃p�[�c�𐶐�
   	CharacterBase::Initialize();
	//�e�N�X�`�����擾����
	m_detectionTexture = yamadalib::Resources::GetInstance()->GetTexture("Detection");

	//�G�̑ҋ@��Ԃ𐶐��A������
	m_enemyWaiting = std::make_unique<EnemyWaiting>(this);
	m_enemyWaiting->Initialize();
	//�G�̜p�j��Ԃ𐶐��A������
	m_enemyWandering = std::make_unique<EnemyWandering>(this);
	m_enemyWandering->SetGraphDate(m_patrolRootMap, m_adjacencyList, m_initialVertexNumber);
	m_enemyWandering->Initialize();
	//�������W���擾����
	if (m_enemyWandering->GetPatrolMap().size() > 0)
	{
	   m_initialPosition = m_enemyWandering->GetInitialPosition();
	}
	//�G�̜p�j�ɖ߂��Ԃ𐶐��A������
	m_enemyWanderingReturn = std::make_unique <EnemyWanderingReturn>(this);
	m_enemyWanderingReturn->Initialize();
	//�G�̒ǐՏ�Ԃ𐶐��A������
	m_enemyChasing = std::make_unique<EnemyChasing>(this);
	m_enemyChasing->Initialize();
	//�G�̒T����Ԃ𐶐��A������
	m_enemyRouteSearch = std::make_unique<EnemyRouteSearch>(this, m_map, m_objectStageNumber);
	m_enemyRouteSearch->Initialize();
	//�G�̎��E�𐶐��A������
	m_visibility = std::make_unique<EnemyVisibility>(this);
	m_visibility->Initialize();
	//�G�̃^�C�g����Ԃ̐����A������
	m_enemyTitle = std::make_unique<EnemyTitle>(this);
	m_enemyTitle->Initialize();
	//AStar�̐����A������
	if (m_map)
	{   
	   m_multiStageAStar = std::make_unique<MultiStageAStar>(m_enemyRouteSearch->GetGrid(), m_map->GetStageTransition());
	}

	//������Ԃ�ݒ肷��
	m_currentState = m_enemyWaiting.get();
	m_currentPosition = m_initialPosition;
	//���E�{�b�N�X��ݒ肷��
	InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�^�[�Q�b�g���W</param>
void Enemy::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
    //���Ԃ̍X�V
	m_time = elapsedTime;
  	m_searchTime -= elapsedTime;

	//���݂̏�Ԃ��X�V����
	m_currentState->Update(elapsedTime);

	//�ǂ�������^�[�Q�b�g���W���X�V����
	m_currentPosition += position;
	m_currentAngle += angle;
	m_targetPosition = target;

	//�����蔻��̍X�V
	m_collisionObject->UpdatePosition(m_currentPosition);

	//�ǐՂ���^�[�Q�b�g�̐ݒ�
	m_enemyRouteSearch->SetTargetStageNumber(m_commonResources->GetBlackBoard()->GetStageNumber());

	//���E�̍X�V
	m_visibility->Update();
	// �G�p�[�c���X�V����
	CharacterBase::Update(elapsedTime, m_currentPosition, m_currentAngle, m_targetPosition);
	
	if (m_currentState == m_enemyTitle.get()) return;

	//�G�̎���ɓ��������Ԃ�ύX����
	if (m_currentState == m_enemyChasing.get())
	   m_viewType = false;
	else
	   m_viewType = true;

	if (InView(m_viewType))
	   Search();

}

/// <summary>
/// �`�悷��
/// </summary>
void Enemy::Render()
{
	//���݂̏�Ԃ�`�悷��
	m_currentState->Render();

	m_visibility->DrawVisibility(m_map);

	//�G�̃p�[�c��`�悷��
	CharacterBase::Render();

	//���E�ɓ�������`�悷��
	if (m_currentState == m_enemyTitle.get()) return;
    if (InView(true))
    {
	   //�G�̍s��𐶐�
	   DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_currentPosition);
	   //�v���C���[�𔭌���������!�}�[�N�`��
	   m_graphics->DrawTextureBillboard(m_detectionTexture.Get(), world);
    }

#ifdef _DEBUG

	// �f�o�b�O�����uDebugString�v�ŕ\������
	//auto debugString = m_commonResources->GetDebugString();
	//debugString->AddString("EnemyPosisiton, %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	//debugString->AddString("EnemyCurrentStage, %d", m_objectStageNumber);

	//if (m_currentState == GetEnemyChasing())
	//{
	//	debugString->AddString("EnemyState : Chasing");
	//}
	//else if (m_currentState == GetEnemyWaiting())
	//{
	//	debugString->AddString("EnemyState: Waiting");
	//}
	//else if (m_currentState == GetEnemyWandering())
	//{
	//	debugString->AddString("EnemyState : Wandering");
	//}
	//else if (m_currentState == GetEnemyWanderingReturn())
	//{
	//   debugString->AddString("EnemyState : Wandering Return");
	//}
	//else if (m_currentState == GetEnemyRouteSearch())
	//{
	//   debugString->AddString("EnemyState : RouteSearch");
	//}
#endif // _DEBUG
}

/// <summary>
/// ��n������
/// </summary>
void Enemy::Finalize()
{
	m_enemyWaiting.reset();
	m_enemyChasing.reset();
	m_enemyRouteSearch.reset();
	m_enemyWandering.reset();
	m_enemyWanderingReturn.reset();
}

/// <summary>
/// �����蔻��̏�����
/// </summary>
void Enemy::InitializeCollision()
{
   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,					 //�����蔻��ړI
	  CollisionManager::CollisionType::HIT,							 //����̃^�C�v
	  DirectX::BoundingBox(m_currentPosition, m_scale / 2.0f));		 //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this,	CollisionManager::ObjectType::ENEMY, collision,	[this](const CollisionObject* other) { HandleCollision(other); }};

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param);
}

/// <summary>
/// �Փˎ��̏���
/// </summary>
/// <param name="other">�ՓˑΏ�</param>
void Enemy::HandleCollision(const CollisionObject* other)
{
   //�R���W�����^�C�v�������o���ł͂Ȃ��ꍇ���������Ȃ�
   if (other->GetCollisionParameter().collision.collisionType != CollisionManager::CollisionType::PUSH) return;
   DirectX::SimpleMath::Vector3 pushBack = m_collisionObject->CalculatePushBack(other);
   PushPosition(m_currentPosition + pushBack);
}

/// <summary>
/// ����̒��ɓ����Ă��邩�H
/// </summary>
/// <param name="object">��Q���Ƃ̔�����s�����ǂ���</param>
/// <returns></returns>
bool Enemy::InView(bool object)
{
	// �^�[�Q�b�g�܂ł̕����x�N�g�����v�Z����
	m_targetDirection = m_targetPosition - m_currentPosition;

	//�^�[�Q�b�g������͈͊O�Ȃ珈�����Ȃ�
	if (m_targetDirection.Length() >= EnemyVisibility::VISIBILITIY_DISTANCE) return false;

	// �G�̌����x�N�g��
	m_enemyDirection = DirectX::SimpleMath::Vector3(sinf(m_currentAngle), 0.0f, cosf(m_currentAngle)); // m_currentAngle �͓G�̌����Ă���p�x
	m_targetDirection.Normalize();

	// ���ς��v�Z����
	float dotProduct = m_enemyDirection.Dot(m_targetDirection);

	// ����̊p�x�����W�A���ɕϊ�
	float viewAngle = cosf(DirectX::XMConvertToRadians(EnemyVisibility::FIELD_OF_VIEW / 2.0f));

	// ���ς�����̊p�x���ɂ��邩���`�F�b�N
	if (dotProduct > viewAngle) 
	{
	   //��Q���ɎՂ��Ă��邩�m�F
	   if (object)
	   {
		  return RayMapObject();
	   }
	   return true;
	}
	return false;
}

/// <summary>
/// ���C�ƃ}�b�v�I�u�W�F�N�g�̔���
/// </summary>
/// <returns></returns>
bool Enemy::RayMapObject()
{
   // �v���C���[�����ւ̃��C��ݒ�
   DirectX::SimpleMath::Vector3 directionToTarget = m_targetPosition - m_currentPosition;
   // �v���C���[�܂ł̋����Ɛ��K��
   float targetDistance = directionToTarget.Length(); 
   directionToTarget.Normalize();
   //�G����v���C���[�܂ł̋����Ƀ��C���쐬
   DirectX::SimpleMath::Ray viewRay(m_currentPosition, directionToTarget);

   if (m_map == nullptr) return true;
   //�}�b�v�I�u�W�F�N�g�ƏՓ˔���
   for (const auto& object : m_map->GetBoundingBoxes())
   {
	  float distance = 0;
	  if (object.Intersects(viewRay.position, viewRay.direction, distance))
	  {
		 if (distance < targetDistance && distance <= EnemyVisibility::VISIBILITIY_DISTANCE) 
		 return false;
	  }
   }
   return true;
}

/// <summary>
/// ��Ԃ�ύX����
/// </summary>
/// <param name="newState">�ύX����X�e�C�g</param>
void Enemy::ChangeState(IState* newState)
{
	m_currentState = newState;
	//������]������
	if(m_applyInitialRotation)	LostInitialRotation();
}

/// <summary>
/// ���G
/// </summary>
void Enemy::Search()
{
   //�ŏ��̍��G
   if (!m_enemyRouteSearch->GetFirstExploration())
   {
	  m_searchTime = TIME_TO_SEARCH;
	  ChangeState(m_enemyRouteSearch.get());
	  m_enemyRouteSearch->SetAstar(m_targetPosition);
   }
   else//��x�ڈȍ~�̍ĒT��
   {
	  if (m_enemyRouteSearch->GetGrid()->GetVertexFromPosition(m_objectStageNumber, m_targetPosition) == nullptr) return;
	  //�^�[�Q�b�g�̍��W���ς�����Ƃ��ĒT��
	  if (m_enemyRouteSearch->GetTargetCell()->GetVertexNumber() != m_enemyRouteSearch->GetGrid()->GetVertexFromPosition(m_objectStageNumber, m_targetPosition)->GetVertexNumber())
	  {
		 m_searchTime = TIME_TO_SEARCH;
		 ChangeState(m_enemyRouteSearch.get());
		 m_enemyRouteSearch->SetAstar(m_targetPosition);
	  }
   }
}

/// <summary>
/// ���f������K�p����
/// </summary>
void Enemy::ApplyModelAndTexture()
{
   CharacterBase::CharacterResources resource;
   resource.head = yamadalib::Resources::GetInstance()->GetModel("EnemyHead");
   resource.body = yamadalib::Resources::GetInstance()->GetModel("EnemyBody");
   resource.arm = yamadalib::Resources::GetInstance()->GetModel("EnemyArm");
   resource.foot = yamadalib::Resources::GetInstance()->GetModel("EnemyFoot");
   resource.headDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyHead_DDS");
   resource.bodyDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyBody_DDS");
   resource.armDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyArm_DDS");
   resource.footDDS = yamadalib::Resources::GetInstance()->GetTexture("EnemyFoot_DDS");
   //�x�[�X�N���X�Ƀ��f������K�p
   CharacterBase::SetPartModels(resource);
}

/// <summary>
/// �L�[�{�[�h����
/// </summary>
/// <param name="keys">�����ꂽ�L�[</param>
inline void Enemy::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// �Փ˂����Ƃ��Ƀv���C���[�������o��
/// </summary>
/// <param name="position">�����o�����W</param>
void Enemy::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
   m_currentPosition = position;
   //���E���̍X�V
   m_collisionObject->UpdatePosition(m_currentPosition);
}
