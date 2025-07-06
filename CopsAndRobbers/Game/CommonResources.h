/*
	@file	CommonResources.h
	@brief	�V�[���֓n���A�Q�[�����Ŏg�p���鋤�ʃ��\�[�X
*/
#pragma once
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
namespace DX
{
	class StepTimer;
	class DeviceResources;
}
namespace mylib
{
	class DebugString;
	class InputManager;
	class BlackBoard;
}


// ���ʃ��\�[�X
class CommonResources
{
private:
	// �󂯓n�����郊�\�[�X�ꗗ
	DX::StepTimer* m_stepTimer;

	DX::DeviceResources* m_deviceResources;

	DirectX::CommonStates* m_commonStates;

	mylib::DebugString* m_debugString;

	mylib::InputManager* m_inputManager;

	mylib::BlackBoard* m_blackboard;

public:
	CommonResources();
	~CommonResources() = default;

	void Initialize(
		DX::StepTimer* timer,
		DX::DeviceResources* dr,
		DirectX::CommonStates* commonStates,
		mylib::DebugString* debugString,
		mylib::InputManager* inputManager,
		mylib::BlackBoard* blackboard
	);

	// getter
	DX::StepTimer* GetStepTimer() const
	{
		return m_stepTimer;
	}

	DX::DeviceResources* GetDeviceResources() const
	{
		return m_deviceResources;
	}

	DirectX::CommonStates* GetCommonStates() const
	{
		return m_commonStates;
	}

	mylib::DebugString* GetDebugString() const
	{
		return m_debugString;
	}

	mylib::InputManager* GetInputManager() const
	{
		return m_inputManager;
	}
	mylib::BlackBoard* GetBlackBoard() const
	{
		return m_blackboard;
	}
};
