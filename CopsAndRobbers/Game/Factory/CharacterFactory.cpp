/*
	@file	CharacterFactory.cpp
	@brief	キャラクターのファクトリークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Observer/Messenger.h"
#include "Game/Factory/CharacterFactory.h"

#include "Game/Character/Player/Player.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Map/Map.h"


/// <summary>
/// キャラクターを生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="map">マップのポインタ</param>
/// <param name="data">キャラクターのデータ</param>
/// <param name="stageNumber">ステージ番号</param>
/// <param name="json">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreateCharacter(IComponent* parent, CommonResources* resources, Map* map, const GameParameters::CharacterData data, const int& stageNumber, LoadJson* json)
{
   //データの種類ごとにキャラクターの生成を行う
   if (data.name == "Player")
   {
      //プレイヤーの生成
	  return CreatePlayer(parent, resources, data, stageNumber);
   }
   else if (data.name == "Fellow")
   {
	  //仲間の生成
	  return CreateFellow(parent, resources, map, data, stageNumber);
   }
   else if (data.name == "Enemy")
   {
	  //敵の生成
	  return CreateEnemy(parent, resources, 0, map, data, stageNumber, json);
   }

   return std::unique_ptr<IComponent>();
}

/// <summary>
/// プレイヤーを生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="data">キャラクターのデータ</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreatePlayer(IComponent* parent, CommonResources* resources, const GameParameters::CharacterData data, const int& stageNumber)
{
	std::unique_ptr<IComponent> player;
	//インスタンスを生成する
	player.reset(new Player(parent, resources, data, stageNumber));
	//初期化する
	player->Initialize();
	//キーをアタッチする
	//前進を登録
	Messenger::Attach(DirectX::Keyboard::Keyboard::W, player.get());
	//後進を登録
	Messenger::Attach(DirectX::Keyboard::Keyboard::S, player.get());
	//左移動を登録
	Messenger::Attach(DirectX::Keyboard::Keyboard::A, player.get());
	//右移動を登録
	Messenger::Attach(DirectX::Keyboard::Keyboard::D, player.get());
	//高速移動を登録
	Messenger::Attach(DirectX::Keyboard::Keyboard::LeftShift, player.get());
	
	//インスタンスを返す
	return std::move(player);
}

/// <summary>
/// 仲間を生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="map">マップのポインタ</param>
/// <param name="data">キャラクターのデータ</param>
/// <param name="stageNumber">ステージ番号</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreateFellow(IComponent* parent, CommonResources* resources, Map* map, const GameParameters::CharacterData data, const int& stageNumber)
{
	std::unique_ptr<IComponent> fellow;
	//インスタンスを生成する
	fellow.reset(new Fellow(parent, resources, map, data, stageNumber));
	//初期化する
	fellow->Initialize();
	//インスタンスを返す
	return std::move(fellow);
}

/// <summary>
/// 敵を生成する
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="number">敵の番号</param>
/// <param name="map">マップのポインタ</param>
/// <param name="data">キャラクターのデータ</param>
/// <param name="stageNumber">ステージ番号</param>
/// <param name="json">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreateEnemy(IComponent* parent, CommonResources* resources, const int& number, Map* map, const GameParameters::CharacterData data, const int& stageNumber, LoadJson* json)
{
	std::unique_ptr<IComponent> enemy;
	//敵のデータを設定する
	GameParameters::EnemyData enemyData;
	enemyData.charaData = data;									  //キャラクタ―データ
	enemyData.enemyNumber = number;								  //敵番号
    enemyData.patrolRouteMap = json->GetPatrolRootMap();		  //巡回ルートマップ
    enemyData.patrolAdjacencyList = json->GetAdjacencyList();	  //隣接リスト
    enemyData.initalNumber = json->GetInitailVertex(number);	  //初期座標の頂点
	//インスタンスを生成する
	enemy.reset(new Enemy(parent, resources, map, enemyData, stageNumber));
	//初期化する
	enemy->Initialize();
	//インスタンスを返す
	return std::move(enemy);
}
