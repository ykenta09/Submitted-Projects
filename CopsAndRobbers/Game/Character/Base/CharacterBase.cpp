/*
	@file	CharacterBase.cpp
	@brief	�L�����N�^�[�x�[�X�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Factory/CharacterPartsFactory.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/yamadaLib/Math.h"

// �p�[�c�J�E���^�[
int CharacterBase::s_partsNumber = 0;
// �m�[�h�J�E���^�[
int CharacterBase::s_nodeCount = 0;

/// <summary>
/// 
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
CharacterBase::CharacterBase(
	IComponent* parent,
	CommonResources* resources,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle,
	const DirectX::SimpleMath::Vector3& scale)
	:
	m_parent(parent),
	m_commonResources(resources),
	m_nodeNumber(CharacterBase::GetNodeCountAfterCountUp()),
	m_partNumber(CharacterBase::GetPartsNumber()),
	m_partID(CharacterBase::Character),
	m_initialPosition(initialPosition),
	m_initialAngle(DirectX::XMConvertToRadians(initialAngle)),
	m_scale( scale ),
    m_currentPosition{ DirectX::SimpleMath::Vector3::Zero },
    m_currentAngle{ 0.0f },
	m_moveAngle( 0.0f ),
    m_time(0.0f),
    m_objectStageNumber(0),
	m_shadow{},
    m_applyInitialRotation(true)
{
    //�O���t�B�b�N�X�̃C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CharacterBase::~CharacterBase()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void CharacterBase::Initialize()
{
	using namespace DirectX::SimpleMath;

	//�v���C���[�̃p�[�c�𐶐�
	CharacterBase::Attach(CharacterPartsFactory::CreateBodyParts(this, m_commonResources, m_modelResources, BODY_POSITION, m_initialAngle, Vector3::One));
	CharacterBase::Attach(CharacterPartsFactory::CreateHeadParts(m_parent, m_commonResources, m_modelResources, HEAD_POSITION, m_initialAngle, Vector3::One));
	CharacterBase::Attach(CharacterPartsFactory::CreateArmParts(m_parent, m_commonResources, m_modelResources, LEFT_ARM_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::LEFT_ARM));
	CharacterBase::Attach(CharacterPartsFactory::CreateArmParts(m_parent, m_commonResources, m_modelResources, RIGHT_ARM_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::RIGHT_ARM));
	CharacterBase::Attach(CharacterPartsFactory::CreateFootParts(m_parent, m_commonResources, m_modelResources, LEFT_FOOT_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::LEFT_FOOT));
	CharacterBase::Attach(CharacterPartsFactory::CreateFootParts(m_parent, m_commonResources, m_modelResources, RIGHT_FOOT_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::RIGHT_FOOT));

	//�e�𐶐�����
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Initialize();

	m_currentPosition = m_initialPosition;
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�X�V�^�[�Q�b�g���W</param>
void CharacterBase::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	m_time = elapsedTime;

	//���݂̏����X�V
	m_currentPosition = position;

	//m_currentPosition.y -= GRAVITY_SCALE;

	if(m_applyInitialRotation)
	{
	   m_currentAngle = m_initialAngle;
	}
	else
	{
	   // ���`��Ԃŉ�]�����X�ɕω�������
	   m_currentAngle = yamadalib::Math::LerpAngle(m_currentAngle, angle, elapsedTime * ROTATE_LERP_SPEED); // ��]���x�i�����\�j;
	}

	// �L�����N�^�[�p�[�c���X�V����
	for (auto& characterPart : m_characterParts)
	{
		characterPart->Update(elapsedTime, m_currentPosition, m_currentAngle, target);
	}
}

/// <summary>
/// �p�[�c��ǉ�����
/// </summary>
/// <param name="characterParts">�ǉ�����p�[�c</param>
void CharacterBase::Attach(std::unique_ptr<IComponent> characterParts)
{
	// ����������
	m_characterParts.emplace_back(std::move(characterParts));
}

/// <summary>
/// �p�[�c���폜����
/// </summary>
/// <param name="characterParts">�폜����p�[�c</param>
void CharacterBase::Detach(std::unique_ptr<IComponent> characterParts)
{
}

/// <summary>
/// �`�悷��
/// </summary>
inline void CharacterBase::Render()
{
	//�e��`�悷��
	m_shadow->Render(DirectX::SimpleMath::Vector3(m_currentPosition.x, SHADOW_Y, m_currentPosition.z));
	
	//�L�����N�^�[�p�[�c��`�悷��
	for (auto& characterPart : m_characterParts)
	{
		characterPart->Render();
	}
}

/// <summary>
/// ��n������
/// </summary>
void CharacterBase::Finalize()
{
	//�L�����N�^�[�p�[�c�����Z�b�g����
	m_characterParts.clear();
}

/// <summary>
/// �L�[�{�[�h�ʒm
/// </summary>
/// <param name="keys">�������L�[</param>
inline void CharacterBase::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// �Փ˂����Ƃ��ɃL�����N�^�[�������o��
/// </summary>
/// <param name="position">�����o�����W</param>
void CharacterBase::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_currentPosition = position;
	//�L�����N�^�[�p�[�c�̍X�V������
	for (auto& characterPart : m_characterParts)
	{
		characterPart->Update(m_time, m_currentPosition, m_angleY + m_moveAngle, DirectX::SimpleMath::Vector3::Zero);
	}
}

/// <summary>
/// ������]�𖳌��ɂ���
/// </summary>
void CharacterBase::LostInitialRotation()
{
   m_applyInitialRotation = false;
}

/// <summary>
/// �p�[�c���f����ݒ肷��
/// </summary>
/// <param name="resources">���f���̃��\�[�X</param>
void CharacterBase::SetPartModels(const CharacterResources& resources)
{
   m_modelResources = resources;
}
