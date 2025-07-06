/*
	@file	ICharacterPartsFactory.h
	@brief	�L�����N�^�[�p�[�c�t�@�N�g���[�̃C���^�[�t�F�C�X�N���X
*/
#ifndef ICHARACTER_PARTS_FACTORY
#define ICHARACTER_PARTS_FACTORY

#pragma once
#include "Game/Interface/IComponent.h"

// �O���錾
class CommonResources;

class ICharacterPartsFactory
{
public:
	//�L�����N�^�[�p�[�c�𐶐�����
	virtual std::unique_ptr<IComponent> CreateParts(
		CommonResources* resources,
		IComponent* object,
		const DirectX::SimpleMath::Vector3 initialPosition, 
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;
};
#endif // ICHARACTER_PARTS_FACTORY
