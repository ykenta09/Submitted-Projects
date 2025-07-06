/*
	@file	ManagerFactory.cpp
	@brief	マネージャーのファクトリークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Factory/ManagerFactory.h"
#include "Game/Observer/Messenger.h"
#include "Game/Map/Map.h"


/// <summary>
/// ゲームマネージャーを生成
/// </summary>
/// <param name="resources">共通リソース</param>
/// <returns></returns>
std::unique_ptr<INonObject> ManagerFactory::CreateGameManager(CommonResources* resources, Map* map)
{
   //ゲームマネージャーを宣言する
   std::unique_ptr<INonObject> gameManager;
   //インスタンスを生成する
   gameManager.reset(new GameManager(resources, map));
   //初期化する
   gameManager->Initialize();
   //Spaceキーを登録する
   Messenger::Attach(DirectX::Keyboard::Keyboard::Space, gameManager.get());

   //インスタンスを返す
   return std::move(gameManager);
}
