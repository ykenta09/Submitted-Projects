/*
	@file	Map.h
	@brief	マップクラス
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

// 前方宣言
class CommonResources;
class IMapObject;
class GimmickObjectBase;
class DeviceObjectBase;

class Map
{
public:
	//バウンディングボックス配列を取得する
	const std::vector<DirectX::BoundingBox>& GetBoundingBoxes() const { return m_allMapBoundingBoxes; }
	//指定ステージの境界ボックスを取得する
	std::vector<DirectX::BoundingBox> GetStageBoundingBoxes(int stageNumber) const;
	//境界球配列を取得する
	const std::vector<DirectX::BoundingSphere>& GetBoundingSphere() const { return m_allMapBoundingSpheres; }
	const std::unordered_map<int, std::vector<std::unique_ptr<IMapObject>>>& GetMapObjects() const { return m_stageObjects; }
	//csvマップデータを取得する
	const std::vector<std::vector<GameParameters::CreateObjectParameter>> GetMapData(int stageNumber) { return m_map[stageNumber]; }
	//指定したステージの中心座標を取得する
	const DirectX::SimpleMath::Vector3& GetStageCenterPosition(int stageNumber) { return m_stagePositions[stageNumber]; }
	//接続ノード配列を取得する
	const std::vector<GameParameters::StageTransition> GetStageTransition() { return m_stageTransition; }

	//ギミックオブジェクト配列を取得する
	std::vector<GimmickObjectBase*> GetGimmickObjects() const { return m_gimmickObjects; }
	//デバイスオブジェクト配列を取得する
	std::vector<DeviceObjectBase*> GetDeviceObjects() const { return m_deviceObjects; }

	//天球の中心座標を設定する
	void SetCelestialSpherePosition(const DirectX::SimpleMath::Vector3& pos) { m_celestialSpherePosition = pos; }

	void ChangeStageNumber(const int& stageNumber) { m_stageNumber = stageNumber; }

	//デバック
	int GetDrawObject() { return m_drawObject; }

	// 特定ステージのオブジェクトをconst参照で取得
	const std::vector<std::unique_ptr<IMapObject>>& GetStageObjects(int stageID) const 
	{
	   static const std::vector<std::unique_ptr<IMapObject>> empty; // 見つからない場合用

	   if (stageID < 1) return empty;
	   auto it = m_stageObjects.find(stageID);
	   if (it != m_stageObjects.end())
	   {
		  return it->second;
	   }
	   return empty;
	}

	// 指定したタイプのオブジェクトを取得するテンプレートメソッド
   // 特定ステージのみ対象とするバージョン
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

	// 指定したタイプのオブジェクトを取得するテンプレートメソッド
   // 共通オブジェクトから取得する
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
    //コンストラクタ
	Map(CommonResources* resources);
	//デストラクタ
	~Map();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float elapsedTime);
	//描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	//後始末をする
	void Finalize();

	//ステージのデータを取得する
	void LoadStageData();

	//JSONファイルからオブジェクト生成
	std::vector<std::unique_ptr<IMapObject>> LoadObjectsFromJson(int stageNumber, GameParameters::StageParamger& parameter);
	//CSVファイルからオブジェクト生成
	std::vector<std::unique_ptr<IMapObject>> LoadObjectsFromCsv(int stageNumber, GameParameters::StageParamger& parameter);
	//当たり判定を設定する
	void SetCollider();
	//クリア
	void Clear();
private:
   //ギミックオブジェクト配列を作成
   void RegisterGimmickObjects();
   //デバイスオブジェクト配列を作成
   void RegisterDeviceObjects();
private:
	CommonResources* m_commonResources;	// 共通リソース
	yamadalib::Graphics* m_graphics;	//グラフィックス

	//マップオブジェクト
	std::unique_ptr<CelestialSphere> m_celestialSphere;	 //天球
	std::unique_ptr<LoadJson> m_loadJson;	 //Jsonファイルロード
	std::unique_ptr<LoadCSV> m_loadCsv;		 //csvファイルロード
	std::unique_ptr<FrustumCuller> m_frustum;	//視錐台カリング

	//マップ配列
	std::vector<std::unique_ptr<IMapObject>> m_mapObject;
	//csvのマップデータ配列
	std::unordered_map<int, std::vector<std::vector<GameParameters::CreateObjectParameter>>> m_map;
	//接続ノード配列
	std::vector<GameParameters::StageTransition> m_stageTransition;

	//全ステージデータを保持
	std::vector<IMapObject*> m_allObjects;													 //全てのマップオブジェクト
	std::unordered_map<int, std::vector<std::unique_ptr<IMapObject>>> m_stageObjects;		 //ステージオブジェクト
	std::vector<std::unique_ptr<IMapObject>> m_commonObjects;								 // 共通オブジェクト（ドアなど）
	std::vector<GimmickObjectBase*> m_gimmickObjects;										 //ギミックオブジェクト
	std::vector<DeviceObjectBase*> m_deviceObjects;											 //デバイスオブジェクト
	std::vector<IMapObject*> m_shadowObjects;												 //ポイントライトを遮るオブジェクト
	std::unordered_map<int, DirectX::SimpleMath::Vector3> m_stagePositions;					 //各ステージの中心座標
	DirectX::SimpleMath::Vector3 m_celestialSpherePosition;									 //天球の中心座標

	//全てのマップオブジェクトの境界ボックス配列
	std::vector<DirectX::BoundingBox> m_allMapBoundingBoxes;
	std::vector<DirectX::BoundingSphere> m_allMapBoundingSpheres;
	int m_stageNumber;	//ステージ番号
		 
	//キューブマップへの書き出しフラグ
	bool m_isDrawCubeMap;
	//デバック変数
	int m_drawObject;		  //描画しているオブジェクトの数を取得する
};
#endif // MAP_DEFINED
