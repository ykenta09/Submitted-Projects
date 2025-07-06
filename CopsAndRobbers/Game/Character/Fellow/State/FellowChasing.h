/*
	@file	FellowChasing.h
	@brief	���Ԃ̒ǐՏ�ԃN���X
*/
#ifndef FELLOW_CHASING_DEFINED
#define FELLOW_CHASING_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;
class Fellow;

class FellowChasing : public IState
{
public:
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_targetPosition = target; }
	bool GeteGoalFlag() { return m_isGoal; }
	void SetGoalFlag(bool flag) { m_isGoal = flag; }
public:
	//�R���X�g���N�^
	FellowChasing(Fellow* fellow);
	//�f�X�g���N�^
	~FellowChasing();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
	void Move(const float& elapsedTime);
	void TargetMove(const DirectX::SimpleMath::Vector3& target, const float& elapsedTime);
	//�T��
	void Search();
	//�󂯎�����ڕW�n�_�Ɉړ�����
private:
	//����
	Fellow* m_fellow;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X�@�f�o�b�N�p
	yamadalib::Graphics* m_graphics;

	//�^�[�Q�b�g�̍��W
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//�X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	//���E�{�b�N�X
	DirectX::BoundingBox m_boundingBox;
	//�ړI�n�ɒB������
	bool m_isArrived;

	//�S�[���������ǂ���
	bool m_isGoal;
};
#endif // FELLOW_CHASING_DEFINED
