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
#include "Game/Map/Floor/MovingFloor.h"
#include "Game/Map/Floor/RotationFloor.h"
#include "Game/Map/Device/LeverDevice.h"
#include "Game/Map/Wall/Wall.h" 
#include "Game/Map/Wall/MovingWall.h"
#include "Game/Map/Goal/Goal.h"
#include "Game/Map/Door/Door.h"
#include "Game/Map/Light/Light.h"
#include "Libraries/yamadaLib/LoadJson.h"
#include <Game/Observer/Messenger.h>

/// <summary>
/// jsonファイルから読み込んだデータからオブジェクトを生成
/// </summary>
/// <param name="stageParam">ステージのパラメータ</param>
/// <param name="type">オブジェクトのタイプ</param>
/// <param name="obj">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateObject(CommonResources* resources, const GameParameters::StageParamger& stageParam, const std::string& type, const nlohmann::json& obj)
{
   //オブジェクトのパラメータ設定
   GameParameters::ObjectParameter param;
   param.initialPos = { obj["position"]["x"], obj["position"]["y"], obj["position"]["z"] };
   param.initialPos += stageParam.stagePos;
   param.initialRot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
	  DirectX::XMConvertToRadians(obj["rotation"]["y"]),
	  DirectX::XMConvertToRadians(obj["rotation"]["x"]),
	  DirectX::XMConvertToRadians(obj["rotation"]["z"])
	  );
   param.scale = { obj["scale"]["x"], obj["scale"]["y"], obj["scale"]["z"] };
   param.stageParam = stageParam;

   //オブジェクトのタイプ事にインスタンスを生成
   if (type == "Prison")			   //牢屋を生成
	  return CreatePrison(param);
   else if (type == "Key")			   //鍵を生成
	  return CreateKey(param);
   else if (type == "Wall")			   //壁を生成
	  return CreateWall(param, obj);
   else if (type == "MovingWall")	   //動く壁を生成
	  return CreateMovingWall(param, obj, stageParam.stagePos);
   else if (type == "Floor")		   //床を生成
	  return CreateFloor(param, obj);
   else if (type == "MovingFloor")	   //動く床を生成
	  return CreateMovingFloor(param, obj, stageParam.stagePos);
   else if (type == "RotationFloor")   //回転する床を生成
	  return CreateRotationFloor(param, obj);
   else if (type == "Lever")		   //ギミック起動装置を生成
	  return CreateLeverDevice(param, obj);
   else if (type == "Gate")			   //ゴールゲートを生成
	  return CreateGate(param);
   else if (type == "Door")			   //ステージ間の移動ドアを生成
	  return CreateDoor(resources, param, obj);
   else if (type == "Light")		   //ライトを生成
	  return CreateLight(resources, param, obj);
 
   return std::move(nullptr);
}

/// <summary>
/// csvファイルから読み込んだデータからオブジェクトを生成
/// </summary>
/// <param name="stageParam">ステージのパラメータ</param>
/// <param name="type">オブジェクトのタイプ</param>
/// <param name="modelNumber">モデルの番号</param>
/// <param name="pos">配置座標</param>
/// <param name="scl">スケール</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateObject(const GameParameters::StageParamger& stageParam, const int& type, const int& modelNumber, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scl)
{
   //オブジェクトのパラメータ設定
   GameParameters::ObjectParameter param;
   param.initialPos = pos + stageParam.stagePos;
   param.initialRot = DirectX::SimpleMath::Quaternion::Identity;
   param.scale = scl;
   param.stageParam = stageParam;

   //オブジェクトのタイプ事にインスタンスを生成
   if (type == 1) //床オブジェクト
   {
	  std::unique_ptr<IMapObject> floor;
	  floor.reset(new Floor(modelNumber, param));
	  floor->Initialize();
	  return std::move(floor);
   }
   if (type == 2) //壁オブジェクト
   {
	  std::unique_ptr<IMapObject> wall;
	  wall.reset(new Wall(modelNumber, param));
	  wall->Initialize();
	  return std::move(wall);
   }
   return std::move(nullptr);
}


/// <summary>
/// 牢屋を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreatePrison(const GameParameters::ObjectParameter& param)
{
   //牢屋を宣言
   std::unique_ptr<IMapObject> prison;
   //インスタンスを生成
   prison.reset(new Prison(1, param));
   //初期化する
   prison->Initialize();
   //インスタンスを返す
   return  std::move(prison);
}

/// <summary>
/// 鍵を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateKey(const GameParameters::ObjectParameter& param)
{
   //鍵を宣言
   std::unique_ptr<IMapObject> key;
   //インスタンスを生成
   key.reset(new Key(param));
   //初期化する
   key->Initialize();
   //インスタンスを生成
   return std::move(key);
}

/// <summary>
/// 壁を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //モデルのタイプを取得
   int modelType = obj.value("modelType", 1);
   //壁を宣言
   std::unique_ptr<IMapObject> wall;
   //インスタンスを生成
   wall.reset(new Wall(modelType, param));
   //初期化する
   wall->Initialize();
   //インスタンスを返す
   return std::move(wall);
}

