/*
	@file	Foot.cpp
	@brief	���N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Character/Base/Parts/Foot.h"
#include "Game/Factory/CharacterPartsFactory.h"
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="model">���f��</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
/// <param name="partId">�p�[�c��ID</param>
Foot::Foot(
	IComponent* parent,
	CommonResources* resources,
	const CharacterBase::CharacterResources& model,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle,
	const DirectX::SimpleMath::Vector3& scale,
	CharacterBase::PartID partId)
	:
	m_parent( parent),
	m_commonResources(resources),
	m_nodeNumber(CharacterBase::GetNodeCountAfterCountUp()),
	m_partNumber(CharacterBase::GetPartsNumber()),
	m_partID(partId),
	m_initialPosition(initialPosition),
	m_initialAngle(initialAngle),
	m_currentPosition{},
	m_currentAngle{},
	m_model( model.foot ),
    m_modelDDS(model.footDDS),
	m_scale( scale ),
	m_direction(DirectX::XMConvertToRadians(FOOT_SWING_INIT_DEG)),
	m_rotaY(DirectX::XMConvertToRadians(FOOT_ROT_OFFSET_Y)),
	m_isMoving( false )
{
    //�O���t�B�b�N�X�̃C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Foot::~Foot()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
void Foot::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�X�V�^�[�Q�b�g���W</param>
inline void Foot::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	UNREFERENCED_PARAMETER(target);

	// �ʒu���ω����Ă���Έړ����t���O�𗧂Ă�
	if (m_currentPosition != position)
		m_isMoving = true;
	else
		m_isMoving = false;

	//���݂̏����X�V����
	m_currentPosition = position;
	m_currentAngle = angle;

	//�ړ����Ȃ瑫�𓮂���
	Move();

	//���[���h�s��𐶐�����
	m_worldMatrix =
	    DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
		DirectX::SimpleMath::Matrix::CreateRotationX(m_rotaY) *
		DirectX::SimpleMath::Matrix::CreateTranslation(m_initialPosition) *
		DirectX::SimpleMath::Matrix::CreateRotationY(m_currentAngle) *
		DirectX::SimpleMath::Matrix::CreateTranslation(m_currentPosition);
}

/// <summary>
/// �`�悷��
/// </summary>
inline void Foot::Render()
{
	// ���f����`�悷��
    m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_modelDDS.Get(), m_worldMatrix);
}

/// <summary>
/// ��n������
/// </summary>
void Foot::Finalize()
{
	// do nothing.
}

/// <summary>
/// �L�[�{�[�h�ʒm
/// </summary>
/// <param name="keys">�������L�[</param>
inline void Foot::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// �ړ������𓮂���
/// </summary>
void Foot::Move()
{
	//�����Ă���Ƃ��������𓮂���
	if (!m_isMoving)
	{
	   // �����W����K�p���ĉ�]�p�x�����X�ɖ߂�
	   // 0 �` 1 �͈̔͂Œ����\�i1 �ɋ߂Â��قǂ�����茸���j
	   m_rotaY *= FOOT_SWING_DECAY;
	   return;
	}

	// ���𓮂������������E�Ŕ��]������
	if (m_partID == CharacterBase::LEFT_FOOT)
	{
	   m_rotaY += m_direction;  // ����: ���̕����ɉ�]
	   if (m_rotaY >= DirectX::XMConvertToRadians(FOOT_SWING_LIMIT_DEG) || m_rotaY <= DirectX::XMConvertToRadians(-FOOT_SWING_LIMIT_DEG))
	   {
		  m_direction *= -1;
	   }
	}
	else
	{
	   m_rotaY -= m_direction;  // �E��: ���̕����ɉ�]
	   if (m_rotaY <= DirectX::XMConvertToRadians(-FOOT_SWING_LIMIT_DEG) || m_rotaY >= DirectX::XMConvertToRadians(FOOT_SWING_LIMIT_DEG))
	   {
		  m_direction *= -1;
	   }
	}
}
