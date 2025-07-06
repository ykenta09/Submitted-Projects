/*
	@file	CommonResources.cpp
	@brief	�V�[���֓n���A�Q�[�����Ŏg�p���鋤�ʃ��\�[�X
*/
#include "pch.h"
#include "CommonResources.h"
#include <cassert>

/// <summary>
/// �R���X�g���N�^
/// </summary>
CommonResources::CommonResources()
	:
	m_stepTimer{ nullptr },
	m_deviceResources{ nullptr },
	m_commonStates{ nullptr },
	m_debugString{ nullptr },
	m_inputManager{ nullptr }
{
}


/// <summary>
/// ����������
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <param name="dr">�f�o�C�X���\�[�X</param>
/// <param name="commonStates">�R�����X�e�[�g</param>
/// <param name="debugString">�f�o�b�N�X�g�����O</param>
/// <param name="inputManager">���̓��C�A�E�g</param>
/// <param name="blackboard">�o�b�N�{�[�h</param>
void CommonResources::Initialize(
	DX::StepTimer* timer,
	DX::DeviceResources* dr,
	DirectX::CommonStates* commonStates,
	mylib::DebugString* debugString,
	mylib::InputManager* inputManager,
	mylib::BlackBoard* blackboard
)
{
	assert(timer);
	assert(dr);
	assert(commonStates);
	assert(debugString);
	assert(inputManager);
	assert(blackboard);


	m_stepTimer = timer;
	m_deviceResources = dr;
	m_commonStates = commonStates;
	m_debugString = debugString;
	m_inputManager = inputManager;
	m_blackboard = blackboard;

}


