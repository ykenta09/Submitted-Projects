#include "pch.h"
#include "Game/Astar/CellVertex.h"


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
CellVertex::CellVertex()
   :
   m_position(DirectX::SimpleMath::Vector3::Zero),
   m_vertexNumber(0),
   m_parentIndex(0),
   m_state(0),
   m_score(0.0f),
   m_heuristic(0.0f),
   m_cost(0.0f)
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">�쐬������W</param>
CellVertex::CellVertex(const DirectX::SimpleMath::Vector3& position)
   :
   m_position(position),
   m_vertexNumber(0),
   m_parentIndex(0),
   m_state(0),
   m_score(0.0f),
   m_heuristic(0.0f),
   m_cost(0.0f)
{
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="vertexNumber">���_�ԍ�</param>
/// <param name="parentIndex">�e�m�[�h�̒��_�C���f�b�N�X</param>
/// <param name="position">���W</param>
/// <param name="cost">�R�X�g</param>
/// <param name="heuristic">�q���[���X�e�B�b�N�R�X�g</param>
/// <param name="score">�X�R�A</param>
CellVertex::CellVertex(
   int vertexNumber, 
   int parentIndex, 
   const DirectX::SimpleMath::Vector3& position,
   const float& cost, 
   const float& heuristic,
   const float& score)
   :
   m_vertexNumber(vertexNumber),
   m_parentIndex(parentIndex),
   m_position(position),
   m_cost(cost),
   m_heuristic(heuristic),
   m_score(score)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
CellVertex::~CellVertex()
{
}

/// <summary>
/// �N���A
/// </summary>
void CellVertex::Clear()
{
}
