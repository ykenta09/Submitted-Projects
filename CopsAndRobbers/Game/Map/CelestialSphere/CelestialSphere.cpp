/*
	@file	CelestialSphere.cpp
	@brief	�V���N���X
*/
#include "pch.h"
#include "CelestialSphere.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Libraries/yamadaLib/Resources.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="initialPosition">�������W</param>
/// <param name="initialQuaternion">������]</param>
CelestialSphere::CelestialSphere(const DirectX::SimpleMath::Vector3& initialPosition, const DirectX::SimpleMath::Quaternion& initialQuaternion)
	:
	m_position{ initialPosition },
	m_quaternion { initialQuaternion },
    m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CelestialSphere::~CelestialSphere()
{
}

/// <summary>
/// ����������
/// </summary>
void CelestialSphere::Initialize()
{
	//���f����ǂݍ���
	m_model = yamadalib::Resources::GetInstance()->GetModel("CelestialSphere");
	m_dds = yamadalib::Resources::GetInstance()->GetTexture("CelestialSphere_DDS");
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="centerPosition">�V���̒��S���W</param>
void CelestialSphere::Update(const DirectX::SimpleMath::Vector3& centerPosition)
{
	//���W���X�V����
    m_position = centerPosition;
	//��]������
	m_currentQuaternion *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, ROTATION_SPEED);
}

/// <summary>
/// �`�悷��
/// </summary>
void CelestialSphere::Render()
{
	//���[���h�s����X�V����
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_currentQuaternion) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//���f����`�悷��
	m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_dds.Get(), world);
}

/// <summary>
/// ��n������
/// </summary>
void CelestialSphere::Finalize()
{
	// do nothing.
}


