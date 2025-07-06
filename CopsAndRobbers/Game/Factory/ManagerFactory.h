/*
	@file	ManagerFactory.h
	@brief	�}�l�[�W���[�t�@�N�g���[�N���X
*/
#pragma once
#ifndef MANAGER_FACTORY_DEFINED
#define MANAGER_FACTORY_DEFINED
#include <memory>
#include <string>
#include "Game/GameManager/GameManager.h"
#include "Game/Interface/INonObject.h"


class ManagerFactory
{
public:
   //�Q�[���}�l�[�W���[�𐶐�����
   static std::unique_ptr<INonObject> CreateGameManager(CommonResources* resources, Map* map);
};
#endif //MANAGER_FACTORY_DEFINED

