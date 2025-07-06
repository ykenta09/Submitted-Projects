/*
	@file	MouseControl.cpp
	@brief	�}�E�X����N���X
*/
#include "pch.h"
#include "MouseControl.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Libraries/yamadaLib/Math.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="commonResources"></param>
MouseControl::MouseControl(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
    m_yaw{},
    m_pitch{},
	m_mouseMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MouseControl::~MouseControl()
{
	// do nothing.
}

/// <summary>
/// ����������
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
void MouseControl::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	//��]�p������������
	m_yaw = 0.0f;
	m_pitch = 0.0f;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void MouseControl::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	//�}�E�X�̏�Ԃ��擾����
	m_mouseState = m_commonResources->GetInputManager()->GetMouseState();

	//�N�H�[�^�j�I���̌v�Z������
	QuaternionCalculation();
}


/// <summary>
/// �N�H�[�^�j�I���̌v�Z������
/// </summary>
void MouseControl::QuaternionCalculation()
{
	// �}�E�X�̑��Έړ��ʂ��擾���ĉ�]���X�V����
	float deltaYaw = DirectX::XMConvertToRadians(static_cast<float>(-m_mouseState.x)) * 0.08f;
	float deltaPitch = DirectX::XMConvertToRadians(static_cast<float>(-m_mouseState.y)) * 0.03f;

	//�L�[����
	OnKeyPressed();

	// �}�E�X���W���猻�݂̊p�x���X�V
	m_yaw += deltaYaw;
	m_pitch += deltaPitch;
	//Y���̉�]�ɐ������ۂ�
	m_pitch = yamadalib::Math::Clamp(m_pitch, -0.3f, 0.3f);

	// �N�H�[�^�j�I���𐶐�����
	DirectX::SimpleMath::Quaternion pitchQuat = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_pitch);
	DirectX::SimpleMath::Quaternion yawQuat = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, m_yaw);

	// �N�H�[�^�j�I�����������čs��ɕϊ�
	DirectX::SimpleMath::Quaternion combinedQuat = pitchQuat * yawQuat;
	m_mouseMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(combinedQuat);
}

/// <summary>
/// �L�[�{�[�h����
/// </summary>
void MouseControl::OnKeyPressed()
{
   //�L�[�{�[�h�̏�Ԃ��擾����
   m_keyboardState = DirectX::Keyboard::Get().GetState();
   if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Left)) //��������
   {
	  m_yaw += 0.06f;
   }
   else if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Right))  //�E������
   {
	  m_yaw -= 0.06f;
   }
   if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Up))	//�������
   {
	  m_pitch += 0.03f;
   }
   else if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Down))	  //��������
   {
	  m_pitch -= 0.03f;
   }
}
