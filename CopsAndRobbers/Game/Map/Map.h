/*
	@file	Map.h
	@brief	�}�b�v�N���X
*/
#pragma once
#ifndef MAP_DEFINED
#define MAP_DEFINED

#include "Game/Interface/IMapObject.h"
#include "Game/Map/Key/Key.h"
#include "Game/Map/Prison/Prison.h"
#include "Game/Map/Door/Door.h"
#include "Libraries/yamadaLib/LoadCSV.h"

#include "Game/Map/Goal/Goal.h"
#include "Game/Map/CelestialSphere/CelestialSphere.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <Libraries/yamadaLib/LoadJson.h>
#include "Libraries/yamadaLib/GameParameter.h"
#include  "Game/FrustumCuller.h"


#include "Game/Render/CustomModelRenderer.h"

// �O���錾
class CommonResources;
class IMapObject;
class GimmickObjectBase;
class DeviceObjectBase;

class Map
{
public:
	//�o�E���f�B���O�{�b�N�X�z����擾����
	const std::vector<DirectX::BoundingBox>& GetBoundingBoxes() const { return m_allMapBoundingBoxes; }
	//�w��X�e�[�W�̋��E�{�b�N�X���擾����
	std::vector<DirectX::BoundingBox> GetStageBoundingBoxes(int stageNumber) const;
	//���E���z����擾����
	const std::vector<DirectX::BoundingSphere>& GetBoundingSphere() const { return m_allMapBoundingSpheres; }
	const std::unordered_map<int, std::vector<std::unique_ptr<IMapObject>>>& GetMapObjects() const { return m_stageObjects; }
	//csv�}�b�v�f�[�^���擾����
	const std::vector<std::vector<GameParameters::CreateObjectParameter>> GetMapData(int stageNumber) { return m_map[stageNumber]; }
	//�w�肵���X�e�[�W�̒��S���W���擾����
	const DirectX::SimpleMath::Vector3& GetStageCenterPosition(int stageNumber) { return m_stagePositions[stageNumber]; }
	//�ڑ��m�[�h�z����擾����
	const std::vector<GameParameters::StageTransition> GetStageTransition() { return m_stageTransition; }

	//�M�~�b�N�I�u�W�F�N�g�z����擾����
	std::vector<GimmickObjectBase*> GetGimmickObjects() const { return m_gimmickObjects; }
	//�f�o�C�X�I�u�W�F�N�g�z����擾����
	std::vector<DeviceObjectBase*> GetDeviceObjects() const { return m_deviceObjects; }

	//�V���̒��S���W��ݒ肷��
	void SetCelestialSpherePosition(const DirectX::SimpleMath::Vector3& pos) { m_celestialSpherePosition = pos; }

	void ChangeStageNumber(const int& stageNumber) { m_stageNumber = stageNumber; }

	//�f�o�b�N
	int GetDrawObject() { return m_drawObject; }

	// ����X�e�[�W�̃I�u�W�F�N�g��const�Q�ƂŎ擾
	const std::vector<std::unique_ptr<IMapObject>>& GetStageObjects(int stageID) const 
	{
	   static const std::vector<std::unique_ptr<IMapObject>> empty; // ������Ȃ��ꍇ�p

	   if (stageID < 1) return empty;
	   auto it = m_stageObjects.find(stageID);
	   if (it != m_stageObjects.end())
	   {
		  return it->second;
	   }
	   return empty;
	}

	// �w�肵���^�C�v�̃I�u�W�F�N�g���擾����e���v���[�g���\�b�h
   // ����X�e�[�W�̂ݑΏۂƂ���o�[�W����
	template<typename T>
	std::vector<T*> GetObjectsByType(int stageID)
	{
	   if (stageID < 1) return {};
	   auto it = m_stageObjects.find(stageID);
	   if (it != m_stageObjects.end())
	   {
		  std::vector<T*> result;
		  for (const auto& obj : it->second)
		  {
			 if (obj->GetType() == T::StaticType()) 
			 {
				result.push_back(dynamic_cast<T*>(obj.get()));
			 }
		  }
		  return result;
	   }
	   return {};
	}

