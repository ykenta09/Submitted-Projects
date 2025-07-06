/*
	@file	IState.h
	@brief	�X�e�[�g�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED

// �O���錾
class CommonResources;

class IState
{
public:
	virtual ~IState() = default;

	virtual void Initialize() = 0;

	virtual void Update(const float& elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;
};
#endif // ISTATE_DEFINED
