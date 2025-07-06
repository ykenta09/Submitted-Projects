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

#include <nlohmann/json.hpp>

class MapObjectFactory 
{
public:
   static std::unique_ptr<IMapObject> CreateObject(const std::string& type, const nlohmann::json& obj);
   static std::unique_ptr<IMapObject> CreateObject(const int& type, const int& modelNumber, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scl);
};
#endif // MAPOBJECT_FACTORY_DEFINED

