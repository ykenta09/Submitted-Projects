#include "pch.h"
#include "Game/Map/Door/Door.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/MyLib/BlackBoard.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Player/Player.h"

/// <summary>
///  �R���X�g���N�^
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="nextStage">�ړ���̃X�e�[�W�ԍ�</param>
/// <param name="backStage">�߂�X�e�[�W�ԍ�</param>
Door::Door(CommonResources* resources, const GameParameters::ObjectParameter& param, const int nextStage, const int backStage)
    : 
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_commonResources(resources),
    m_doarLeftInitialPosition(param.initialPos - DOOR_OFFSET),
    m_doarLeftPosition(param.initialPos),
    m_doarRightInitialPosition(param.initialPos + DOOR_OFFSET),
    m_doarRightPosition(param.initialPos),   
    m_nextStageNumber(nextStage),
    m_backStageNumber(backStage),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_isOpened(false),
    m_time(0.0f),
    m_closeTimer(CLOSE_TIME),
    m_isOpen(false),
    m_isChangeStage(false),
    m_collisionleftObject(nullptr)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
Door::~Door()
{
   Finalize();
}

/// <summary>
/// ������
/// </summary>
void Door::Initialize()
{
    //���f�������擾����
    SetModel(yamadalib::Resources::GetInstance()->GetModel("Door"));
    m_dds = yamadalib::Resources::GetInstance()->GetTexture("Door_DDS");

    //���W�𒲐�����
    UpdatePosition();

    //�Փ˔���̓o�^
    InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Door::Update(const float elapsedTime)
{
   //���Ԃ��X�V����
   m_time = elapsedTime;
   m_closeTimer -= elapsedTime;

   //���Ԍo�߂�������̊J���t���O��false�ɂ���
   if (m_closeTimer <= 0)
   {
      m_closeTimer = CLOSE_TIME;
      m_isOpen = false;
   }
   //�t���Ofalse�ɂȂ�����������
   if (!m_isOpen)
   {
      MoveDoar(false, m_time);
   }
   //�����蔻��̍X�V
   if (m_collisionleftObject)
   {
      m_collisionleftObject->UpdatePosition(m_doarLeftPosition);
      m_collisionrightObject->UpdatePosition(m_doarRightPosition);
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void Door::Render()
{      
    //�����̍s��
    DirectX::SimpleMath::Matrix goalDoarLeftMatrix =
       DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
       DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarLeftPosition);
    //�E���̍s��
    DirectX::SimpleMath::Matrix goalDoarRightMatrix = 
       DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
       DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
       DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180)) *
       DirectX::SimpleMath::Matrix::CreateTranslation(m_doarRightPosition);

   // ���f����`�悷��
    m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), goalDoarLeftMatrix);
    m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), goalDoarRightMatrix);


#ifdef _DEBUG
    //���E�{�b�N�X�̕`��
    m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
       [this](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
       {
          if (!m_collisionleftObject)return;

          m_graphics->DrawBoundingBox(m_collisionleftObject->GetCollisionParameter().collision.box);
          m_graphics->DrawBoundingBox(m_collisionrightObject->GetCollisionParameter().collision.box);
          m_graphics->DrawBoundingBox(m_frontEventObject->GetCollisionParameter().collision.box);
          m_graphics->DrawBoundingBox(m_backEventObject->GetCollisionParameter().collision.box);
       });

#endif // _DEBUG
}

/// <summary>
/// �㏈��������
/// </summary>
void Door::Finalize()
{
   // do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Door::InitializeCollision()
{
   //��]�ɉ����ăT�C�Y����
   const DirectX::SimpleMath::Vector3 adjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(COLISION_EXTENTS, GetRotation());
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_COLISION_EXTENTS, GetRotation());
   
   //�����蔻��̐ݒ�(�Փ˔���̖ړI�A�@�Փ˔���̃^�C�v�A�@��������Փ˔���j
   CollisionManager::Collision leftCollision = 
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::PHYSICAL, CollisionManager::CollisionType::PUSH, DirectX::BoundingBox(m_doarLeftPosition, adjustedExtents));
   CollisionManager::Collision rightCollision = 
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::PHYSICAL, CollisionManager::CollisionType::PUSH, DirectX::BoundingBox(m_doarRightPosition, adjustedExtents));
   CollisionManager::Collision eventCollisionA =
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::EVENT_TRIGGER, CollisionManager::CollisionType::HIT, DirectX::BoundingBox(m_frontEventPosition, eventAdjustedExtents));
   CollisionManager::Collision eventCollisionB =
      CollisionManager::GetInstance().CreateBoxCollision(CollisionManager::CollisionPurpose::EVENT_TRIGGER, CollisionManager::CollisionType::HIT, DirectX::BoundingBox(m_backEventPosition, eventAdjustedExtents));

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter paramLeft = { this, CollisionManager::ObjectType::DOOR, leftCollision };
   CollisionManager::CollisionParameter paramRight = { this, CollisionManager::ObjectType::DOOR, rightCollision };
   //�C�x���g�p�̏Փ˔���p�����[�^
   CollisionManager::CollisionParameter eventParamA = { this, CollisionManager::ObjectType::DOOR, eventCollisionA, [this](const CollisionObject* other) { HandleFrontCollisionEvent(other); }};
   CollisionManager::CollisionParameter eventParamB = { this, CollisionManager::ObjectType::DOOR, eventCollisionB, [this](const CollisionObject* other) { HandleBackCollisionEvent(other); } };

   //�����蔻��̓o�^
   m_collisionleftObject = CollisionManager::GetInstance().RegisterObject(paramLeft);
   m_collisionrightObject = CollisionManager::GetInstance().RegisterObject(paramRight);
   m_frontEventObject = CollisionManager::GetInstance().RegisterObject(eventParamA);
   m_backEventObject = CollisionManager::GetInstance().RegisterObject(eventParamB);
}

