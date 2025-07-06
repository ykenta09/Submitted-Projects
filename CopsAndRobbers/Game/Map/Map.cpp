/*
	@file	Map.cpp
	@brief	�}�b�v�N���X
*/
#include "pch.h"
#include "Game/Map/Map.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/CelestialSphere/CelestialSphere.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/MyLib/BlackBoard.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Game/Map/Base/DeviceObjectBase.h"
#include <fstream>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
Map::Map(CommonResources* resources)
   :
   m_commonResources(resources),
   m_isDrawCubeMap(false),
   m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Map::~Map()
{
}

/// <summary>
/// ����������
/// </summary>
void Map::Initialize()
{
	//�V���𐶐��A����������
	m_celestialSphere = std::make_unique<CelestialSphere>(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Quaternion::Identity);
	m_celestialSphere->Initialize();

	//������𐶐�
	m_frustum = std::make_unique<FrustumCuller>();

	//�X�e�[�W�f�[�^��ǂݍ���
	LoadStageData();
	//���E�{�b�N�X�𐶐�����
	SetCollider();
	//�M�~�b�N�I�u�W�F�N�g�̔z����쐬
	RegisterGimmickObjects();
	//�f�o�C�X�I�u�W�F�N�g�̔z����쐬
	RegisterDeviceObjects();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void Map::Update(const float elapsedTime)
{
	//�V�����X�V����
	m_celestialSphere->Update(m_celestialSpherePosition);
	
	//�I�u�W�F�N�g�̍X�V����
	for (const auto& object : m_allObjects)
	{
	   object->Update(elapsedTime);
	}

	//���E�{�b�N�X���X�V����
	SetCollider();
}

/// <summary>
/// �`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void Map::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
   //�f�o�b�N�p(�`��I�u�W�F�N�g�̐���ۑ�)
   m_drawObject = 0;

   //�V����`�悷��
   m_celestialSphere->Render();
   
   //��������쐬
   DirectX::BoundingFrustum frustum = m_frustum->CreateViewFrustum(view, proj);

   //�}�b�v�I�u�W�F�N�g��������J�����O���ĕ`�悷��
   for (const auto& object : m_allObjects)
   {
	  //�R���W�����I�u�W�F�N�g���擾
	  CollisionObject* collisionObj = object->GetCollisionObject();
	  if (collisionObj)
	  {
		 //�{�b�N�X�Ǝ�����Ŕ�����s���`��
		 const auto box = collisionObj->GetCollisionParameter().collision.box;
		 if (m_frustum->IsVisible(frustum, box))
		 {
			m_drawObject++;
			object->Render();
		 }
	  }
	  else //�R���W�����I�u�W�F�N�g���Ȃ����̂�`��
	  {
		 m_drawObject++;
		 object->Render();
	  }
   }

#ifdef _DEBUG
      //�f�o�b�N�i���E�{�b�N�X��`��j
	  m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
	 [this](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
	 {
		 for (const auto& object : m_allObjects)
		 {
			//�Փ˔���̃p�����[�^���擾����
			CollisionObject* collisionObj = object->GetCollisionObject();
			if (collisionObj)
			{
			   const auto& param = collisionObj->GetCollisionParameter();
			   //if (param.collision.collisionType < CollisionManager::CollisionType::PUSH) continue;
			   if (!param.collisionFlag) continue;
			   //m_graphics->DrawBoundingSphere(param.collision.sphere);
			   //m_graphics->DrawBoundingBox(param.collision.box);
			}
		 }
	 });

#endif // _DEBUG
}

/// <summary>
/// ��n������
/// </summary>
void Map::Finalize()
{
	// do nothing.
}

