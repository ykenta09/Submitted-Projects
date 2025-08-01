/*
	@file	CommonResources.h
	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#pragma once
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
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


// 共通リソース
class CommonResources
{
private:
	// 受け渡しするリソース一覧
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
