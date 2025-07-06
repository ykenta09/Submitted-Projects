/*
	@file	MapObjectFactory.cpp
	@brief	�}�b�v�I�u�W�F�N�g�̃t�@�N�g���[�N���X
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
/// json�t�@�C������ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�
/// </summary>
/// <param name="stageParam">�X�e�[�W�̃p�����[�^</param>
/// <param name="type">�I�u�W�F�N�g�̃^�C�v</param>
/// <param name="obj">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateObject(CommonResources* resources, const GameParameters::StageParamger& stageParam, const std::string& type, const nlohmann::json& obj)
{
   //�I�u�W�F�N�g�̃p�����[�^�ݒ�
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

   //�I�u�W�F�N�g�̃^�C�v���ɃC���X�^���X�𐶐�
   if (type == "Prison")			   //�S���𐶐�
	  return CreatePrison(param);
   else if (type == "Key")			   //���𐶐�
	  return CreateKey(param);
   else if (type == "Wall")			   //�ǂ𐶐�
	  return CreateWall(param, obj);
   else if (type == "MovingWall")	   //�����ǂ𐶐�
	  return CreateMovingWall(param, obj, stageParam.stagePos);
   else if (type == "Floor")		   //���𐶐�
	  return CreateFloor(param, obj);
   else if (type == "MovingFloor")	   //�������𐶐�
	  return CreateMovingFloor(param, obj, stageParam.stagePos);
   else if (type == "RotationFloor")   //��]���鏰�𐶐�
	  return CreateRotationFloor(param, obj);
   else if (type == "Lever")		   //�M�~�b�N�N�����u�𐶐�
	  return CreateLeverDevice(param, obj);
   else if (type == "Gate")			   //�S�[���Q�[�g�𐶐�
	  return CreateGate(param);
   else if (type == "Door")			   //�X�e�[�W�Ԃ̈ړ��h�A�𐶐�
	  return CreateDoor(resources, param, obj);
   else if (type == "Light")		   //���C�g�𐶐�
	  return CreateLight(resources, param, obj);
 
   return std::move(nullptr);
}

/// <summary>
/// csv�t�@�C������ǂݍ��񂾃f�[�^����I�u�W�F�N�g�𐶐�
/// </summary>
/// <param name="stageParam">�X�e�[�W�̃p�����[�^</param>
/// <param name="type">�I�u�W�F�N�g�̃^�C�v</param>
/// <param name="modelNumber">���f���̔ԍ�</param>
/// <param name="pos">�z�u���W</param>
/// <param name="scl">�X�P�[��</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateObject(const GameParameters::StageParamger& stageParam, const int& type, const int& modelNumber, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 scl)
{
   //�I�u�W�F�N�g�̃p�����[�^�ݒ�
   GameParameters::ObjectParameter param;
   param.initialPos = pos + stageParam.stagePos;
   param.initialRot = DirectX::SimpleMath::Quaternion::Identity;
   param.scale = scl;
   param.stageParam = stageParam;

   //�I�u�W�F�N�g�̃^�C�v���ɃC���X�^���X�𐶐�
   if (type == 1) //���I�u�W�F�N�g
   {
	  std::unique_ptr<IMapObject> floor;
	  floor.reset(new Floor(modelNumber, param));
	  floor->Initialize();
	  return std::move(floor);
   }
   if (type == 2) //�ǃI�u�W�F�N�g
   {
	  std::unique_ptr<IMapObject> wall;
	  wall.reset(new Wall(modelNumber, param));
	  wall->Initialize();
	  return std::move(wall);
   }
   return std::move(nullptr);
}


/// <summary>
/// �S���𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreatePrison(const GameParameters::ObjectParameter& param)
{
   //�S����錾
   std::unique_ptr<IMapObject> prison;
   //�C���X�^���X�𐶐�
   prison.reset(new Prison(1, param));
   //����������
   prison->Initialize();
   //�C���X�^���X��Ԃ�
   return  std::move(prison);
}

/// <summary>
/// ���𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateKey(const GameParameters::ObjectParameter& param)
{
   //����錾
   std::unique_ptr<IMapObject> key;
   //�C���X�^���X�𐶐�
   key.reset(new Key(param));
   //����������
   key->Initialize();
   //�C���X�^���X�𐶐�
   return std::move(key);
}

/// <summary>
/// �ǂ𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //���f���̃^�C�v���擾
   int modelType = obj.value("modelType", 1);
   //�ǂ�錾
   std::unique_ptr<IMapObject> wall;
   //�C���X�^���X�𐶐�
   wall.reset(new Wall(modelType, param));
   //����������
   wall->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(wall);
}

/// <summary>
/// �����ǂ𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <param name="stagePos">�X�e�[�W�̒��S���W</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateMovingWall(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos)
{
   //���f���̃^�C�v�ƃM�~�b�NID���擾����
   int modelType = obj.value("modelType", 1);
   int gimmickID = obj.value("gimmickID", 1);
   //�ړ���̍��W���擾����
   DirectX::SimpleMath::Vector3 movePos = { obj["movePosition"]["x"], obj["movePosition"]["y"], obj["movePosition"]["z"] };
   movePos += stagePos;
   //�����ǂ�錾
   std::unique_ptr<IMapObject> movingWall;
   //�C���X�^���X�𐶐�
   movingWall.reset(new MovingWall(modelType, param, movePos, gimmickID));
   //����������
   movingWall->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(movingWall);
}

/// <summary>
/// ���𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //���f���̃^�C�v���擾����
   int modelType = obj.value("modelType", 1);
   //����錾
   std::unique_ptr<IMapObject> floor;
   //�C���X�^���X�𐶐�
   floor.reset(new Floor(modelType, param));
   //����������
   floor->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(floor);
}

/// <summary>
/// �������𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <param name="stagePos">�X�e�[�W�̒��S���W</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateMovingFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj, DirectX::SimpleMath::Vector3 stagePos)
{
   //���f���̃^�C�v�ƃM�~�b�NID���擾����
   int modelType = obj.value("modelType", 1);
   int gimmickID = obj.value("gimmickID", 1);
   //�ړ���̍��W���擾����
   DirectX::SimpleMath::Vector3 movePos = { obj["movePosition"]["x"], obj["movePosition"]["y"], obj["movePosition"]["z"] };
   movePos += stagePos;
   //��������錾
   std::unique_ptr<IMapObject> movingFloor;
   //�C���X�^���X�𐶐�
   movingFloor.reset(new MovingFloor(modelType, param, movePos, gimmickID));
   //����������
   movingFloor->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(movingFloor);
}

/// <summary>
/// ��]���鏰
/// </summary>
/// <param name="param">�I�u�W�F�N�g�p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <param name="stagePos">�X�e�[�W�̒��S���W</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateRotationFloor(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //���f���̃^�C�v�ƃM�~�b�NID���擾����
   int modelType = obj.value("modelType", 1);
   int gimmickID = obj.value("gimmickID", 1);
   //��������錾
   std::unique_ptr<IMapObject> rotationFloor;
   //�C���X�^���X�𐶐�
   rotationFloor.reset(new RotationFloor(modelType, param, gimmickID));
   //����������
   rotationFloor->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(rotationFloor);
}

/// <summary>
/// ���o�[���u�𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateLeverDevice(const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //���o�[ID���擾����
   int leverID = obj.value("leverID", 1);
   //���o�[��錾
   std::unique_ptr<DeviceObjectBase> lever;
   //�C���X�^���X�𐶐�
   lever.reset(new LeverDevice(param, leverID));
   //����������
   lever->Initialize();
   //���u�N���L�[��o�^����
   Messenger::Attach(DirectX::Keyboard::Keyboard::E, lever.get());

   //�C���X�^���X��Ԃ�
   return std::move(lever);
}


/// <summary>
/// �Q�[�g�𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateGate(const GameParameters::ObjectParameter& param)
{
   //�Q�[�g��錾
   std::unique_ptr<IMapObject> gate;
   //�C���X�^���X�𐶐�
   gate.reset(new Goal(param));
   //����������
   gate->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(gate);
}

/// <summary>
/// �h�A�𐶐�����
/// </summary>
/// <param name="param">�I�u�W�F�N�g�̃p�����[�^</param>
/// <param name="obj">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateDoor(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //�ړ���̃X�e�[�W�ԍ��ƌ��̃X�e�[�W�ԍ����擾����
   int nextStage = obj.value("nextStage", 1);
   int backStage = obj.value("backStage", 1);
   //�h�A��錾
   std::unique_ptr<IMapObject> door;
   //�C���X�^���X�𐶐�
   door.reset(new Door(resources, param, nextStage, backStage));
   //����������
   door->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(door);
}

/// <summary>
/// ���C�g�𐶐�����
/// </summary>
/// <param name="resources"></param>
/// <param name="param"></param>
/// <returns></returns>
std::unique_ptr<IMapObject> MapObjectFactory::CreateLight(CommonResources* resources, const GameParameters::ObjectParameter& param, const nlohmann::json& obj)
{
   //���C�g�̃J���[���擾����
   DirectX::SimpleMath::Vector3 color = { obj["color"]["r"], obj["color"]["g"], obj["color"]["b"] };
   //���C�g�̉e���͈�
   float range = static_cast<float>(obj.value("range", 1));
   //���C�g���f���̗L��
   bool modelDraw = obj.value("model", 1);
   //���C�g��錾
   std::unique_ptr<IMapObject> light;
   //�C���X�^���X�𐶐�
   light.reset(new Light(resources, param, color, range, modelDraw));
   //����������
   light->Initialize();
   //�C���X�^���X��Ԃ�
   return std::move(light);
}
