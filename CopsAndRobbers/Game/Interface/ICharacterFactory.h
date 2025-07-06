/*
	@file	ICharacterFactory.h
	@brief	キャラクターファクトリーのインターフェイスクラス
*/
#ifndef ICHARACTER_FACTORY
#define ICHARACTER_FACTORY

#pragma once
#include "Game/Interface/IComponent.h"

// 前方宣言
class CommonResources;

class ICharacterFactory
{
public:
	//プレイヤーを生成する
	virtual std::unique_ptr<IComponent> CreatePlayer(
		CommonResources* resources,
		IComponent* object,
		const DirectX::SimpleMath::Vector3 initialPosition, 
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;
	//仲間を生成する
	virtual std::unique_ptr<IComponent> CreateFellow(
		CommonResources* resources,
		IComponent* object,
		const DirectX::SimpleMath::Vector3 initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;

	//敵を生成する
	virtual std::unique_ptr<IComponent> CreateEnemy(
		CommonResources* resources,
		IComponent* object,
		const int& number,
		const DirectX::SimpleMath::Vector3 initialPosition,
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale) = 0;


};
#endif // ICHARACTER_FACTORY
