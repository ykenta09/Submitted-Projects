/*
	@file	ISubject.h
	@brief	�I�u�U�[�o�[�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef ISUBJECT_DEFINED
#define ISUBJECT_DEFINED

#include "Game/Interface/IComponent.h"

// �O���錾
class IComponent;

class ISubject
{
public:
	// �f�X�g���N�^
	virtual ~ISubject() = default;
	// �ώ@�҂��A�^�b�`����
	virtual void Attach(const DirectX::Keyboard::Keys& key, IComponent* observer) = 0;
	// �ώ@�҂��f�^�b�`����
	virtual void Detach(const DirectX::Keyboard::Keys& key, IComponent* observer) = 0;
	// �ʒm����
	virtual void Notify(const DirectX::Keyboard::State& keyboardState,
		const std::vector<int>& playerNodeNumbers) = 0;
};
#endif // ISUBJECT_DEFINED

