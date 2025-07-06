/*
	@file	CollisionManager.h
	@brief	�R���W�����}�l�[�W���[�N���X
*/
#pragma once
#ifndef COLLISION_MANAGER_DEFINED
#define COLLISION_MANAGER_DEFINED
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>

// �O���錾
class CollisionObject;

class CollisionManager
{
public:
   //�V���O���g���C���X�^���X�擾
   static CollisionManager& GetInstance();

   //�Փ˔���̃`�F�b�N�^�C�v
   enum class CollisionCheckType
   {
	  SphereOnly,		//���̂̂�
	  BoxOnly,			//�{�b�N�X�̂�
	  SphereAndBox		//����
   };
   //�I�u�W�F�N�g�^�C�v
   enum class ObjectType
   {
	  NOTYPE = 0,
	  PLAYER,	  //�v���C���[
	  ENEMY,	  //�G�l�~�[
	  FELLOW,	  //����
	  OBJECT,	  //�I�u�W�F�N�g(�}�b�v�I�u�W�F�N�g�͂�����艺�ɓo�^)
	  FLOOR,	  //��
	  WALL,		  //��
	  PRISON,	  //�S��
	  GOAL,		  //�S�[��
	  KEY,		  //��
	  DOOR,		  //�h�A
	  LEVER		  //���o�[
   };

   //�����蔻��̖ړI
   enum class CollisionPurpose
   {
	  PHYSICAL,			// �����I�ȏՓˁi�����o���Ȃǁj
	  EVENT_TRIGGER,    // �C�x���g�g���K�[
	  Sensor			// ���m�̂݁i���ۂ̏Փˏ����͍s��Ȃ��j
   };

   //�����蔻��̎��
   enum class CollisionType
   {
	  NONE = 0,		 //�Ȃ�
	  HIT,			 //�q�b�g����̂�
	  PUSH			 //�����o���������s��
   };

   //�����蔻��
   struct Collision
   {
	  DirectX::BoundingSphere sphere;		 //���E��
	  DirectX::BoundingBox box;				 //���E�{�b�N�X
	  CollisionCheckType checkType;		     //�����蔻��̃`�F�b�N�^�C�v
	  CollisionPurpose purpose;			     //�����蔻��̖ړI
	  CollisionType collisionType;			 //�����蔻��̃^�C�v
   }; 

   //�����蔻��̃p�����[�^
   struct CollisionParameter
   {
	  void* owner = nullptr;
	  ObjectType objectID = ObjectType::NOTYPE;								  //�I�u�W�F�N�gID
	  Collision collision{};												  //�����蔻��
	  std::function<void(const CollisionObject*)> NotifyEvent = nullptr;	  //�Փˎ��ɌĂ΂��C�x���g�֐�
	  std::function<void(const CollisionObject*)> NotifyExitEvent = nullptr;
	  bool collisionFlag = true;											  //�����蔻�肪�L����
   };

   struct DeleteQueueParameter
   {
	  int stageNumber;
	  CollisionObject* object;
   };

public:
   //������J�����O�p�̔z����擾����
   std::vector<CollisionObject*> GetFrustumObject() { return m_frustumObjects; };

public:
	//�R���X�g���N�^
	CollisionManager();
	~CollisionManager();

	//�X�V����
	void Update();

	void ProcessDeleteQueue();

	/// �R���W�����I�u�W�F�N�g�̂ݐ�������
	std::unique_ptr<CollisionObject> CreateCollisionObject(const CollisionParameter& param);

	//�I�u�W�F�N�g�o�^
	CollisionObject* RegisterObject(const CollisionParameter& param, const int& stageNumber = 0);

	// �I�u�W�F�N�g�̍폜
	void UnregisterCommonObject(CollisionObject* object);
	void UnregisterStageObject(const int& stageNumber, CollisionObject* object);

	// �Փ˔�����s�����X�g��ύX����
	void ChangeStageObjects(const int& stageNumber);

	void RemoveObjectFromCollisionSet(
	   std::set<std::pair<const CollisionObject*, const CollisionObject*>>& collisionSet,
	   CollisionObject* target);

	// �Փ˔���̎��s
	void CheckCollisions();

	//�Փ˂��Ă��邩�̔���
	void CollisionEnter();
	//�Փ˂𗣒E�������̔���
	void CollisionExit();

	// �S�I�u�W�F�N�g�̃N���A
	void ClearObjects();

	// ���E�{�b�N�X�𐶐�����
	Collision CreateBoxCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingBox& box);
	// ���E���𐶐�����
	Collision CreateSphereCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingSphere& sphere);
private:
   //�X�L�b�v����Փ˔���
   bool ShouldSkipCollision(const CollisionObject& object1, const CollisionObject& object2);
   // �ʃX���b�h�Ɋ֐���o�^����(�}���`�X���b�h)
   void RegisterThread();
   //�ʃX���b�h�̊֐��o�^����������(�}���`�X���b�h)
   void ExitThread();
private:
   std::vector<CollisionObject*> m_allObjects;		  //�S�Ă̓o�^�I�u�W�F�N�g���X�g
   std::vector<CollisionObject*> m_frustumObjects;	  //������J�����O�p�I�u�W�F�N�g���X�g

   std::vector<std::unique_ptr<CollisionObject>> m_commonObjects;							 //���ʂ̓o�^�I�u�W�F�N�g���X�g
   std::unordered_map<int, std::vector<std::unique_ptr<CollisionObject>>>  m_stageObjects;   //�X�e�[�W���ƂɕύX����o�^�I�u�W�F�N�g���X�g


   std::set<std::pair<const CollisionObject*, const CollisionObject*>> m_currentCollisions;	   //���t���[���Փ˂��Ă���I�u�W�F�N�g
   std::set<std::pair<const CollisionObject*, const CollisionObject*>> m_previousCollisions;	   //�O�t���[���Փ˂��Ă����I�u�W�F�N�g

   // === �}���`�X���b�h ===
   // �Փ˔����p�̃X���b�h�I�u�W�F�N�g
   std::thread m_collisionThread;
   // �X���b�h�ی�p�̃~���[�e�b�N�X
   std::mutex m_mutex;
   // �X���b�h�N���ʒm�̂��߂̏����ϐ�
   std::condition_variable m_cv;
   // Update�֐����ɂ�����Փ˔���v���t���O
   bool m_updateRequested = false;
   // �f�X�g���N�^�ł̃X���b�h�I�����x������t���O
   bool m_exitRequested = false;

   // === �폜�����ҋ@�L���[ ===
   std::vector<DeleteQueueParameter> m_pendingDeleteQueue;
   // �폜�L���[�ی�p�~���[�e�b�N�X
   std::mutex m_deleteQueueMutex;
   // �Փˏ��������ǂ����̃t���O�i��ɑҋ@���f�Ɏg�p�j
   std::atomic<bool> m_isProcessingCollisions;
   // Update�I���ʒm�p�̏����ϐ�
   std::condition_variable m_cv_complete;
};
#endif // COLLISIONMANAGER_DEFINED

