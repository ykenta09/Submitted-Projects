/*
	@file	MapObjectFactory.h
	@brief	マップオブジェクトファクトリークラス
*/
#pragma once
#ifndef MAPOBJECT_FACTORY_DEFINED
#define MAPOBJECT_FACTORY_DEFINED
#include <memory>
#include <string>
#include "Game/Interface/IMapObject.h"
#include "Libraries/yamadaLib/GameParameter.h"

#include <nlohmann/json.hpp>

class LoadJson;

class MapObjectFactory 
{
public:
   //jsonファイルから読み込んだデータからオブジェクトを生成
   static std::unique_ptr<IMapObject> CreateObject(CommonResources* resources, const GameParameters::StageParamger& stageParam, const std::string& type, const nlohmann::json& obj);
   //csvファイルから読み込んだデータからオブジェクトを生成
   static std::unique_ptr<IMapObject> CreateObject(const GameParameters::StageParamger& stageParam, const int& type, const int& modelNumber, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scl);

   //牢屋を生成する
   static std::unique_ptr<IMapObject> CreatePrison(const GameParameters::ObjectParameter& param);
   //鍵を生成する
   static std::unique_ptr<IMapObject> CreateKey(const GameParameters::ObjectParameter& param);
   //壁を生成する
   static std::unique_ptr<IMapObject> CreateWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //動く壁を生成する
   static std::unique_ptr<IMapObject> CreateMovingWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos);
   //床を生成する
   static std::unique_ptr<IMapObject> CreateFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //動く床を生成する
   static std::unique_ptr<IMapObject> CreateMovingFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos);
   //回転する床を生成する
   static std::unique_ptr<IMapObject> CreateRotationFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //レバー装置を生成する
   static std::unique_ptr<IMapObject> CreateLeverDevice(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //ゲートを生成する
   static std::unique_ptr<IMapObject> CreateGate(const GameParameters::ObjectParameter& param);
   //ドアを生成する
   static std::unique_ptr<IMapObject> CreateDoor(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //ライトを生成する
   static std::unique_ptr<IMapObject> CreateLight(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj);

};
#endif // MAPOBJECT_FACTORY_DEFINED

