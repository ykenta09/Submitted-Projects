/*
	@file	Floor.cpp
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
#include "Game/Map/Floor/Floor.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/Render/CustomModelRenderer.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="type">�I�u�W�F�N�g�̃^�C�v</param>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
Floor::Floor(const int& type, const GameParameters::ObjectParameter& param)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_type(type),
    m_objectStageNumber(param.stageParam.objectNumber)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Floor::~Floor()
{
}

/// <summary>
/// ����������
/// </summary>
void Floor::Initialize()
{
   // �^�C�v�ɉ����ă��\�[�X���擾����
   switch (m_type)
   {
   case 1:
	  m_model = yamadalib::Resources::GetInstance()->GetModel("Floor_01");
	  m_texture = yamadalib::Resources::GetInstance()->GetTexture("Floor01_DDS");
	  break;
   case 2:
	  m_model = yamadalib::Resources::GetInstance()->GetModel("Floor_02");
	  m_texture = yamadalib::Resources::GetInstance()->GetTexture("Floor02_DDS");
	  break;
   case 3:
	  m_model = yamadalib::Resources::GetInstance()->GetModel("Floor_03");
	  m_texture = yamadalib::Resources::GetInstance()->GetTexture("Floor03_DDS");
	  break;
   }
   //�����蔻��̓o�^
   InitializeCollision();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Floor::Update(const float elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
}


/// <summary>
/// �`�悷��
/// </summary>
void Floor::Render()
{
	// ���[���h�s��
	DirectX::SimpleMath::Matrix world =
	   DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
	   DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	//���f����`�悷��
	m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_texture.Get(), world, DirectX::SimpleMath::Vector2(GetScale().x, GetScale().z));
}


/// <summary>
/// �N���b�h�𐶐��֐�
/// </summary>
/// <param name="x">���_��X���W</param>
/// <param name="y">���_��Y���W</param>
/// <param name="z">���_��Z���W</param>
/// <param name="u">�e�N�X�`��U���W<</param>
/// <param name="v">�e�N�X�`��V���W<</param>
/// <returns></returns>
inline DirectX::VertexPositionTexture Floor::MakeVPT(float x, float y, float z, float u, float v) 
{
   return DirectX::VertexPositionTexture(DirectX::XMFLOAT3(x, y, z), DirectX::XMFLOAT2(u, v));
}


/// <summary>
/// ��n������
/// </summary>
void Floor::Finalize()
{
	// do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Floor::InitializeCollision()
{
   //�����蔻��̐ݒ�
   CollisionManager::Collision collision = CollisionManager::GetInstance().CreateBoxCollision(
	  CollisionManager::CollisionPurpose::PHYSICAL,						   //�����蔻��ړI
	  CollisionManager::CollisionType::HIT,								   //����̃^�C�v
	  DirectX::BoundingBox(GetPosition(), GetScale() / 2));				   //���E�{�b�N�X

   //�����蔻��̃p�����[�^��ݒ�(�I�u�W�F�N�g�̃|�C���^�A�I�u�W�F�N�g�^�C�v�A�@�����蔻��A �Փˎ��̏����֐�)
   CollisionManager::CollisionParameter param = { this, CollisionManager::ObjectType::FLOOR, collision };
   // �����蔻��̓o�^
   m_collisionObject = CollisionManager::GetInstance().RegisterObject(param, m_objectStageNumber);
}
