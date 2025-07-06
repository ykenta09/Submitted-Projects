#include "pch.h"
#include "Game/Map/Goal/Goal.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/Math.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
Goal::Goal(const GameParameters::ObjectParameter& param)
    : 
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_doarLeftPosition(param.initialPos),
    m_doarRightPosition(param.initialPos - GOAL_POSITION_OFFSET),
    m_isOpened(false),
    m_time(0.0f),
    m_doarMoved(0.0f),
    m_goalPlayer(false),
    m_goalFellow(false),
    m_isGoal(false),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_collisionObject(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Goal::~Goal()
{
}
 

/// <summary>
/// ����������
/// </summary>
void Goal::Initialize()
{
    //���f�������擾����
    m_goalModel[GoalParts::FRAM] = yamadalib::Resources::GetInstance()->GetModel("GateFrame");
    m_goalModel[GoalParts::DOOR] = yamadalib::Resources::GetInstance()->GetModel("Gate");
    m_dds[GoalParts::FRAM] = yamadalib::Resources::GetInstance()->GetTexture("GateFrame_DDS");
    m_dds[GoalParts::DOOR] = yamadalib::Resources::GetInstance()->GetTexture("Gate_DDS");

    //�Փ˔���̓o�^
    InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Goal::Update(const float elapsedTime)
{
   m_time = elapsedTime;
   if (m_isOpened)
   {
      CollisionManager::GetInstance().UnregisterStageObject(m_objectStageNumber, m_collisionObject);
      m_collisionObject = nullptr;
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void Goal::Render()
{
    // ���[���h�s����X�V����
   DirectX::SimpleMath::Matrix goalMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
      DirectX::SimpleMath::Matrix::CreateScale(GetScale());
    DirectX::SimpleMath::Matrix goalFramMatrix = goalMatrix *
       DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());
    DirectX::SimpleMath::Matrix goalDoarLeftMatrix = goalMatrix *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarLeftPosition);
    DirectX::SimpleMath::Matrix goalDoarRightMatrix = goalMatrix *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarRightPosition);

    m_graphics->GetCustomModelRenderer()->Draw(*m_goalModel[GoalParts::FRAM], m_dds[GoalParts::FRAM].Get(), goalFramMatrix);
    m_graphics->GetCustomModelRenderer()->Draw(*m_goalModel[GoalParts::DOOR], m_dds[GoalParts::DOOR].Get(), goalDoarLeftMatrix);
    m_graphics->GetCustomModelRenderer()->Draw(*m_goalModel[GoalParts::DOOR], m_dds[GoalParts::DOOR].Get(), goalDoarRightMatrix);

#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// �㏈��������
/// </summary>
void Goal::Finalize()
{
   // do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Goal::InitializeCollision()
{
   //��]�ɉ������T�C�Y����
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(COLISION_EXTENTS, GetRotation());
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_COLISION_EXTENTS, GetRotation());

   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
      CollisionManager::CollisionPurpose::PHYSICAL,
      CollisionManager::CollisionType::PUSH,
      DirectX::BoundingBox(GetPosition(), adjustedExtents));
   CollisionManager::Collision eventCollision = CollisionManager::GetInstance().CreateBoxCollision(
      CollisionManager::CollisionPurpose::EVENT_TRIGGER,
      CollisionManager::CollisionType::HIT,
      DirectX::BoundingBox(GetPosition() + EVENT_COLISION_OFFSET, eventAdjustedExtents));
  
   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::GOAL, collision };
   //�C�x���g�p�̏Փ˔���p�����[�^
   CollisionManager::CollisionParameter eventParam = { this, CollisionManager::ObjectType::GOAL, eventCollision, [this](const CollisionObject* other) { HandleCollisionEvent(other); } };

   //�����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
   CollisionManager::GetInstance().RegisterObject(eventParam, m_objectStageNumber);
}

/// <summary>
/// �S�[������̓����蔻��ʒm
/// </summary>
/// <param name="other"></param>
void Goal::HandleCollisionEvent(const CollisionObject* other)
{
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::PLAYER)
   {
      m_goalPlayer = true;
   }
   if (other->GetCollisionParameter().objectID == CollisionManager::ObjectType::FELLOW)
   {
      m_goalFellow = true;
   }
   //�v���C���[�ƒ��Ԃ��S�[���ɒH�蒅���ƃS�[��
   if (m_goalPlayer && m_goalFellow)
   {
      m_isGoal = true;
      MoveDoar(m_time);
   }
}


/// <summary>
/// �S�[���̔����J��
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Goal::MoveDoar(const float elapsedTime)
{
   ////�����J���������珈�����s��Ȃ�
   //if (m_isOpened) return;

   //���[�J�����W�ňړ��ʂ��v�Z
   const float localMoveAmout = OPNE_GOAL_SPEED * elapsedTime;

   m_doarMoved += localMoveAmout;
   m_doarMoved = yamadalib::Math::Clamp(m_doarMoved, 0.0f, OPEN_LENGTH);
   
   //��]��K�p
   const DirectX::SimpleMath::Matrix rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
   //���[�J�����W�̈ړ��x�N�g�����쐬
   const DirectX::SimpleMath::Vector3 localMovement(m_doarMoved, 0.0f, 0.0f);
   //���[���h���W�ϊ�
   const DirectX::SimpleMath::Vector3 worldOfset = DirectX::SimpleMath::Vector3::Transform(localMovement, rotationMatrix);
   //���W�X�V
   m_doarLeftPosition = GetPosition() + worldOfset;
   m_doarRightPosition = GetPosition() - worldOfset - GOAL_POSITION_OFFSET;
   if (m_doarMoved >= OPEN_LENGTH)
   {
      m_isOpened = true;
   }
}
