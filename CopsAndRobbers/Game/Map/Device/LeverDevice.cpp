#include "pch.h"
#include "Game/Map/Device/LeverDevice.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Astar/AStarGraph.h"

/// <summary>
///  �R���X�g���N�^
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="leverID">���o�[��ID</param>
LeverDevice::LeverDevice(const GameParameters::ObjectParameter& param, const int leverID)
    : 
    DeviceObjectBase(param.initialPos, param.initialRot, param.scale),
    m_initialPosition(param.initialPos),
    m_currentPosition(param.initialPos),
    m_leverHandle(HANDLE_ON_ROTATION),
    m_eventPosition(param.initialPos),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_leverID(leverID),
    m_isAnimating(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LeverDevice::~LeverDevice()
{
   Finalize();
}

/// <summary>
///  ����������
/// </summary>
void LeverDevice::Initialize()
{
    //���f�������擾����
    m_leverStandModel = yamadalib::Resources::GetInstance()->GetModel("Lever01Stand");
    m_leverHandleModel = yamadalib::Resources::GetInstance()->GetModel("Lever01Handle");
	m_standTexture = yamadalib::Resources::GetInstance()->GetTexture("Lever01Stand_DDS");
	m_handleTexture = yamadalib::Resources::GetInstance()->GetTexture("Lever01Handle_DDS");

    //�Փ˔���̓o�^
    InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void LeverDevice::Update(const float elapsedTime)
{
   //���u���N�����̂ݏ�������
   if (IsTriggered())
   {
	  Trigger(IsEnabled());
	  AnimateHandle(elapsedTime, IsEnabled());
	  //�M�~�b�N���N�����I�������̊m�F
	  AreAllGimmicksFinished();
	  //�N�����̓O���t���f���Ă��Ȃ����ɕύX
	  GameManager::GetInstance()->GetGraph()->InitializeDynamicGraph();
	  //�S�ẴM�~�b�N���N�����I���Ă���A�j���[�V���������Z�b�g����
	  if (!IsTriggered())
	  {
		 //�O���t�ɃM�~�b�N�̕ω��𔽉f����
		 GameManager::GetInstance()->GetGraph()->UpdateDynamicObjects();
		 m_isAnimating = false;
	  }
   }
}

/// <summary>
/// �`�悷��
/// </summary>
void LeverDevice::Render()
{
   // ���[���h�s����X�V����
   DirectX::SimpleMath::Matrix leverMatrix =
	  DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

   // �n���h���̃��[�J���ϊ��i��]�{�I�t�Z�b�g�j
   DirectX::SimpleMath::Matrix handleLocalMatrix =
	  DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_leverHandle) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(HANDOLE_LOCAL_OFFSET);
   DirectX::SimpleMath::Matrix handleWorldMatrix = handleLocalMatrix * leverMatrix;

   //���f����`�悷��
   m_graphics->GetCustomModelRenderer()->Draw(*m_leverStandModel, m_standTexture.Get(), leverMatrix);
   m_graphics->GetCustomModelRenderer()->Draw(*m_leverHandleModel, m_handleTexture.Get(), handleWorldMatrix);
}

/// <summary>
/// �㏈��������
/// </summary>
void LeverDevice::Finalize()
{
   // do nothing.
}


/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void LeverDevice::InitializeCollision()
{
   //��]�ɉ����ċ��E�{�b�N�X�̃T�C�X�𒲐�
   const DirectX::SimpleMath::Vector3 eventAdjustedExtents = yamadalib::Math::AdjustExtentsForGridRotation(EVENT_COLISION_EXTENTS, GetRotation());
   DirectX::SimpleMath::Vector3 rotatedOffset = DirectX::SimpleMath::Vector3::Transform(EVENT_COLISION_OFFSET, GetRotation());
   m_eventPosition += rotatedOffset;
   //�C�x���g�Փ˔���𐶐�
   CollisionManager::Collision eventCollision =
	  CollisionManager::GetInstance().CreateBoxCollision(
		 CollisionManager::CollisionPurpose::EVENT_TRIGGER,                 //�Փ˔���̖ړI 
		 CollisionManager::CollisionType::HIT,                              //�Փˏ���
		 DirectX::BoundingBox(m_eventPosition, eventAdjustedExtents));      //�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter eventParam = {
	  this, 
	  CollisionManager::ObjectType::LEVER, 
	  eventCollision,
	  [this](const CollisionObject* other) { HandleCollision(other); }, 
	  [this](const CollisionObject* other) { HandleCollisionExit(other); } 
   };

   //�����蔻��̓o�^
   m_eventObject = CollisionManager::GetInstance().RegisterObject(eventParam, m_objectStageNumber);
}

/// <summary>
/// �Փˎ��̃C�x���g����
/// </summary>
/// <param name="other">�ՓˑΏۂ̃R���W�����I�u�W�F�N�g</param>
void LeverDevice::HandleCollision(const CollisionObject* other)
{
   UNREFERENCED_PARAMETER(other);
   //���u�𑀍�\�ɂ���
   SetInsideControlZone(true);
}

/// <summary>
/// �Փ˔��藣�E���̏���
/// </summary>
/// <param name="other"></param>
void LeverDevice::HandleCollisionExit(const CollisionObject* other)
{
   UNREFERENCED_PARAMETER(other);
   //���u�𑀍�s�\�ɂ���
   SetInsideControlZone(false);
}



/// <summary>
/// ���o�[�n���h���̃A�j���[�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="leverFlag">���o�[��ON�EOFF�t���O</param>
void LeverDevice::AnimateHandle(const float elapsedTime, bool leverFlag)
{
   //�A�j���[�V���������������珈�������Ȃ�
   if (m_isAnimating) return;

   //�A�j���[�V�����̐i�s�x�̍X�V
   m_handleAnimetionProgress += HANDLE_ANIMETION_SPEED * elapsedTime;
   m_handleAnimetionProgress = yamadalib::Math::Clamp(m_handleAnimetionProgress, 0.0f, 1.0f);

   //�n���h���̃t���O���ƂɃn���h���𓮂���
   if (leverFlag) //���o�[ON
   {
	  m_leverHandle = DirectX::SimpleMath::Quaternion::Slerp(HANDLE_ON_ROTATION, HANDLE_OFF_ROTATION, m_handleAnimetionProgress);
   }
   else  //���o�[OFF
   {
	  m_leverHandle = DirectX::SimpleMath::Quaternion::Slerp(HANDLE_OFF_ROTATION, HANDLE_ON_ROTATION, m_handleAnimetionProgress);
   }

   //�A�j���[�V�����i�s�x��1.0f�ɂȂ�����A�j���[�V�������I������
   if (m_handleAnimetionProgress >= 1.0f)
   {
	  m_handleAnimetionProgress = 0.0f;
	  m_isAnimating = true;
   }
}