/// <summary>
/// �I�u�W�F�N�g�f�[�^���擾����
/// </summary>
void Map::LoadStageData()
{
   //�X�e�[�W�P����ő�X�e�[�W���܂Ń}�b�v�I�u�W�F�N�g�̃f�[�^��ǂݍ��݁A�擾����
   for (int stageNumber = 1; stageNumber <= GameParameters::MAX_STAGE_NUM; stageNumber++)
   {
	  std::vector<std::unique_ptr<IMapObject>> stageObjects;

	  //�X�e�[�W�p�����[�^�̐ݒ�
	  GameParameters::StageParamger paramter;
	  //json�f�[�^����I�u�W�F�N�g�𐶐�
	  auto jsonObjects = LoadObjectsFromJson(stageNumber, paramter);
	  //csv�f�[�^����I�u�W�F�N�g�𐶐�
	  auto csvObjects = LoadObjectsFromCsv(stageNumber, paramter);

	  stageObjects.reserve(jsonObjects.size() + csvObjects.size()); // �z��̗e�ʂ��m��
	  std::move(jsonObjects.begin(), jsonObjects.end(), std::back_inserter(stageObjects));
	  std::move(csvObjects.begin(), csvObjects.end(), std::back_inserter(stageObjects));

	  // �X�e�[�W�ԍ����Ƃɕۑ�
	  m_stageObjects[stageNumber] = std::move(stageObjects);
   }
}


/// <summary>
/// JSON�t�@�C������I�u�W�F�N�g����
/// </summary>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <param name="parameter">�X�e�[�W�̃p�����[�^</param>
/// <returns></returns>
std::vector<std::unique_ptr<IMapObject>> Map::LoadObjectsFromJson(int stageNumber, GameParameters::StageParamger& parameter)
{
   m_loadJson = std::make_unique<LoadJson>();
   m_loadJson->SetResources(m_commonResources);
   //�X�e�[�W�̃p�����[�^�̐ݒ�
   parameter.objectNumber = stageNumber;
   parameter.stagePos = m_loadJson->LoadStageData(stageNumber);
   m_stagePositions[stageNumber] = parameter.stagePos;
   //�p�����[�^����I�u�W�F�N�g��ǂݍ���
   m_loadJson->LoadObject(parameter);
   //�ǂݍ��񂾃I�u�W�F�N�g���擾���Ĕz��Ɋi�[����
   auto jsonObjects = m_loadJson->GetMapObjects();
   auto commonObjects = m_loadJson->GetCommonMapObjects();
   std::move(commonObjects.begin(), commonObjects.end(), std::back_inserter(m_commonObjects));

   return jsonObjects;
}


/// <summary>
/// CSV�t�@�C������I�u�W�F�N�g����
/// </summary>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <param name="parameter">�X�e�[�W�̃p�����[�^</param>
/// <returns></returns>
std::vector<std::unique_ptr<IMapObject>> Map::LoadObjectsFromCsv(int stageNumber, GameParameters::StageParamger& parameter)
{
   //���[�hCSV�𐶐�
   m_loadCsv = std::make_unique<LoadCSV>(stageNumber);
   //�p�����[�^����I�u�W�F�N�g�𐶐�����
   m_loadCsv->CraeteObject(parameter);
   //���������I�u�W�F�N�g���擾���Ĕz��Ɋi�[����
   m_map[stageNumber] = std::move(m_loadCsv->GetMapData());
   //�ڑ��m�[�h�z����擾���Ēǉ�
   auto stageTransitions = m_loadCsv->GetStageTransition();
   m_stageTransition.insert(m_stageTransition.end(), std::make_move_iterator(stageTransitions.begin()), std::make_move_iterator(stageTransitions.end()));   
   return m_loadCsv->GetMapObjects();
}

