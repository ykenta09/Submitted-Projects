/*
	@file	ICharacterFactory.h
	@brief	�L�����N�^�[�t�@�N�g���[�̃C���^�[�t�F�C�X�N���X
*/
#ifndef ICHARACTER_FACTORY
#define ICHARACTER_FACTORY

#pragma once
#include "Game/Interface/IComponent.h"

// �O���錾
class CommonResources;

class ICharacterFactory
{
public:
	//�v���C���[�𐶐�����
	virtual std::unique_ptr<IComponent> CreatePlayer(
		CommonResources* resources,
		IComponent* object,
		const DirectX::SimpleMath::Vector3 initialPosition, 
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;
	//���Ԃ𐶐�����
	virtual std::unique_ptr<IComponent> CreateFellow(
		CommonResources* resources,
		IComponent* object,
		const DirectX::SimpleMath::Vector3 initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;

	//�G�𐶐�����
	virtual std::unique_ptr<IComponent> CreateEnemy(
		CommonResources* resources,
		IComponent* object,
		const int& number,
		const DirectX::SimpleMath::Vector3 initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;


};
#endif // ICHARACTER_FACTORY
