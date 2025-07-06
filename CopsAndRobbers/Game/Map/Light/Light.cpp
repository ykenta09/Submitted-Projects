/*
	@file	Light.cpp
	@brief	���C�g�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Light/Light.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="param">�I�u�W�F�N�g�p�����[�^</param>
/// <param name="lightColor">���C�g�J���[</param>
Light::Light(CommonResources* resources, const GameParameters::ObjectParameter& param, const DirectX::SimpleMath::Vector3& lightColor, const float& range, const bool& modelDraw)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_commonResources(resources),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_color(lightColor),
    m_range(range),
    m_modelDraw(modelDraw),
    m_collisionObject(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Light::~Light()
{
}

/// <summary>
/// ����������
/// </summary>
void Light::Initialize()
{
	// ���f����ǂݍ���
	SetModel(yamadalib::Resources::GetInstance()->GetModel("Light"));
	m_dds = yamadalib::Resources::GetInstance()->GetTexture("Light_DDS");

	//���C�g�̐ݒ�
	SettingLight();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Light::Update(const float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}


/// <summary>
/// �`�悷��
/// </summary>
void Light::Render()
{
   //���f���`�悪�s�v�ȏꍇ�͕`�悵�Ȃ�
   if (!m_modelDraw) return;
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
void Light::Finalize()
{
	// do nothing.
}

/// <summary>
/// �Փ˔���̓o�^
/// </summary>
void Light::InitializeCollision()
{
}

/// <summary>
/// �Փˎ��̃C�x���g����
/// </summary>
/// <param name="other">�ՓˑΏۂ̃R���W�����I�u�W�F�N�g</param>
void Light::HandleCollision(const CollisionObject* other)
{
   UNREFERENCED_PARAMETER(other);
}

/// <summary>
/// ���C�g�̐ݒ�
/// </summary>
void Light::SettingLight()
{
   //���C�g�̃p�����[�^�ݒ�
   CustomModelRenderer::PointLight lights;
   lights.position = GetPosition() - DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
   lights.range = m_range;
   lights.color = m_color;
   m_graphics->GetCustomModelRenderer()->SetPointLightParameters(lights);
}
