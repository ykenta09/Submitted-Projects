/*
	@file	Head.cpp
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
#include "Game/Character/Base/Parts/Head.h"
#include "Game/Factory/CharacterPartsFactory.h"

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
Head::Head(
	IComponent* parent,
	CommonResources* resources,
	const CharacterBase::CharacterResources& model,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle,
	const DirectX::SimpleMath::Vector3& scale)
	:
	m_parent(parent),
	m_commonResources(resources),
	m_nodeNumber(CharacterBase::GetNodeCountAfterCountUp()),
	m_partNumber(CharacterBase::GetPartsNumber()),
	m_partID(CharacterBase::HEAD),
	m_initialPosition(initialPosition),
	m_initialAngle(initialAngle),
	m_currentPosition{},
	m_currentAngle{},
	m_model( model.head ),
    m_modelDDS(model.headDDS),
	m_scale( scale )
{
    //�O���t�B�b�N�X�̃C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Head::~Head()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void Head::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�X�V�^�[�Q�b�g���W</param>
inline void Head::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	UNREFERENCED_PARAMETER(target);

	//���݂̏����X�V����
	m_currentPosition = position + m_initialPosition;
	m_currentAngle =  angle;

	//���[���h�s��𐶐�����
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(m_initialPosition) *
	   DirectX::SimpleMath:: Matrix::CreateRotationY(m_currentAngle) *
	   DirectX::SimpleMath:: Matrix::CreateTranslation(m_currentPosition);
}

/// <summary>
/// �`�悷��
/// </summary>
inline void Head::Render()
{
	// ���f����`�悷��
    m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_modelDDS.Get(), m_worldMatrix);
}

/// <summary>
/// ��n������
/// </summary>
void Head::Finalize()
{
	// do nothing.
}

/// <summary>
/// �L�[�{�[�h�ʒm
/// </summary>
/// <param name="keys">�������L�[</param>
inline void Head::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}