	// �w�肵���^�C�v�̃I�u�W�F�N�g���擾����e���v���[�g���\�b�h
   // ���ʃI�u�W�F�N�g����擾����
	template<typename T>
	std::vector<T*> GetObjectsByType()
	{

	  std::vector<T*> result;
	  for (const auto& obj : m_commonObjects)
	  {
		 if (obj->GetType() == T::StaticType())
		 {
			result.push_back(static_cast<T*>(obj.get()));
		 }
	  }
	  return result;
	}

public:
    //�R���X�g���N�^
	Map(CommonResources* resources);
	//�f�X�g���N�^
	~Map();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float elapsedTime);
	//�`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	//��n��������
	void Finalize();

	//�X�e�[�W�̃f�[�^���擾����
	void LoadStageData();

	//JSON�t�@�C������I�u�W�F�N�g����
	std::vector<std::unique_ptr<IMapObject>> LoadObjectsFromJson(int stageNumber, GameParameters::StageParamger& parameter);
	//CSV�t�@�C������I�u�W�F�N�g����
	std::vector<std::unique_ptr<IMapObject>> LoadObjectsFromCsv(int stageNumber, GameParameters::StageParamger& parameter);
	//�����蔻���ݒ肷��
	void SetCollider();
	//�N���A
	void Clear();
private:
   //�M�~�b�N�I�u�W�F�N�g�z����쐬
   void RegisterGimmickObjects();
   //�f�o�C�X�I�u�W�F�N�g�z����쐬
   void RegisterDeviceObjects();
private:
	CommonResources* m_commonResources;	// ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;	//�O���t�B�b�N�X

	//�}�b�v�I�u�W�F�N�g
	std::unique_ptr<CelestialSphere> m_celestialSphere;	 //�V��
	std::unique_ptr<LoadJson> m_loadJson;	 //Json�t�@�C�����[�h
	std::unique_ptr<LoadCSV> m_loadCsv;		 //csv�t�@�C�����[�h
	std::unique_ptr<FrustumCuller> m_frustum;	//������J�����O

	//�}�b�v�z��
	std::vector<std::unique_ptr<IMapObject>> m_mapObject;
	//csv�̃}�b�v�f�[�^�z��
	std::unordered_map<int, std::vector<std::vector<GameParameters::CreateObjectParameter>>> m_map;
	//�ڑ��m�[�h�z��
	std::vector<GameParameters::StageTransition> m_stageTransition;

	//�S�X�e�[�W�f�[�^��ێ�
	std::vector<IMapObject*> m_allObjects;													 //�S�Ẵ}�b�v�I�u�W�F�N�g
	std::unordered_map<int, std::vector<std::unique_ptr<IMapObject>>> m_stageObjects;		 //�X�e�[�W�I�u�W�F�N�g
	std::vector<std::unique_ptr<IMapObject>> m_commonObjects;								 // ���ʃI�u�W�F�N�g�i�h�A�Ȃǁj
	std::vector<GimmickObjectBase*> m_gimmickObjects;										 //�M�~�b�N�I�u�W�F�N�g
	std::vector<DeviceObjectBase*> m_deviceObjects;											 //�f�o�C�X�I�u�W�F�N�g
	std::vector<IMapObject*> m_shadowObjects;												 //�|�C���g���C�g���Ղ�I�u�W�F�N�g
	std::unordered_map<int, DirectX::SimpleMath::Vector3> m_stagePositions;					 //�e�X�e�[�W�̒��S���W
	DirectX::SimpleMath::Vector3 m_celestialSpherePosition;									 //�V���̒��S���W

	//�S�Ẵ}�b�v�I�u�W�F�N�g�̋��E�{�b�N�X�z��
	std::vector<DirectX::BoundingBox> m_allMapBoundingBoxes;
	std::vector<DirectX::BoundingSphere> m_allMapBoundingSpheres;
	int m_stageNumber;	//�X�e�[�W�ԍ�
		 
	//�L���[�u�}�b�v�ւ̏����o���t���O
	bool m_isDrawCubeMap;
	//�f�o�b�N�ϐ�
	int m_drawObject;		  //�`�悵�Ă���I�u�W�F�N�g�̐����擾����
};
#endif // MAP_DEFINED
