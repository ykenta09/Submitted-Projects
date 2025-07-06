/*
	@file	CollisionManager.cpp
	@brief	コリジョンマネージャークラス
*/
#include "pch.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Collision/CollisionManager.h"
#include "Game/Collision/CollisionObject.h"

/// <summary>
/// シングルトンのインスタンスを取得する
/// </summary>
/// <returns></returns>
CollisionManager& CollisionManager::GetInstance()
{
   static CollisionManager instance;
   return instance;
}

/// <summary>
// コンストラクタ
/// </summary>
CollisionManager::CollisionManager()
{
   RegisterThread();
}

/// <summary>
/// デストラクタ
/// </summary>
CollisionManager::~CollisionManager()
{
   //別スレッド終了
   ExitThread();
   //リセット
   ClearObjects();
}

/// <summary>
/// 更新する
/// </summary>
void CollisionManager::Update()
{
   //衝突判定中フラグを設定
   m_isProcessingCollisions = true;

   {
	  //衝突判定処理の実行スレッドに通知
	  std::lock_guard<std::mutex> lock(m_mutex);
	  m_updateRequested = true;
   }

   //スレッドを起床させる
   m_cv.notify_one();

   //衝突判定の完了を待機する
   {
	  std::unique_lock<std::mutex> lock(m_mutex);
	  m_cv_complete.wait(lock, [this]() { return !m_updateRequested || m_exitRequested; });
   }

   //衝突判定が完了したので、削除キューを処理
   ProcessDeleteQueue();
   //衝突判定中フラグを解除
   m_isProcessingCollisions = false;
}


/// <summary>
/// 削除キューを処理
/// </summary>
void CollisionManager::ProcessDeleteQueue()
{
   //削除キューが空ならスキップ
   if (m_pendingDeleteQueue.empty()) return;

   //キューをロックして処理
   std::vector<DeleteQueueParameter> localQueue;
   {
	  std::lock_guard<std::mutex> lock(m_deleteQueueMutex);
	  //キューの内容をローカルにコピーしてからクリア
	  localQueue = std::move(m_pendingDeleteQueue);
	  m_pendingDeleteQueue.clear();
   }

   //削除対象オブジェクトリストを作成
   std::vector<CollisionObject*> objectsToDelete;
   for (const auto& deleteDate : localQueue)
   {
	  objectsToDelete.push_back(deleteDate.object);
   }

   //衝突ペアから削除対象オブジェクトを除去
   for (auto* object : objectsToDelete)
   {
	  RemoveObjectFromCollisionSet(m_previousCollisions, object);
	  RemoveObjectFromCollisionSet(m_currentCollisions, object); // 安全のため  
   }


   //通常のオブジェクト判定配列の削除処理
   for (const auto& deleteData : localQueue)
   {
	  int stageNumber = deleteData.stageNumber;
	  auto object = deleteData.object;

	  auto it = std::remove_if(m_stageObjects[stageNumber].begin(), m_stageObjects[stageNumber].end(),
		 [object](const std::unique_ptr<CollisionObject>& obj)
		 { return obj.get() == object;  });

	  //配列から除外する
	  m_stageObjects[stageNumber].erase(it, m_stageObjects[stageNumber].end());

	  //衝突判定を行うオブジェクトを変更
	  ChangeStageObjects(stageNumber);
   }
}

/// <summary>
/// コリジョンオブジェクトのみ生成する
/// </summary>
/// <param name="param">衝突判定のパラメータ</param>
/// <returns></returns>
std::unique_ptr<CollisionObject> CollisionManager::CreateCollisionObject(const CollisionParameter& param)
{
   return std::make_unique<CollisionObject>(param);
}

/// <summary>
/// オブジェクトリストを登録する
/// </summary>
/// <param name="param">衝突判定のパラメータ</param>
/// <returns></returns>
CollisionObject* CollisionManager::RegisterObject(const CollisionParameter& param, const int& stageNumber)
{
   auto object = std::make_unique<CollisionObject>(param);
   CollisionObject* rawPtr = object.get();

   //ステージ番号が入っている場合ステージオブジェクトに加える
   if (stageNumber > 0)
   {
	  m_stageObjects[stageNumber].push_back(std::move(object));
   }
   else //入っていない場合共通オブジェクトに加える
   {
	  m_commonObjects.push_back(std::move(object));
   }
   return rawPtr;
}

/// <summary>
/// オブジェクトリストから削除する
/// </summary>
/// <param name="object">コリジョンオブジェクト</param>
void CollisionManager::UnregisterCommonObject(CollisionObject* object)
{
   auto it = std::remove_if(m_commonObjects.begin(), m_commonObjects.end(),
	  [object](const std::unique_ptr<CollisionObject>& obj)
	  { return obj.get() == object;  });
   //配列から除外する
   m_commonObjects.erase(it, m_commonObjects.end());
}

/// <summary>
/// ステージごとのオブジェクトを削除する
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
/// <param name="object">衝突判定のパラメータ</param>
void CollisionManager::UnregisterStageObject(const int& stageNumber, CollisionObject* object)
{
   {
	  std::lock_guard<std::mutex> lock(m_deleteQueueMutex);
	  DeleteQueueParameter queue = { stageNumber, object };
	  m_pendingDeleteQueue.push_back(queue);
   }
}

/// <summary>
/// 衝突判定の実行
/// </summary>
void CollisionManager::CheckCollisions()
{
   //衝突したかの判定
   CollisionEnter();
   //衝突を離脱したかの判定
   CollisionExit();
}

