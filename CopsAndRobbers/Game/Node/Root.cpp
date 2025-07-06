#include "pch.h"
#include "Game/Node/Root.h"
#include "Game/Character/Enemy/Enemy.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
Root::Root(IComponent* parent,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle)
	:
	m_parent(parent),
	m_nodeNumber{},
	m_partNumber{},
	m_partID{},
	m_nodes{},
	m_initialPosition(initialPosition),
	m_initialAngle(initialAngle),
	m_currentPosition(DirectX::SimpleMath::Vector3::Zero),
	m_currentAngle{},
	m_targetPosition{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Root::~Root()
{
   Finalize();
}

/// <summary>
///����������
/// </summary>
void Root::Initialize()
{
}

/// <summary>
/// �L�[�{�[�h�ʒm����
/// </summary>
/// <param name="keys">�����ꂽ�L�[</param>
inline void Root::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="position">�X�V���W</param>
/// <param name="angle">�X�V��]�p</param>
/// <param name="target">�^�[�Q�b�g�̍X�V���W</param>
void Root::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	//�m�[�h���X�V����
	for (auto& node : m_nodes)
	{
		node->Update(elapsedTime, position, angle, target);
	}
}


/// <summary>
/// �p�[�c��ǉ�����
/// </summary>
/// <param name="parts">�ǉ�����L�����N�^�[�p�[�c</param>
void Root::Attach(std::unique_ptr<IComponent> parts)
{
	// �m�[�h��ǉ�����
	m_nodes.emplace_back(std::move(parts));
	// �v���C���[���ǂ����𔻒�
	if (auto* player = m_nodes.back()->AsPlayer())
	{
	   // m_player �Ƀv���C���[�̃|�C���^��ݒ�
	   m_player = player;
	}
	// �G���ǂ����𔻒�
	if (auto* enemy = m_nodes.back()->AsEnemy())
	{
	   m_enemies.push_back(enemy);
	}
	// ���Ԃ��ǂ����𔻒�
	if (auto* fellow = m_nodes.back()->AsFellow())
	{
	   m_fellows.push_back(fellow);
	}
}


/// <summary>
/// �p�[�c���폜����
/// </summary>
/// <param name="parts">�폜����p�[�c</param>
void Root::Detach(std::unique_ptr<IComponent> parts)
{
	// ���i���폜����
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
		[&](const std::unique_ptr<IComponent>& parts)
		{
			return parts.get() == parts.get();
		}
	);
	// �폜���镔�i�����������ꍇ�͕��i���폜����
	if (it != m_nodes.end())
	{
		m_nodes.erase(it);
	}
}

/// <summary>
/// �`�悷��
/// </summary>
void Root::Render()
{
	// �m�[�h��`�悷��
	for (auto& node : m_nodes)
	{
		node->Render();
	}
}

/// <summary>
/// �㏈��������
/// </summary>
void Root::Finalize()
{
	//�m�[�h���N���A
	m_nodes.clear();
	m_enemies.clear();
	m_fellows.clear();
}
