/*
	@file	CollisionManager.cpp
	@brief	�R���W�����}�l�[�W���[�N���X
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Collision/CollisionManager.h"
#include "Game/Collision/CollisionObject.h"

/// <summary>
/// �V���O���g���̃C���X�^���X���擾����
/// </summary>
/// <returns></returns>
CollisionManager& CollisionManager::GetInstance()
{
   static CollisionManager instance;
   return instance;
}

/// <summary>
// �R���X�g���N�^
/// </summary>
CollisionManager::CollisionManager()
{
   RegisterThread();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CollisionManager::~CollisionManager()
{
   //�ʃX���b�h�I��
   ExitThread();
   //���Z�b�g
   ClearObjects();
}

/// <summary>
/// �X�V����
/// </summary>
void CollisionManager::Update()
{
   //�Փ˔��蒆�t���O��ݒ�
   m_isProcessingCollisions = true;

   {
	  //�Փ˔��菈���̎��s�X���b�h�ɒʒm
	  std::lock_guard<std::mutex> lock(m_mutex);
	  m_updateRequested = true;
   }

   //�X���b�h���N��������
   m_cv.notify_one();

   //�Փ˔���̊�����ҋ@����
   {
	  std::unique_lock<std::mutex> lock(m_mutex);
	  m_cv_complete.wait(lock, [this]() { return !m_updateRequested || m_exitRequested; });
   }

   //�Փ˔��肪���������̂ŁA�폜�L���[������
   ProcessDeleteQueue();
   //�Փ˔��蒆�t���O������
   m_isProcessingCollisions = false;
}


/// <summary>
/// �폜�L���[������
/// </summary>
void CollisionManager::ProcessDeleteQueue()
{
   //�폜�L���[����Ȃ�X�L�b�v
   if (m_pendingDeleteQueue.empty()) return;

   //�L���[�����b�N���ď���
   std::vector<DeleteQueueParameter> localQueue;
   {
	  std::lock_guard<std::mutex> lock(m_deleteQueueMutex);
	  //�L���[�̓��e�����[�J���ɃR�s�[���Ă���N���A
	  localQueue = std::move(m_pendingDeleteQueue);
	  m_pendingDeleteQueue.clear();
   }

   //�폜�ΏۃI�u�W�F�N�g���X�g���쐬
   std::vector<CollisionObject*> objectsToDelete;
   for (const auto& deleteDate : localQueue)
   {
	  objectsToDelete.push_back(deleteDate.object);
   }

   //�Փ˃y�A����폜�ΏۃI�u�W�F�N�g������
   for (auto* object : objectsToDelete)
   {
	  RemoveObjectFromCollisionSet(m_previousCollisions, object);
	  RemoveObjectFromCollisionSet(m_currentCollisions, object); // ���S�̂���  
   }


   //�ʏ�̃I�u�W�F�N�g����z��̍폜����
   for (const auto& deleteData : localQueue)
   {
	  int stageNumber = deleteData.stageNumber;
	  auto object = deleteData.object;

	  auto it = std::remove_if(m_stageObjects[stageNumber].begin(), m_stageObjects[stageNumber].end(),
		 [object](const std::unique_ptr<CollisionObject>& obj)
		 { return obj.get() == object;  });

	  //�z�񂩂珜�O����
	  m_stageObjects[stageNumber].erase(it, m_stageObjects[stageNumber].end());

	  //�Փ˔�����s���I�u�W�F�N�g��ύX
	  ChangeStageObjects(stageNumber);
   }
}

/// <summary>
/// �R���W�����I�u�W�F�N�g�̂ݐ�������
/// </summary>
/// <param name="param">�Փ˔���̃p�����[�^</param>
/// <returns></returns>
std::unique_ptr<CollisionObject> CollisionManager::CreateCollisionObject(const CollisionParameter& param)
{
   return std::make_unique<CollisionObject>(param);
}

/// <summary>
/// �I�u�W�F�N�g���X�g��o�^����
/// </summary>
/// <param name="param">�Փ˔���̃p�����[�^</param>
/// <returns></returns>
CollisionObject* CollisionManager::RegisterObject(const CollisionParameter& param, const int& stageNumber)
{
   auto object = std::make_unique<CollisionObject>(param);
   CollisionObject* rawPtr = object.get();

   //�X�e�[�W�ԍ��������Ă���ꍇ�X�e�[�W�I�u�W�F�N�g�ɉ�����
   if (stageNumber > 0)
   {
	  m_stageObjects[stageNumber].push_back(std::move(object));
   }
   else //�����Ă��Ȃ��ꍇ���ʃI�u�W�F�N�g�ɉ�����
   {
	  m_commonObjects.push_back(std::move(object));
   }
   return rawPtr;
}

/// <summary>
/// �I�u�W�F�N�g���X�g����폜����
/// </summary>
/// <param name="object">�R���W�����I�u�W�F�N�g</param>
void CollisionManager::UnregisterCommonObject(CollisionObject* object)
{
   auto it = std::remove_if(m_commonObjects.begin(), m_commonObjects.end(),
	  [object](const std::unique_ptr<CollisionObject>& obj)
	  { return obj.get() == object;  });
   //�z�񂩂珜�O����
   m_commonObjects.erase(it, m_commonObjects.end());
}

/// <summary>
/// �X�e�[�W���Ƃ̃I�u�W�F�N�g���폜����
/// </summary>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <param name="object">�Փ˔���̃p�����[�^</param>
void CollisionManager::UnregisterStageObject(const int& stageNumber, CollisionObject* object)
{
   {
	  std::lock_guard<std::mutex> lock(m_deleteQueueMutex);
	  DeleteQueueParameter queue = { stageNumber, object };
	  m_pendingDeleteQueue.push_back(queue);
   }
}

/// <summary>
/// �Փ˔���̎��s
/// </summary>
void CollisionManager::CheckCollisions()
{
   //�Փ˂������̔���
   CollisionEnter();
   //�Փ˂𗣒E�������̔���
   CollisionExit();
}

/// <summary>
/// �Փ˂������̔���
/// </summary>
void CollisionManager::CollisionEnter()
{
   m_currentCollisions.clear();

   for (int i = 0; i < m_allObjects.size(); i++)
   {
	  for (int j = i + 1; j < m_allObjects.size(); j++)
	  {
		 const auto object1 = m_allObjects[i];
		 const auto object2 = m_allObjects[j];

		 if (ShouldSkipCollision(*object1, *object2)) continue;

		 //�Փ˔�����s��
		 if (object1->CollidesWith(object2))
		 {
			//�Փ˃y�A���L�^
			m_currentCollisions.insert(std::make_pair(object1, object2));

			object1->HandleCollision(object2);
			object2->HandleCollision(object1);
		 }
	  }
   }
}

/// <summary>
/// �Փ˂𗣒E�������̔���
/// </summary>
void CollisionManager::CollisionExit()
{
   //���E����
   for (const auto& prevPair : m_previousCollisions)
   {
	  if (m_currentCollisions.find(prevPair) == m_currentCollisions.end())
	  {
		 //���̃y�A�͗��E����
		 prevPair.first->HandleCollisionExit(prevPair.second);
		 prevPair.second->HandleCollisionExit(prevPair.first);
	  }
   }

   //����̏Փ˂����̃t���[���̂��߂ɕۑ�
   m_previousCollisions = m_currentCollisions;
}

/// <summary>
/// ���݂̃X�e�[�W�̏Փ˔��胊�X�g�ɕύX
/// </summary>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
void CollisionManager::ChangeStageObjects(const int& stageNumber)
{
   //�S�I�u�W�F�N�g�z��Ǝ�����J�����O�p�z���������
   m_allObjects.clear();
   m_frustumObjects.clear();

   // ���ʃI�u�W�F�N�g��ǉ�
   m_allObjects.reserve(m_commonObjects.size() + m_stageObjects[stageNumber].size());
   //������J�����O�p�̔z��ɃI�u�W�F�N�g��ǉ�
   m_frustumObjects.reserve(m_commonObjects.size() + m_stageObjects.size());

   //���ʃI�u�W�F�N�g�ƃX�e�[�W�I�u�W�F�N�g��z��ɂ܂Ƃ߂�
   //������J�����O�p�z����܂Ƃ߂�
   for (const auto& object : m_commonObjects)
   {
	  m_allObjects.push_back(object.get());
	  m_frustumObjects.push_back(object.get());
   }
   for (const auto& objects : m_stageObjects)
   {
	  for (const auto& object : objects.second)
	  {
		 if (objects.first == stageNumber)
		 {
			m_allObjects.push_back(object.get());
		 }

		 m_frustumObjects.push_back(object.get());
	  }
   }
}

/// <summary>
/// �S�I�u�W�F�N�g�z����N���A����
/// </summary>
void CollisionManager::ClearObjects()
{
   m_commonObjects.clear();
   m_stageObjects.clear();
   m_previousCollisions.clear();
}

/// <summary>
/// �X�L�b�v����Փ˔���
/// </summary>
/// <param name="object1"></param>
/// <param name="object2"></param>
/// <returns></returns>
bool CollisionManager::ShouldSkipCollision(const CollisionObject& object1, const CollisionObject& object2)
{
   //�I�u�W�F�N�g��ID���擾����
   const auto object1ID = object1.GetCollisionParameter().objectID;
   const auto object2ID = object2.GetCollisionParameter().objectID;

   //�����I�u�W�F�N�g���m�̔�����X�L�b�v�A�}�b�v�I�u�W�F�N�g���m�̔�����X�L�b�v
   if (object1ID == object2ID) return true;
   if ((object1ID >= ObjectType::OBJECT) && (object2ID >= ObjectType::OBJECT)) return true;

   return false;
}

/// <summary>
/// �ʃX���b�h�Ɋ֐���o�^����(�}���`�X���b�h)
/// </summary>
void CollisionManager::RegisterThread()
{
   //�X���b�h�J�n���ɏI���t���O��������
   m_exitRequested = false;
   m_updateRequested = false;

   m_collisionThread = std::thread([this]
   {
	  while (true)
	  {
		 std::unique_lock<std::mutex> lock(m_mutex);
		 m_cv.wait(lock, [this]() { return m_updateRequested || m_exitRequested; });

		 if (m_exitRequested)
		 {
			break;
		 }

		 //�Փ˔���̎��s
		 CheckCollisions();

		 m_updateRequested = false;

		 lock.unlock();
		 m_cv_complete.notify_one();
	  }
	  ClearObjects();
   });
}


/// <summary>
/// �ʃX���b�h�̊֐��o�^����������
/// </summary>
void CollisionManager::ExitThread()
{
   //�I�����N�G�X�g���Z�b�g
   {
	  std::lock_guard<std::mutex> lock(m_mutex);
	  m_exitRequested = true;
   }

   //�����ϐ���ʒm���đҋ@���̃X���b�h���N����
   m_cv.notify_all();

   //�X���b�h�������Ă���ΏI���܂őҋ@����
   if (m_collisionThread.joinable())
   {
	  try
	  {
		 m_collisionThread.join();
	  }
	  catch (const std::system_error& e)
	  {
		 // �X���b�hjoin���s�̃��O���o��
		 std::string errorMsg = "Thread join failed: " + std::string(e.what());
		 MessageBoxA(nullptr, errorMsg.c_str(), "�G���[", MB_OK | MB_ICONERROR);
	  }
   }
}


// �Փ˃y�A�������I�u�W�F�N�g���܂ރG���g�����폜
void CollisionManager::RemoveObjectFromCollisionSet(
   std::set<std::pair<const CollisionObject*,const CollisionObject*>>& collisionSet,
   CollisionObject* target)
{
   auto it = collisionSet.begin();
   while (it != collisionSet.end()) 
   {
	  if (it->first == target || it->second == target) {
		 it = collisionSet.erase(it);
	  }
	  else {
		 ++it;
	  }
   }
}

/// <summary>
/// ���E�{�b�N�X�𐶐�����(�y�ʉ��̂��ߋ��E���������Ő���)
/// </summary>
/// <param name="purpose">�Փ˔���̖ړI</param>
/// <param name="type">����̃^�C�v</param>
/// <param name="box">�{�b�N�X�̐ݒ�</param>
/// <returns></returns>
CollisionManager::Collision CollisionManager::CreateBoxCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingBox& box)
{
   Collision collision;
   collision.purpose = purpose;								  //�Փ˔���̖ړI
   collision.collisionType = type;							  //����̃^�C�v
   collision.box = box;										  //�{�b�N�X�̐ݒ�
   float radius = std::max({ box.Extents.x, box.Extents.y, box.Extents.z });
   collision.sphere.Center = box.Center;					   //���̐ݒ�
   collision.sphere.Radius = radius;
   collision.checkType = CollisionCheckType::SphereAndBox;	  //���莞�̎��
   return collision;
}

/// <summary>
/// ���E���𐶐�����
/// </summary>
/// <param name="purpose">�Փ˔���̖ړI</param>
/// <param name="type">����̃^�C�v</param>
/// <param name="sphere">���̐ݒ�</param>
/// <returns></returns>
CollisionManager::Collision CollisionManager::CreateSphereCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingSphere& sphere)
{
   Collision collision;
   collision.purpose = purpose;								  //�Փ˔���̖ړI
   collision.collisionType = type;							  //����̃^�C�v
   collision.box = DirectX::BoundingBox();					  //�{�b�N�X�̐ݒ�(�f�t�H���g�ݒ�)
   collision.sphere = sphere;								  //���̐ݒ�
   collision.checkType = CollisionCheckType::SphereOnly;	  //���莞�̎��
   return collision;
}