/// <summary>
/// ���E�{�b�N�X�𐶐�����
/// </summary>
void Map::SetCollider()
{
	m_allMapBoundingBoxes.clear();	   //���E�{�b�N�X�̔z����N���A
	m_allMapBoundingSpheres.clear();   //���E���̔z����N���A
	m_allObjects.clear();			   //�S�ẴI�u�W�F�N�g�z����N���A

	//�S�ẴI�u�W�F�N�g����̔z��ɂ܂Ƃ߂�
	for (const auto& object : m_commonObjects)	//���ʃI�u�W�F�N�g
	{
	   m_allObjects.push_back(object.get());
	}
	for (const auto& stages : m_stageObjects)	//�X�e�[�W�ʃI�u�W�F�N�g
	{
	   for (const auto& object : stages.second)
	   {
		  m_allObjects.push_back(object.get());
	   }
	}

	//���E�{�b�N�X�z�����ɂ܂Ƃ߂�
	for (const auto& stages : m_stageObjects)
	{
	   //���݂���X�e�[�W�ȊO���������Ȃ�
	   if (stages.first != m_commonResources->GetBlackBoard()->GetStageNumber()) continue;

	   for (const auto& objects : stages.second)
	   {
		 //�R���W�����I�u�W�F�N�g���擾����
		 CollisionObject* collisionObj = objects->GetCollisionObject();
		 if (!collisionObj) continue;

		 //�Փ˔���̃p�����[�^���擾
		 const auto& param = collisionObj->GetCollisionParameter();
		 if (param.collision.collisionType < CollisionManager::CollisionType::PUSH) continue;
		 if (!param.collisionFlag) continue;

		 if (param.objectID == CollisionManager::ObjectType::FLOOR) continue;
		 //�z��ɂ܂Ƃ߂�
		 m_allMapBoundingBoxes.push_back(param.collision.box);
		 m_allMapBoundingSpheres.push_back(param.collision.sphere);
	   }
	}

	for (const auto& object : m_allObjects)
	{
	   //�R���W�����I�u�W�F�N�g���擾����
	   CollisionObject* collisionObj = object->GetCollisionObject();
	   if (!collisionObj) continue;

	   //�Փ˔���̃p�����[�^���擾
	   const auto& param = collisionObj->GetCollisionParameter();
	   if (param.collision.collisionType < CollisionManager::CollisionType::PUSH) continue;
	   if (!param.collisionFlag) continue;

	   //�������ǂ̂ݓo�^
	   if (param.objectID != CollisionManager::ObjectType::WALL) continue;

	   m_shadowObjects.push_back(object);
	}
}

/// <summary>
/// �N���A����
/// </summary>
void Map::Clear()
{
   //�����蔻���������
   m_allMapBoundingBoxes.clear();
   m_allMapBoundingSpheres.clear();
   m_stageObjects.clear();
   m_commonObjects.clear();
   m_allObjects.clear();
   m_gimmickObjects.clear();
   m_deviceObjects.clear();
}

/// <summary>
/// �M�~�b�N�I�u�W�F�N�g�z����쐬
/// </summary>
void Map::RegisterGimmickObjects()
{
   for (const auto& objects : m_stageObjects)
   {
	  for (const auto& object : objects.second)
	  {
		 if (!object->AsGimmick()) continue;
		 auto* gimmick = dynamic_cast<GimmickObjectBase*>(object.get());
		 //�M�~�b�N�I�u�W�F�N�g��z��Ɋi�[
		 m_gimmickObjects.push_back(gimmick);
	  }
   }
}


/// <summary>
/// �f�o�C�X�I�u�W�F�N�g�z����쐬
/// </summary>
void Map::RegisterDeviceObjects()
{
   for (const auto& objects : m_stageObjects)
   {
	  for (const auto& object : objects.second)
	  {
		 if (!object->AsDevice()) continue;
		 auto* device = dynamic_cast<DeviceObjectBase*>(object.get());
		 //�f�o�C�X�I�u�W�F�N�g��z��Ɋi�[
		 m_deviceObjects.push_back(device);
	  }
   }
}


//�w��X�e�[�W�̋��E�{�b�N�X���擾����
std::vector<DirectX::BoundingBox> Map::GetStageBoundingBoxes(int stageNumber) const
{
   std::vector<DirectX::BoundingBox> result;
   auto it = m_stageObjects.find(stageNumber);
   if (it == m_stageObjects.end()) return result;

   for (const auto& obj : it->second)
   {
	  CollisionObject* collisionObj = obj->GetCollisionObject();
	  if (!collisionObj) continue;
	  const auto& param = collisionObj->GetCollisionParameter();
	  if (param.collision.collisionType < CollisionManager::CollisionType::PUSH) continue;
	  if (!param.collisionFlag) continue;
	  if (param.objectID == CollisionManager::ObjectType::FLOOR) continue;
	  result.push_back(param.collision.box);
   }
   return result;
}

