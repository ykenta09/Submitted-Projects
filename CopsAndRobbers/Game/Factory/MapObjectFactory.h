/*
	@file	MapObjectFactory.h
	@brief	�}�b�v�I�u�W�F�N�g�t�@�N�g���[�N���X
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
   //json�t�@�C������ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�
   static std::unique_ptr<IMapObject> CreateObject(CommonResources* resources, const GameParameters::StageParamger& stageParam, const std::string& type, const nlohmann::json& obj);
   //csv�t�@�C������ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�
   static std::unique_ptr<IMapObject> CreateObject(const GameParameters::StageParamger& stageParam, const int& type, const int& modelNumber, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scl);

   //�S���𐶐�����
   static std::unique_ptr<IMapObject> CreatePrison(const GameParameters::ObjectParameter& param);
   //���𐶐�����
   static std::unique_ptr<IMapObject> CreateKey(const GameParameters::ObjectParameter& param);
   //�ǂ𐶐�����
   static std::unique_ptr<IMapObject> CreateWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //�����ǂ𐶐�����
   static std::unique_ptr<IMapObject> CreateMovingWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos);
   //���𐶐�����
   static std::unique_ptr<IMapObject> CreateFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //�������𐶐�����
   static std::unique_ptr<IMapObject> CreateMovingFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos);
   //��]���鏰�𐶐�����
   static std::unique_ptr<IMapObject> CreateRotationFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //���o�[���u�𐶐�����
   static std::unique_ptr<IMapObject> CreateLeverDevice(const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //�Q�[�g�𐶐�����
   static std::unique_ptr<IMapObject> CreateGate(const GameParameters::ObjectParameter& param);
   //�h�A�𐶐�����
   static std::unique_ptr<IMapObject> CreateDoor(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj);
   //���C�g�𐶐�����
   static std::unique_ptr<IMapObject> CreateLight(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj);

};
#endif // MAPOBJECT_FACTORY_DEFINED

