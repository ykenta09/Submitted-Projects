/*
	@file	CharacterPartsFactory.h
	@brief	キャラクターのパーツファクトリークラス
*/
#pragma once
#ifndef CHARACTER_FACTORY_DEFINED
#define CHARACTER_FACTORY_DEFINED

#include "Game/Interface/IComponent.h"
#include "Game/Interface/ICharacterPartsFactory.h"
#include "Game/Character/Base/CharacterBase.h"

class CharacterPartsFactory : public ICharacterPartsFactory
{
public:
	//キャラクターのベースを生成する
	static std::unique_ptr<IComponent> CreateCharacter(
		IComponent* parent,
		CommonResources* resources,
		const DirectX::SimpleMath::Vector3 initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale);
	// 頭を生成する
	static std::unique_ptr<IComponent> CreateHeadParts(
		IComponent* parent,
		CommonResources* resources,
		const CharacterBase::CharacterResources& model,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale);
	// 胴体を生成する
	static std::unique_ptr<IComponent> CreateBodyParts(
		IComponent* parent,
		CommonResources* resources,
		const CharacterBase::CharacterResources& model,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale);
	//腕を生成する
	static std::unique_ptr<IComponent> CreateArmParts(
		IComponent* parent,
		CommonResources* resources,
		const CharacterBase::CharacterResources& model,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale,
		const CharacterBase::PartID& partId);
	//足を生成する
	static std::unique_ptr<IComponent> CreateFootParts(
		IComponent* parent,
		CommonResources* resources,
		const CharacterBase::CharacterResources& model,
		const DirectX::SimpleMath::Vector3& initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale,
		const CharacterBase::PartID& partId);
};
#endif // CHARACTER_FACTORY_DEFINED

