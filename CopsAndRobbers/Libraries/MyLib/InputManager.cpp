/*
	@file	InputManager.cpp
	@brief	�}�E�X�ƃL�[�{�[�h�̓��͂𐿂������N���X
*/
#include "pch.h"
#include "InputManager.h"

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
mylib::InputManager::InputManager(const HWND& window)
	:
	m_mouseState{},
	m_keyboardState{}
{
	// �}�E�X���g�p�ł����Ԃɂ���
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(window);
	//m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();

	// �L�[�{�[�h���g�p�ł����Ԃɂ���
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
}

//---------------------------------------------------------
// �}�l�[�W���̎����\�[�X���X�V����
//---------------------------------------------------------
void mylib::InputManager::Update()
{
	// �}�E�X�̏����X�V����
	m_mouseState = m_mouse->GetState();
	m_mouseTracker->Update(m_mouseState);

	// �L�[�{�[�h�̏����X�V����
	m_keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(m_keyboardState);
}
