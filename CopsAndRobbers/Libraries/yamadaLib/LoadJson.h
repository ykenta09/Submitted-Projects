/*
	@file	LoadJson.h
	@brief	Jsonファイル読み込みクラス
*/
#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "Game/Interface/IMapObject.h"
#include "Game/Interface/IComponent.h"
#include "Libraries/yamadaLib/GameParameter.h"
#include "Game/CommonResources.h"

class Map;
class IGimmick;
class IDevice;

class LoadJson
{
public:
   //共通オブジェクトデータ配列を取得する
   std::vector<std::unique_ptr<IMapObject>> GetCommonMapObjects() { return std::move(m_commonMapObjects); }
   //オブジェクトデータ配列を取得する
   std::vector<std::unique_ptr<IMapObject>> GetMapObjects() { return std::move(m_mapObjects); }
   //キャラクターデータを取得する関数
   const std::unordered_map<std::string, std::vector<std::unique_ptr<IComponent>>>& GetCharacterData() const { return m_characterData; }

   //巡回ルートを取得する
   const std::vector< DirectX::SimpleMath::Vector3>& GetPatrolRootMap() { return m_patrolRootMap; }
   //隣接リストを取得する
   const std::unordered_map<int, GameParameters::PatrolRoute>& GetAdjacencyList() { return m_adjacencyList; }
   //敵の初期頂点番号を取得する
   int GetInitailVertex(const int& number) 
   { 
      if(m_initialVertexNumber.size() > 0){ return m_initialVertexNumber[number][0]; }
      else { return 0; }
   }

   void TransferCharactersTo(std::function<void(std::unique_ptr<IComponent>)> attachFunc)
   {
      for (auto& pair : m_characterData) 
      {
         for (auto& character : pair.second)
         {
            attachFunc(std::move(character));
         }
      }
      // すべてのキャラクターを移動させた後、m_characterDataをクリア
      m_characterData.clear();
   }

   //ステージデータを読み込む
   DirectX::SimpleMath::Vector3 LoadStageData(const int& stageNumber);

   //リソースを設定する
   void SetResources(CommonResources* resource) { m_commonResources = resource; }
public:
   //コンストラクタ
   LoadJson();
   LoadJson(CommonResources* resource, IComponent* parent, Map* map);
   //デストラクタ
   ~LoadJson();
   //Jsonデータを読み込む
   void LoadObject(const GameParameters::StageParamger& stageParam);
   //ファイル名からオブジェクトを読み込む
   void LoadObject(const std::string& filename);
   //キャラクターデータを読み込む
   void LoadCharacterData(const int& stageNumber);
   void LoadCharacterData(const std::string& filname);
   void ConvertJsonToCharacter(std::ifstream file, const int& stageNumber = 0);
   void LoadGraphData(const int& stageNumber);

   std::unordered_map<std::string, std::string> LoadResources(const std::string& typeName);

   //ギミックとデバイスを紐づけする
   void BindGimmicksToDevices();

   void ParseAndRegisterObjects(const std::string& jsonString, const GameParameters::StageParamger& stageParam);
private:
   //マップオブジェクト配列
   std::vector<std::unique_ptr<IMapObject>> m_mapObjects;
   std::vector<std::unique_ptr<IMapObject>> m_commonMapObjects;      //共通ステージオブジェクト

   //ギミックオブジェクト配列と装置の配列
   std::unordered_map<int, std::vector<IGimmick*>> m_gimmickObjects;
   std::unordered_map<int, std::vector<IDevice*>> m_deviceObjects;
   
   //キャラクターデータの配列 
   std::unordered_map<std::string, std::vector<std::unique_ptr<IComponent>>> m_characterData;
   std::unordered_map<std::string, GameParameters::CharacterData> m_character;

   //ステージのすべての巡回ルート
   std::vector<DirectX::SimpleMath::Vector3> m_patrolRootMap;
   //隣接リスト<敵の番号,頂点番号,繋がっている頂点番号>
   std::unordered_map<int, GameParameters::PatrolRoute> m_adjacencyList;
   //敵の初期頂点番号
   std::vector<std::vector<int>> m_initialVertexNumber;

   // 共通リソース
   CommonResources* m_commonResources;
   //親
   IComponent* m_parent;
   //マップ
   Map* m_map;
};
