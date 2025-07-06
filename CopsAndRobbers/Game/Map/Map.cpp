/*
	@file	Map.cpp
	@brief	マップクラス
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
/// コンストラクタ
/// </summary>
/// <param name="resources">共通リソース</param>
Map::Map(CommonResources* resources)
   :
   m_commonResources(resources),
   m_isDrawCubeMap(false),
   m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
}

/// <summary>
/// 初期化する
/// </summary>
void Map::Initialize()
{
	//天球を生成、初期化する
	m_celestialSphere = std::make_unique<CelestialSphere>(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Quaternion::Identity);
	m_celestialSphere->Initialize();

	//視錐台を生成
	m_frustum = std::make_unique<FrustumCuller>();

	//ステージデータを読み込む
	LoadStageData();
	//境界ボックスを生成する
	SetCollider();
	//ギミックオブジェクトの配列を作成
	RegisterGimmickObjects();
	//デバイスオブジェクトの配列を作成
	RegisterDeviceObjects();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Map::Update(const float elapsedTime)
{
	//天球を更新する
	m_celestialSphere->Update(m_celestialSpherePosition);
	
	//オブジェクトの更新する
	for (const auto& object : m_allObjects)
	{
	   object->Update(elapsedTime);
	}

	//境界ボックスを更新する
	SetCollider();
}

/// <summary>
/// 描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void Map::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
   //デバック用(描画オブジェクトの数を保存)
   m_drawObject = 0;

   //天球を描画する
   m_celestialSphere->Render();
   
   //視錐台を作成
   DirectX::BoundingFrustum frustum = m_frustum->CreateViewFrustum(view, proj);

   //マップオブジェクトを視錐台カリングして描画する
   for (const auto& object : m_allObjects)
   {
	  //コリジョンオブジェクトを取得
	  CollisionObject* collisionObj = object->GetCollisionObject();
	  if (collisionObj)
	  {
		 //ボックスと視錐台で判定を行い描画
		 const auto box = collisionObj->GetCollisionParameter().collision.box;
		 if (m_frustum->IsVisible(frustum, box))
		 {
			m_drawObject++;
			object->Render();
		 }
	  }
	  else //コリジョンオブジェクトがないものを描画
	  {
		 m_drawObject++;
		 object->Render();
	  }
   }

#ifdef _DEBUG
      //デバック（境界ボックスを描画）
	  m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
	 [this](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
	 {
		 for (const auto& object : m_allObjects)
		 {
			//衝突判定のパラメータを取得する
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
/// 後始末する
/// </summary>
void Map::Finalize()
{
	// do nothing.
}

/// <summary>
/// オブジェクトデータを取得する
/// </summary>
void Map::LoadStageData()
{
   //ステージ１から最大ステージ数までマップオブジェクトのデータを読み込み、取得する
   for (int stageNumber = 1; stageNumber <= GameParameters::MAX_STAGE_NUM; stageNumber++)
   {
	  std::vector<std::unique_ptr<IMapObject>> stageObjects;

	  //ステージパラメータの設定
	  GameParameters::StageParamger paramter;
	  //jsonデータからオブジェクトを生成
	  auto jsonObjects = LoadObjectsFromJson(stageNumber, paramter);
	  //csvデータからオブジェクトを生成
	  auto csvObjects = LoadObjectsFromCsv(stageNumber, paramter);

	  stageObjects.reserve(jsonObjects.size() + csvObjects.size()); // 配列の容量を確保
	  std::move(jsonObjects.begin(), jsonObjects.end(), std::back_inserter(stageObjects));
	  std::move(csvObjects.begin(), csvObjects.end(), std::back_inserter(stageObjects));

	  // ステージ番号ごとに保存
	  m_stageObjects[stageNumber] = std::move(stageObjects);
   }
}


/// <summary>
/// JSONファイルからオブジェクト生成
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
/// <param name="parameter">ステージのパラメータ</param>
/// <returns></returns>
std::vector<std::unique_ptr<IMapObject>> Map::LoadObjectsFromJson(int stageNumber, GameParameters::StageParamger& parameter)
{
   m_loadJson = std::make_unique<LoadJson>();
   m_loadJson->SetResources(m_commonResources);
   //ステージのパラメータの設定
   parameter.objectNumber = stageNumber;
   parameter.stagePos = m_loadJson->LoadStageData(stageNumber);
   m_stagePositions[stageNumber] = parameter.stagePos;
   //パラメータからオブジェクトを読み込む
   m_loadJson->LoadObject(parameter);
   //読み込んだオブジェクトを取得して配列に格納する
   auto jsonObjects = m_loadJson->GetMapObjects();
   auto commonObjects = m_loadJson->GetCommonMapObjects();
   std::move(commonObjects.begin(), commonObjects.end(), std::back_inserter(m_commonObjects));

   return jsonObjects;
}


/// <summary>
/// CSVファイルからオブジェクト生成
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
/// <param name="parameter">ステージのパラメータ</param>
/// <returns></returns>
std::vector<std::unique_ptr<IMapObject>> Map::LoadObjectsFromCsv(int stageNumber, GameParameters::StageParamger& parameter)
{
   //ロードCSVを生成
   m_loadCsv = std::make_unique<LoadCSV>(stageNumber);
   //パラメータからオブジェクトを生成する
   m_loadCsv->CraeteObject(parameter);
   //生成したオブジェクトを取得して配列に格納する
   m_map[stageNumber] = std::move(m_loadCsv->GetMapData());
   //接続ノード配列を取得して追加
   auto stageTransitions = m_loadCsv->GetStageTransition();
   m_stageTransition.insert(m_stageTransition.end(), std::make_move_iterator(stageTransitions.begin()), std::make_move_iterator(stageTransitions.end()));   
   return m_loadCsv->GetMapObjects();
}

/// <summary>
/// 境界ボックスを生成する
/// </summary>
void Map::SetCollider()
{
	m_allMapBoundingBoxes.clear();	   //境界ボックスの配列をクリア
	m_allMapBoundingSpheres.clear();   //境界球の配列をクリア
	m_allObjects.clear();			   //全てのオブジェクト配列をクリア

	//全てのオブジェクトを一つの配列にまとめる
	for (const auto& object : m_commonObjects)	//共通オブジェクト
	{
	   m_allObjects.push_back(object.get());
	}
	for (const auto& stages : m_stageObjects)	//ステージ別オブジェクト
	{
	   for (const auto& object : stages.second)
	   {
		  m_allObjects.push_back(object.get());
	   }
	}

	//境界ボックス配列を一つにまとめる
	for (const auto& stages : m_stageObjects)
	{
	   //現在いるステージ以外を処理しない
	   if (stages.first != m_commonResources->GetBlackBoard()->GetStageNumber()) continue;

	   for (const auto& objects : stages.second)
	   {
		 //コリジョンオブジェクトを取得する
		 CollisionObject* collisionObj = objects->GetCollisionObject();
		 if (!collisionObj) continue;

		 //衝突判定のパラメータを取得
		 const auto& param = collisionObj->GetCollisionParameter();
		 if (param.collision.collisionType < CollisionManager::CollisionType::PUSH) continue;
		 if (!param.collisionFlag) continue;

		 if (param.objectID == CollisionManager::ObjectType::FLOOR) continue;
		 //配列にまとめる
		 m_allMapBoundingBoxes.push_back(param.collision.box);
		 m_allMapBoundingSpheres.push_back(param.collision.sphere);
	   }
	}

	for (const auto& object : m_allObjects)
	{
	   //コリジョンオブジェクトを取得する
	   CollisionObject* collisionObj = object->GetCollisionObject();
	   if (!collisionObj) continue;

	   //衝突判定のパラメータを取得
	   const auto& param = collisionObj->GetCollisionParameter();
	   if (param.collision.collisionType < CollisionManager::CollisionType::PUSH) continue;
	   if (!param.collisionFlag) continue;

	   //お試し壁のみ登録
	   if (param.objectID != CollisionManager::ObjectType::WALL) continue;

	   m_shadowObjects.push_back(object);
	}
}

/// <summary>
/// クリアする
/// </summary>
void Map::Clear()
{
   //当たり判定を初期化
   m_allMapBoundingBoxes.clear();
   m_allMapBoundingSpheres.clear();
   m_stageObjects.clear();
   m_commonObjects.clear();
   m_allObjects.clear();
   m_gimmickObjects.clear();
   m_deviceObjects.clear();
}

/// <summary>
/// ギミックオブジェクト配列を作成
/// </summary>
void Map::RegisterGimmickObjects()
{
   for (const auto& objects : m_stageObjects)
   {
	  for (const auto& object : objects.second)
	  {
		 if (!object->AsGimmick()) continue;
		 auto* gimmick = dynamic_cast<GimmickObjectBase*>(object.get());
		 //ギミックオブジェクトを配列に格納
		 m_gimmickObjects.push_back(gimmick);
	  }
   }
}


/// <summary>
/// デバイスオブジェクト配列を作成
/// </summary>
void Map::RegisterDeviceObjects()
{
   for (const auto& objects : m_stageObjects)
   {
	  for (const auto& object : objects.second)
	  {
		 if (!object->AsDevice()) continue;
		 auto* device = dynamic_cast<DeviceObjectBase*>(object.get());
		 //デバイスオブジェクトを配列に格納
		 m_deviceObjects.push_back(device);
	  }
   }
}


//指定ステージの境界ボックスを取得する
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

