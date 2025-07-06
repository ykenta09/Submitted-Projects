/*
	@file	CollisionManager.h
	@brief	コリジョンマネージャークラス
*/
#pragma once
#ifndef COLLISION_MANAGER_DEFINED
#define COLLISION_MANAGER_DEFINED
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>

// 前方宣言
class CollisionObject;

class CollisionManager
{
public:
   //シングルトンインスタンス取得
   static CollisionManager& GetInstance();

   //衝突判定のチェックタイプ
   enum class CollisionCheckType
   {
	  SphereOnly,		//球体のみ
	  BoxOnly,			//ボックスのみ
	  SphereAndBox		//両方
   };
   //オブジェクトタイプ
   enum class ObjectType
   {
	  NOTYPE = 0,
	  PLAYER,	  //プレイヤー
	  ENEMY,	  //エネミー
	  FELLOW,	  //仲間
	  OBJECT,	  //オブジェクト(マップオブジェクトはここより下に登録)
	  FLOOR,	  //床
	  WALL,		  //壁
	  PRISON,	  //牢屋
	  GOAL,		  //ゴール
	  KEY,		  //鍵
	  DOOR,		  //ドア
	  LEVER		  //レバー
   };

   //当たり判定の目的
   enum class CollisionPurpose
   {
	  PHYSICAL,			// 物理的な衝突（押し出しなど）
	  EVENT_TRIGGER,    // イベントトリガー
	  Sensor			// 検知のみ（実際の衝突処理は行わない）
   };

   //当たり判定の種類
   enum class CollisionType
   {
	  NONE = 0,		 //なし
	  HIT,			 //ヒット判定のみ
	  PUSH			 //押し出し処理を行う
   };

   //当たり判定
   struct Collision
   {
	  DirectX::BoundingSphere sphere;		 //境界球
	  DirectX::BoundingBox box;				 //境界ボックス
	  CollisionCheckType checkType;		     //当たり判定のチェックタイプ
	  CollisionPurpose purpose;			     //当たり判定の目的
	  CollisionType collisionType;			 //当たり判定のタイプ
   }; 

   //当たり判定のパラメータ
   struct CollisionParameter
   {
	  void* owner = nullptr;
	  ObjectType objectID = ObjectType::NOTYPE;								  //オブジェクトID
	  Collision collision{};												  //当たり判定
	  std::function<void(const CollisionObject*)> NotifyEvent = nullptr;	  //衝突時に呼ばれるイベント関数
	  std::function<void(const CollisionObject*)> NotifyExitEvent = nullptr;
	  bool collisionFlag = true;											  //当たり判定が有効か
   };

   struct DeleteQueueParameter
   {
	  int stageNumber;
	  CollisionObject* object;
   };

public:
   //視錐台カリング用の配列を取得する
   std::vector<CollisionObject*> GetFrustumObject() { return m_frustumObjects; };

public:
	//コンストラクタ
	CollisionManager();
	~CollisionManager();

	//更新する
	void Update();

	void ProcessDeleteQueue();

	/// コリジョンオブジェクトのみ生成する
	std::unique_ptr<CollisionObject> CreateCollisionObject(const CollisionParameter& param);

	//オブジェクト登録
	CollisionObject* RegisterObject(const CollisionParameter& param, const int& stageNumber = 0);

	// オブジェクトの削除
	void UnregisterCommonObject(CollisionObject* object);
	void UnregisterStageObject(const int& stageNumber, CollisionObject* object);

	// 衝突判定を行うリストを変更する
	void ChangeStageObjects(const int& stageNumber);

	void RemoveObjectFromCollisionSet(
	   std::set<std::pair<const CollisionObject*, const CollisionObject*>>& collisionSet,
	   CollisionObject* target);

	// 衝突判定の実行
	void CheckCollisions();

	//衝突しているかの判定
	void CollisionEnter();
	//衝突を離脱したかの判定
	void CollisionExit();

	// 全オブジェクトのクリア
	void ClearObjects();

	// 境界ボックスを生成する
	Collision CreateBoxCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingBox& box);
	// 境界球を生成する
	Collision CreateSphereCollision(CollisionManager::CollisionPurpose purpose, CollisionManager::CollisionType type, const DirectX::BoundingSphere& sphere);
private:
   //スキップする衝突判定
   bool ShouldSkipCollision(const CollisionObject& object1, const CollisionObject& object2);
   // 別スレッドに関数を登録する(マルチスレッド)
   void RegisterThread();
   //別スレッドの関数登録を解除する(マルチスレッド)
   void ExitThread();
private:
   std::vector<CollisionObject*> m_allObjects;		  //全ての登録オブジェクトリスト
   std::vector<CollisionObject*> m_frustumObjects;	  //視錐台カリング用オブジェクトリスト

   std::vector<std::unique_ptr<CollisionObject>> m_commonObjects;							 //共通の登録オブジェクトリスト
   std::unordered_map<int, std::vector<std::unique_ptr<CollisionObject>>>  m_stageObjects;   //ステージごとに変更する登録オブジェクトリスト


   std::set<std::pair<const CollisionObject*, const CollisionObject*>> m_currentCollisions;	   //今フレーム衝突しているオブジェクト
   std::set<std::pair<const CollisionObject*, const CollisionObject*>> m_previousCollisions;	   //前フレーム衝突していたオブジェクト

   // === マルチスレッド ===
   // 衝突判定専用のスレッドオブジェクト
   std::thread m_collisionThread;
   // スレッド保護用のミューテックス
   std::mutex m_mutex;
   // スレッド起床通知のための条件変数
   std::condition_variable m_cv;
   // Update関数内における衝突判定要求フラグ
   bool m_updateRequested = false;
   // デストラクタでのスレッド終了を支持するフラグ
   bool m_exitRequested = false;

   // === 削除処理待機キュー ===
   std::vector<DeleteQueueParameter> m_pendingDeleteQueue;
   // 削除キュー保護用ミューテックス
   std::mutex m_deleteQueueMutex;
   // 衝突処理中かどうかのフラグ（主に待機判断に使用）
   std::atomic<bool> m_isProcessingCollisions;
   // Update終了通知用の条件変数
   std::condition_variable m_cv_complete;
};
#endif // COLLISIONMANAGER_DEFINED