/// <summary>
/// 衝突したかの判定
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

		 //衝突判定を行う
		 if (object1->CollidesWith(object2))
		 {
			//衝突ペアを記録
			m_currentCollisions.insert(std::make_pair(object1, object2));

			object1->HandleCollision(object2);
			object2->HandleCollision(object1);
		 }
	  }
   }
}

/// <summary>
/// 衝突を離脱したかの判定
/// </summary>
void CollisionManager::CollisionExit()
{
   //離脱判定
   for (const auto& prevPair : m_previousCollisions)
   {
	  if (m_currentCollisions.find(prevPair) == m_currentCollisions.end())
	  {
		 //このペアは離脱した
		 prevPair.first->HandleCollisionExit(prevPair.second);
		 prevPair.second->HandleCollisionExit(prevPair.first);
	  }
   }

   //今回の衝突を次のフレームのために保存
   m_previousCollisions = m_currentCollisions;
}

/// <summary>
/// 現在のステージの衝突判定リストに変更
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
void CollisionManager::ChangeStageObjects(const int& stageNumber)
{
   //全オブジェクト配列と視錐台カリング用配列を初期化
   m_allObjects.clear();
   m_frustumObjects.clear();

   // 共通オブジェクトを追加
   m_allObjects.reserve(m_commonObjects.size() + m_stageObjects[stageNumber].size());
   //視錐台カリング用の配列にオブジェクトを追加
   m_frustumObjects.reserve(m_commonObjects.size() + m_stageObjects.size());

   //共通オブジェクトとステージオブジェクトを配列にまとめる
   //視錐台カリング用配列をまとめる
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
/// 全オブジェクト配列をクリアする
/// </summary>
void CollisionManager::ClearObjects()
{
   m_commonObjects.clear();
   m_stageObjects.clear();
   m_previousCollisions.clear();
}

/// <summary>
/// スキップする衝突判定
/// </summary>
/// <param name="object1"></param>
/// <param name="object2"></param>
/// <returns></returns>
bool CollisionManager::ShouldSkipCollision(const CollisionObject& object1, const CollisionObject& object2)
{
   //オブジェクトのIDを取得する
   const auto object1ID = object1.GetCollisionParameter().objectID;
   const auto object2ID = object2.GetCollisionParameter().objectID;

   //同じオブジェクト同士の判定をスキップ、マップオブジェクト同士の判定をスキップ
   if (object1ID == object2ID) return true;
   if ((object1ID >= ObjectType::OBJECT) && (object2ID >= ObjectType::OBJECT)) return true;

   return false;
}

/// <summary>
/// 別スレッドに関数を登録する(マルチスレッド)
/// </summary>
void CollisionManager::RegisterThread()
{
   //スレッド開始時に終了フラグを初期化
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

		 //衝突判定の実行
		 CheckCollisions();

		 m_updateRequested = false;

		 lock.unlock();
		 m_cv_complete.notify_one();
	  }
	  ClearObjects();
   });
}


/// <summary>
/// 別スレッドの関数登録を解除する
/// </summary>
void CollisionManager::ExitThread()
{
   //終了リクエストをセット
   {
	  std::lock_guard<std::mutex> lock(m_mutex);
	  m_exitRequested = true;
   }

   //条件変数を通知して待機中のスレッドを起こす
   m_cv.notify_all();

   //スレッドが生きていれば終了まで待機する
   if (m_collisionThread.joinable())
   {
	  try
	  {
		 m_collisionThread.join();
	  }
	  catch (const std::system_error& e)
	  {
		 // スレッドjoin失敗のログを出力
		 std::string errorMsg = "Thread join failed: " + std::string(e.what());
		 MessageBoxA(nullptr, errorMsg.c_str(), "エラー", MB_OK | MB_ICONERROR);
	  }
   }
}


// 衝突ペアから特定オブジェクトを含むエントリを削除
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
/// 境界ボックスを生成する(軽量化のため境界球も自動で生成)
/// </summary>
/// <param name="purpose">衝突判定の目的</param>
/// <param name="type">判定のタイプ</param>
/// <param name="box">ボックスの設定</param>
/// <returns></returns>
CollisionManager::Collision CollisionManager::CreateBoxCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingBox& box)
{
   Collision collision;
   collision.purpose = purpose;								  //衝突判定の目的
   collision.collisionType = type;							  //判定のタイプ
   collision.box = box;										  //ボックスの設定
   float radius = std::max({ box.Extents.x, box.Extents.y, box.Extents.z });
   collision.sphere.Center = box.Center;					   //球の設定
   collision.sphere.Radius = radius;
   collision.checkType = CollisionCheckType::SphereAndBox;	  //判定時の種類
   return collision;
}

/// <summary>
/// 境界球を生成する
/// </summary>
/// <param name="purpose">衝突判定の目的</param>
/// <param name="type">判定のタイプ</param>
/// <param name="sphere">球の設定</param>
/// <returns></returns>
CollisionManager::Collision CollisionManager::CreateSphereCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingSphere& sphere)
{
   Collision collision;
   collision.purpose = purpose;								  //衝突判定の目的
   collision.collisionType = type;							  //判定のタイプ
   collision.box = DirectX::BoundingBox();					  //ボックスの設定(デフォルト設定)
   collision.sphere = sphere;								  //球の設定
   collision.checkType = CollisionCheckType::SphereOnly;	  //判定時の種類
   return collision;
}
