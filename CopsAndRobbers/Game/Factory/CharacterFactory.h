/*
	@file	CharacterFactory.h
	@brief	�L�����N�^�[�t�@�N�g���[�N���X
*/
#pragma once
#ifndef CHARACTER_FACTORY_DEFINED
#define CHARACTER_FACTORY_DEFINED

#include "Game/Interface/IComponent.h"
#include "Game/Interface/ICharacterFactory.h"

#include "Game/Character/Player/Player.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Enemy/Enemy.h"

#include "GraphEditor/GraphScene.h"
#include "Libraries/yamadaLib/GameParameter.h"


class CharacterFactory : public ICharacterFactory
{   
public:
    //�L�����N�^�[�𐶐�����
   static std::unique_ptr<IComponent> CreateCharacter(
	  IComponent* parent,
	  CommonResources* resources,
	  Map* map,
	  const GameParameters::CharacterData data,
	  const int& stageNumber,
	  LoadJson* json);
	//�v���C���[�𐶐�����
	static std::unique_ptr<IComponent> CreatePlayer(
		IComponent* parent,
		CommonResources* resources,
	    const GameParameters::CharacterData data,
	    const int& stageNumber);

	//���Ԃ𐶐�����
	static std::unique_ptr<IComponent> CreateFellow(
		IComponent* parent,
		CommonResources* resources,
	    Map* map,
	    const GameParameters::CharacterData data, 
	    const int& stageNumber);

	//�G�𐶐�����
	static std::unique_ptr<IComponent> CreateEnemy(
		IComponent* parent,
		CommonResources* resources,
		const int& number,
	    Map* map,
	    const GameParameters::CharacterData data,
	    const int& stageNumber,
	    LoadJson* json);

};
#endif // CHARACTER_FACTORY_DEFINED