/// <summary>
/// 動く壁を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <param name="stagePos">ステージの中心座標</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateMovingWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos)
{
   //モデルのタイプとギミックIDを取得する
   int modelType = obj.value("modelType", 1);
   int gimmickID = obj.value("gimmickID", 1);
   //移動先の座標を取得する
   DirectX::SimpleMath::Vector3 movePos = { obj["movePosition"]["x"], obj["movePosition"]["y"], obj["movePosition"]["z"] };
   movePos += stagePos;
   //動く壁を宣言
   std::unique_ptr<IMapObject> movingWall;
   //インスタンスを生成
   movingWall.reset(new MovingWall(modelType, param, movePos, gimmickID));
   //初期化する
   movingWall->Initialize();
   //インスタンスを返す
   return std::move(movingWall);
}

/// <summary>
/// 床を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //モデルのタイプを取得する
   int modelType = obj.value("modelType", 1);
   //床を宣言
   std::unique_ptr<IMapObject> floor;
   //インスタンスを生成
   floor.reset(new Floor(modelType, param));
   //初期化する
   floor->Initialize();
   //インスタンスを返す
   return std::move(floor);
}

/// <summary>
/// 動く床を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <param name="stagePos">ステージの中心座標</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateMovingFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos)
{
   //モデルのタイプとギミックIDを取得する
   int modelType = obj.value("modelType", 1);
   int gimmickID = obj.value("gimmickID", 1);
   //移動先の座標を取得する
   DirectX::SimpleMath::Vector3 movePos = { obj["movePosition"]["x"], obj["movePosition"]["y"], obj["movePosition"]["z"] };
   movePos += stagePos;
   //動く床を宣言
   std::unique_ptr<IMapObject> movingFloor;
   //インスタンスを生成
   movingFloor.reset(new MovingFloor(modelType, param, movePos, gimmickID));
   //初期化する
   movingFloor->Initialize();
   //インスタンスを返す
   return std::move(movingFloor);
}

/// <summary>
/// 回転する床
/// </summary>
/// <param name="param">オブジェクトパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <param name="stagePos">ステージの中心座標</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateRotationFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //モデルのタイプとギミックIDを取得する
   int modelType = obj.value("modelType", 1);
   int gimmickID = obj.value("gimmickID", 1);
   //動く床を宣言
   std::unique_ptr<IMapObject> rotationFloor;
   //インスタンスを生成
   rotationFloor.reset(new RotationFloor(modelType, param, gimmickID));
   //初期化する
   rotationFloor->Initialize();
   //インスタンスを返す
   return std::move(rotationFloor);
}

/// <summary>
/// レバー装置を生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateLeverDevice(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //レバーIDを取得する
   int leverID = obj.value("leverID", 1);
   //レバーを宣言
   std::unique_ptr<DeviceObjectBase> lever;
   //インスタンスを生成
   lever.reset(new LeverDevice(param, leverID));
   //初期化する
   lever->Initialize();
   //装置起動キーを登録する
   Messenger::Attach(DirectX::Keyboard::Keyboard::E, lever.get());

   //インスタンスを返す
   return std::move(lever);
}


/// <summary>
/// ゲートを生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateGate(const GameParameters::ObjectParameter& param)
{
   //ゲートを宣言
   std::unique_ptr<IMapObject> gate;
   //インスタンスを生成
   gate.reset(new Goal(param));
   //初期化する
   gate->Initialize();
   //インスタンスを返す
   return std::move(gate);
}

/// <summary>
/// ドアを生成する
/// </summary>
/// <param name="param">オブジェクトのパラメータ</param>
/// <param name="obj">jsonデータ</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateDoor(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //移動先のステージ番号と元のステージ番号を取得する
   int nextStage = obj.value("nextStage", 1);
   int backStage = obj.value("backStage", 1);
   //ドアを宣言
   std::unique_ptr<IMapObject> door;
   //インスタンスを生成
   door.reset(new Door(resources, param, nextStage, backStage));
   //初期化する
   door->Initialize();
   //インスタンスを返す
   return std::move(door);
}

/// <summary>
/// ライトを生成する
/// </summary>
/// <param name="resources"></param>
/// <param name="param"></param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateLight(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //ライトのカラーを取得する
   DirectX::SimpleMath::Vector3 color = { obj["color"]["r"], obj["color"]["g"], obj["color"]["b"] };
   //ライトの影響範囲
   float range = static_cast<float>(obj.value("range", 1));
   //ライトモデルの有無
   bool modelDraw = obj.value("model", 1);
   //ライトを宣言
   std::unique_ptr<IMapObject> light;
   //インスタンスを生成
   light.reset(new Light(resources, param, color, range, modelDraw));
   //初期化する
   light->Initialize();
   //インスタンスを返す
   return std::move(light);
}
