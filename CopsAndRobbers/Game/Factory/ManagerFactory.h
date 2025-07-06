/*
	@file	ManagerFactory.h
	@brief	マネージャーファクトリークラス
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
   //ゲームマネージャーを生成する
   static std::unique_ptr<INonObject> CreateGameManager(CommonResources* resources, Map* map);
};
#endif //MANAGER_FACTORY_DEFINED

