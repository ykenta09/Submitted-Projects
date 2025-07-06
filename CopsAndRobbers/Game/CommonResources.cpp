/*
	@file	CommonResources.cpp
	@brief	シーンへ渡す、ゲーム内で使用する共通リソース
*/
#include "pch.h"
#include "CommonResources.h"
#include <cassert>

/// <summary>
/// コンストラクタ
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
/// 初期化する
/// </summary>
/// <param name="timer">経過時間</param>
/// <param name="dr">デバイスリソース</param>
/// <param name="commonStates">コモンステート</param>
/// <param name="debugString">デバックストリング</param>
/// <param name="inputManager">入力レイアウト</param>
/// <param name="blackboard">バックボード</param>
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


