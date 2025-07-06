/*
	@file	CharacterPartsFactory.cpp
	@brief	キャラクターパーツのファクトリークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Factory/CharacterPartsFactory.h"

#include "Game/Character/Base/Parts/Head.h"
#include "Game/Character/Base/Parts/Body.h"
#include "Game/Character/Base/Parts/Arm.h"
#include "Game/Character/Base/Parts/Foot.h"

/// <summary>
/// キャラクターのベースを生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateCharacter(
   IComponent* parent,
   CommonResources* resources,
   const DirectX::SimpleMath::Vector3 initialPosition,
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale)
{
	std::unique_ptr<IComponent> character;
	//インスタンスを生成する
	character.reset(new CharacterBase(parent, resources, initialPosition, initialAngle, scale));
	//初期化する
	character->Initialize();
	//インスタンスを返す
	return std::move(character);
}


/// <summary>
/// 頭を生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="model">モデル</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateHeadParts(
   IComponent* parent, 
   CommonResources* resources, 
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition, 
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale)
{
	//頭を宣言する
	std::unique_ptr<IComponent> headParts;
	//インスタンスを生成する
	headParts.reset(new Head(parent, resources, model,initialPosition, initialAngle, scale));
	//初期化する
	headParts->Initialize();

	//インスタンスを返す
	return std::move(headParts);
}


/// <summary>
/// 胴体を生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="model">モデル</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateBodyParts(
   IComponent* parent, 
   CommonResources* resources,
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition,
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale)
{
	//胴体を宣言する
	std::unique_ptr<IComponent> bodyParts;
	//インスタンスを生成する
	bodyParts.reset(new Body(parent, resources, model, initialPosition, initialAngle, scale));
	//初期化する
	bodyParts->Initialize();

	//インスタンスを返す
	return std::move(bodyParts);
}

/// <summary>
/// 腕を生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="model">モデル</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateArmParts(
   IComponent* parent, 
   CommonResources* resources,
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition,
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale, 
   const CharacterBase::PartID& partId)
{
	//腕を宣言する
	std::unique_ptr<IComponent> armParts;
	//インスタンスを生成する
	armParts.reset(new Arm(parent, resources, model, initialPosition, initialAngle, scale, partId));
	//初期化する
	armParts->Initialize();

	//インスタンスを返す
	return std::move(armParts);
}

/// <summary>
/// 足を生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="model">モデル</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateFootParts(
   IComponent* parent, 
   CommonResources* resources,
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition,
   const float& initialAngle, 
   const DirectX::SimpleMath::Vector3& scale,
   const CharacterBase::PartID& partId)
{
	//足を宣言する
	std::unique_ptr<IComponent> footParts;
	//インスタンスを生成する
	footParts.reset(new Foot(parent, resources, model, initialPosition, initialAngle, scale, partId));
	//初期化する
	footParts->Initialize();

	//インスタンスを返す
	return std::move(footParts);
}
