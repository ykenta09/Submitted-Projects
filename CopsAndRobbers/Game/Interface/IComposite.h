/*
	@file	IPlayer.h
	@brief	�v���C���[�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef ICOMPOSITE_DEFINED
#define ICOMPOSITE_DEFINED

#include "Game/Interface/IComponent.h"


class IComposite : public IComponent
{
public:
	//�p�[�c��ǉ�����
	virtual void Attach(std::unique_ptr<IComponent> playerParts) = 0;
	//�p�[�c���폜����
	virtual void Detach(std::unique_ptr<IComponent> playerParts) = 0;
	virtual IComponent* GetLastAttachedNode() const = 0;

};
#endif // ICOMPOSITE_DEFINED