/// <summary>
/// �h�A�̐��ʃC�x���g
/// </summary>
/// <param name="other">�Փˑ���</param>
void Door::HandleFrontCollisionEvent(const CollisionObject* other)
{
   HandleDoorEvent(other, m_backStageNumber);
}

/// <summary>
/// �h�A�̌�둤�̃C�x���g
/// </summary>
/// <param name="other">�Փˑ���</param>
void Door::HandleBackCollisionEvent(const CollisionObject* other)
{
   HandleDoorEvent(other, m_nextStageNumber);
}

/// <summary>
/// �h�A�̃C�x���g����
/// </summary>
/// <param name="other">�Փˑ���</param>
/// <param name="targetStage">�ύX����X�e�[�W�ԍ�</param>
void Door::HandleDoorEvent(const CollisionObject* other, int targetStage)
{
   // �h�A���J���鏈��
   m_isOpen = true;
   MoveDoar(true, m_time);

   void* owner = other->GetOwner();
   if (!owner) return;

   auto objectID = other->GetCollisionParameter().objectID;
   if (objectID == CollisionManager::ObjectType::PLAYER)
   {
      static_cast<Player*>(owner)->SetObjectStageNumber(targetStage);
      LoadStage(targetStage);
   }
   if (objectID == CollisionManager::ObjectType::ENEMY)
   {
      static_cast<Enemy*>(owner)->SetObjectStageNumber(targetStage);
   }
   if (objectID == CollisionManager::ObjectType::FELLOW)
   {
      static_cast<Fellow*>(owner)->SetObjectStageNumber(targetStage);
   }
}

/// <summary>
/// �X�e�[�W�f�[�^�����[�h����
/// </summary>
/// <param name="stageNumber">���[�h����X�e�[�W�ԍ�</param>
void Door::LoadStage(int stageNumber)
{
   m_commonResources->GetBlackBoard()->SetStageNumber(stageNumber);
   CollisionManager::GetInstance().ChangeStageObjects(stageNumber);
}

/// <summary>
/// �h�A�̈ړ�����
/// </summary>
/// <param name="isOpening"></param>
/// <param name="elapsedTime"></param>
void Door::MoveDoar(bool isOpening, const float elapsedTime)
{
   //�h�A�̈ړ����x
    const float localMoveAmount = OPEN_GOAL_SPEED * elapsedTime;
    
    // �h�A�̊J�t���O�ɉ����Ĉړ��ʂ����Z�܂��͌��Z
    if (isOpening) 
       m_doarMoved += localMoveAmount;
    else 
       m_doarMoved -= localMoveAmount;

    // �N�����v�͈͂����[0.0f, OPEN_LENGTH]�Ɉێ�
    m_doarMoved = yamadalib::Math::Clamp(m_doarMoved, 0.0f, OPEN_LENGTH);
    
    // �h�A�̉�]�s����擾
    const auto& rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
    // ���[�J����Ԃł̈ړ��x�N�g�����쐬
    const DirectX::SimpleMath::Vector3 localMovement(m_doarMoved, 0.0f, 0.0f);
    // ���[�J���ړ��x�N�g�������[���h��Ԃɕϊ�
    const auto& worldOffset = DirectX::SimpleMath::Vector3::Transform(localMovement, rotationMatrix);
    // ���E�̃h�A�̌��ݍ��W���X�V
    m_doarLeftPosition = m_doarLeftInitialPosition - worldOffset;
    m_doarRightPosition = m_doarRightInitialPosition + worldOffset;

    // ��ԃt���O�X�V�i�K�v�ɉ����ĕ�����Ԃ��ǐՁj
    m_isOpened = (m_doarMoved >= OPEN_LENGTH);
}

/// <summary>
/// ���W�𒲐�����
/// </summary>
void Door::UpdatePosition()
{
   // ��]�N�H�[�^�j�I�������]�s����쐬
   DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation());
   // ��]�I�t�Z�b�g��K�p
   m_doarLeftInitialPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(-DOOR_OFFSET, rotMat);
   m_doarRightInitialPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(DOOR_OFFSET, rotMat);
   m_frontEventPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(EVENT_OFFSET, rotMat);
   m_backEventPosition = GetPosition() + DirectX::SimpleMath::Vector3::Transform(-EVENT_OFFSET, rotMat);
}
