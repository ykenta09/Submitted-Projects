/*
	@file	MapObjectFactory.cpp
	@brief	マップオブジェクトのファクトリークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Factory/MapObjectFactory.h"
#include "Game/Map/Key/Key.h"
#include "Game/Map/Prison/Prison.h"
#include "Game/Map/Floor/Floor.h"
#include "Game/Map/Wall/Wall.h" 
#include "Game/Map/Goal/Goal.h"

//jsonファイルから読み込んだデータからオブジェクトを生成
std::unique_ptr<IMapObject> MapObjectFactory::CreateObject(const std::string& type, const nlohmann::json& obj)
{
   DirectX::SimpleMath::Vector3 pos = { obj["position"]["x"], obj["position"]["y"], obj["position"]["z"] };
   DirectX::SimpleMath::Quaternion rot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
	  DirectX::XMConvertToRadians(obj["rotation"]["y"]),
	  DirectX::XMConvertToRadians(obj["rotation"]["x"]),
	  DirectX::XMConvertToRadians(obj["rotation"]["z"])
	  );
   DirectX::SimpleMath::Vector3 scl = { obj["scale"]["x"], obj["scale"]["y"], obj["scale"]["z"] };

   if (type == "Prison")
   {
	  std::unique_ptr<IMapObject> prison;
	  prison.reset(new Prison(1, pos, rot, scl));
	  prison->Initialize();
	  return std::move(prison);
   }
   else if (type == "Key")
   {
	  std::unique_ptr<IMapObject> key;
	  key.reset(new Key(pos, rot, scl));
	  key->Initialize();
	  return std::move(key);
   }
   else if (type == "Wall01")
   {
	  std::unique_ptr<IMapObject> wall01;
	  wall01.reset(new Wall(3, pos, rot, scl));
	  wall01->Initialize();
	  return std::move(wall01);
   }
   else if (type == "Wall02")
   {
	  std::unique_ptr<IMapObject> wall02;
	  wall02.reset(new Wall(4, pos, rot, scl));
	  wall02->Initialize();
	  return std::move(wall02);
   }
   else if (type == "Floor01")
   {
	  std::unique_ptr<IMapObject> floor01;
	  floor01.reset(new Floor(1, pos, rot, scl));
	  floor01->Initialize();
	  return std::move(floor01);
   }
   else if (type == "Floor02")
   {
	  std::unique_ptr<IMapObject> floor02;
	  floor02.reset(new Floor(2, pos, rot, scl));
	  floor02->Initialize();
	  return std::move(floor02);
   }
   else if (type == "Floor03")
   {
	  std::unique_ptr<IMapObject> floor03;
	  floor03.reset(new Floor(3, pos, rot, scl));
	  floor03->Initialize();
	  return std::move(floor03);
   }

   else if (type == "Gate")
   {
	  std::unique_ptr<IMapObject> gate;
	  gate.reset(new Goal(pos, rot, scl));
	  gate->Initialize();
	  return std::move(gate);
   }
 
   return std::move(nullptr);
}

//csvファイルから読み込んだデータからオブジェクトを生成
std::unique_ptr<IMapObject> MapObjectFactory::CreateObject(const int& type, const int& modelNumber, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scl)
{
   DirectX::SimpleMath::Quaternion rot = DirectX::SimpleMath::Quaternion::Identity;

   if (type == 1)
   {
   }
   else if (type == 2) //壁オブジェクト
   {
	  std::unique_ptr<IMapObject> wall;
	  wall.reset(new Wall(modelNumber, pos, rot, scl));
	  wall->Initialize();
	  return std::move(wall);
   }
   return std::move(nullptr);
}
