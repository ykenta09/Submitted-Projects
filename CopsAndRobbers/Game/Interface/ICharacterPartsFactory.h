/*
	@file	ICharacterPartsFactory.h
	@brief	キャラクターパーツファクトリーのインターフェイスクラス
*/
#ifndef ICHARACTER_PARTS_FACTORY
#define ICHARACTER_PARTS_FACTORY

#pragma once
#include "Game/Interface/IComponent.h"

// 前方宣言
class CommonResources;

class ICharacterPartsFactory
{
public:
	//キャラクターパーツを生成する
	virtual std::unique_ptr<IComponent> CreateParts(
		CommonResources* resources,
		IComponent* object,
		const DirectX::SimpleMath::Vector3 initialPosition, 
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;
};
#endif // ICHARACTER_PARTS_FACTORY
