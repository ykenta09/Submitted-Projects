/*
	@file	Vertex.h
	@brief	�Z�����_�N���X
*/
#pragma once
#include "SimpleMath.h"

class CellVertex
{
public:
   //�Z���̏��
   int GetState() const { return m_state; }
   //�Z���̏�Ԃ�ݒ肷��
   void SetState(int state) { m_state = state; }
   //���_�ԍ����擾����
   const int GetVertexNumber() const { return m_vertexNumber; }
   //���_�ԍ���ݒ肷��
   void SetVertexNumber(const int& vertexNumber) { m_vertexNumber = vertexNumber; }
   //���_�̍��W���擾����
   DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
   //���_�̈ʒu��ݒ肷��
   void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
   //�e�C���f�b�N�X���擾����
   int GetParentVertexIndex() { return m_parentIndex; }
   //�e�C���f�b�N�X��ݒ肷��
   void SetParentVertexIndex(const int& parentIndex) { m_parentIndex = parentIndex; }
   //�J�n�m�[�h���猻�݂̃m�[�h�܂ł̃R�X�g���擾����
   float GetCost() const { return m_cost; }
   //�R�X�g��ݒ肷��
   void SetCost(const float& cost) { m_cost = cost; }
   //�q���[���X�e�B�b�N���擾����
   float GetHeuristic() const { return m_heuristic; }
   //�q���[���X�e�B�b�N���擾����
   void SetHeuristic(const float& heuristic) { m_heuristic = heuristic; }
   //�X�R�A���擾����
   float GetScore() const { return m_score; }
   //�X�R�A��ݒ肷��
   void SetScore(const float& score) { m_score = score; }
public:
   //�R���X�g���N�^
   CellVertex();
   CellVertex(const DirectX::SimpleMath::Vector3& position);
   CellVertex(
	  int vertexNumber,
	  int parentIndex,
	  const DirectX::SimpleMath::Vector3& position,
	  const float& cost,
	  const float& heuristic,
	  const float& score
   );
   //�f�X�g���N�^
   ~CellVertex();

   //�N���A
   void Clear();

private:
   //�O�F�ʍs�\�@�P�F��Q��
   int m_state;       
   //�ő咸�_�ԍ�
   static int s_maxVertexNumber;
   //���_�ԍ�
   int m_vertexNumber;
   //�ʒu
   DirectX::SimpleMath::Vector3 m_position;
   //�J�n�m�[�h���猻�݂̃m�[�h�̃R�X�g
   float m_cost;
   //�e�m�[�h�̒��_�C���f�b�N�X
   int m_parentIndex;
   //�q���[���X�e�B�b�N�R�X�g
   float m_heuristic;
   //���X�R�A
   float m_score;
};

