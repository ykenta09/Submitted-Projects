/*
	@file	LoadJson.h
	@brief	Jsonファイル読み込みクラス
*/
#include "pch.h"
#include "LoadJson.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "Game/Factory/MapObjectFactory.h"
#include "Game/Factory/CharacterFactory.h"
#include "Game/Interface/IGimmick.h"
#include "Game/Interface/IDevice.h"


/// <summary>
/// コンストラクタ
/// </summary>
LoadJson::LoadJson()
{
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resource">共通リソース</param>
/// <param name="parent">親のポインタ</param>
/// <param name="map">マップ</param>
LoadJson::LoadJson(CommonResources* resource, IComponent* parent, Map* map)
{
   m_commonResources = resource;
   m_parent = parent;
   m_map = map;
}

/// <summary>
/// デストラクタ
/// </summary>
LoadJson::~LoadJson()
{
}

/// <summary>
/// ステージの座標を読み込む
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
/// <returns></returns>
DirectX::SimpleMath::Vector3 LoadJson::LoadStageData(const int& stageNumber)
{
   //ファイルパスを設定する
   const std::string& filename = "Resources\\MapData\\json\\StagesData.json";
   // ファイルを開く
   std::ifstream file(filename);
   if (!file.is_open())
   {
      // ファイルが開けませんでした
      return DirectX::SimpleMath::Vector3::Zero;
   }

   nlohmann::json jsonData;
   file >> jsonData;
   file.close();

   DirectX::SimpleMath::Vector3 stagesPosition;

   for (const auto& stage : jsonData["stages"].items())
   {
      const auto& objects = stage.value();  // 値を取得
      int number = objects.value("number", 1);
      if(stageNumber != number) continue;
      stagesPosition = { objects["position"]["x"], objects["position"]["y"], objects["position"]["z"] };
      break;
   }
   return stagesPosition;
}

/// <summary>
/// Jsonファイルからマップオブジェクトを読み込む
/// </summary>
/// <param name="stageParam">ステージパラメータ</param>
void LoadJson::LoadObject(const GameParameters::StageParamger& stageParam)
{
   //ファイルパスを設定する
   const std::string& filename = "Resources\\MapData\\json\\Stage" + std::to_string(stageParam.objectNumber) + ".json";
   // ファイルを開く
   std::ifstream file(filename);
   if (!file.is_open())
   {
      // ファイルが開けませんでした
      return;
   }

   std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
   file.close();

   // JSONデータをパースしてオブジェクトに変換
   ParseAndRegisterObjects(jsonString, stageParam);
   //ギミックと装置の紐づけ処理
   BindGimmicksToDevices();
}


void LoadJson::LoadObject(const std::string& filename)
{
   // ファイルを開く
   std::ifstream file(filename);
   if (!file.is_open())
   {
      // ファイルが開けませんでした
      return;
   }

   std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
   file.close();

   //ステージ番号のないオブジェクト
   GameParameters::StageParamger parameter;
   parameter.stagePos = DirectX::SimpleMath::Vector3::Zero;
   parameter.objectNumber = 0;

   // JSONデータをパースしてオブジェクトに変換
   ParseAndRegisterObjects(jsonString, parameter);
   //ギミックと装置の紐づけ処理
   BindGimmicksToDevices();
}


/// <summary>
/// ステージ番号から対応したステージのデータを取得する
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
void LoadJson::LoadCharacterData(const int& stageNumber)
{
   //ファイルパスを設定する
   const std::string& filename = "Resources\\MapData\\json\\Stage" + std::to_string(stageNumber) + ".json";

   // ファイルを開く
   std::ifstream file(filename);
   if (!file.is_open())
   {
      // ファイルが開けませんでした
      return;
   }

   nlohmann::json jsonData;
   file >> jsonData;
   file.close();

   //配列のリセット
   m_characterData.clear();
   m_character.clear();

   // JSONデータをパースしてキャラクターデータに変換
   for (const auto& chara : jsonData["character"].items())
   {
      const std::string& characterType = chara.key();  // キャラクタータイプを取得
      const auto& characterArray = chara.value();  // キャラクターデータを取得

      for (const auto& character : characterArray)
      {
         //キャラクターデータの設定
         GameParameters::CharacterData charData;
         charData.name = characterType;
         charData.position = { character["position"]["x"].get<float>(), character["position"]["y"].get<float>(), character["position"]["z"].get<float>() };
         charData.rotation = character["rotation"].get<float>();
         charData.scale = { character["scale"]["x"].get<float>(), character["scale"]["y"].get<float>(), character["scale"]["z"].get<float>() };
         charData.stageParam.objectNumber = stageNumber;
         if (charData.name == "Enemy")
         {
            int enemyNumber = character["enemyNumber"].get<int>() - 1;
            m_characterData[characterType].emplace_back(CharacterFactory::CreateEnemy(m_parent, m_commonResources, enemyNumber, m_map, charData, stageNumber, this));
         }
         else
         {
            m_character[characterType] = charData;
         }
      }
   }

   // m_characterDataを使ってキャラクターを生成する処理
   for (const auto& pair : m_character)
   {
      const GameParameters::CharacterData& charData = pair.second;
      std::unique_ptr<IComponent> character;
      //敵は選択したステージ以外も生成
      if (charData.name == "Enemy")
      {
         character = CharacterFactory::CreateCharacter(m_parent, m_commonResources, m_map, charData, stageNumber, this);
      }
      else//プレイヤーと仲間は選択したステージのみ生成
      {
         if (stageNumber == m_commonResources->GetBlackBoard()->GetStageNumber())
         {
            character = CharacterFactory::CreateCharacter(m_parent, m_commonResources, m_map, charData, stageNumber, this);
         }
      }
      if (character)
      {
         m_characterData[charData.name].push_back(std::move(character));
      }
      else
      {
         // 不明なキャラクタータイプ
         continue;
      }
   }
}

/// <summary>
/// ファイルパスからデータを取得する
/// </summary>
/// <param name="filename">ファイルパス</param>
void LoadJson::LoadCharacterData(const std::string& filename)
{
   std::ifstream file(filename);
   if (!file.is_open())
   {
      return;
   }

   nlohmann::json data;
   file >> data;
   file.close();

   //データがない場合は終了
   if (data.is_null()) return;

   // JSONデータをパースしてキャラクターデータに変換
   for (const auto& chara : data["character"].items())
   {
      const std::string& characterType = chara.key();  // キャラクタータイプを取得
      const auto& characterArray = chara.value();  // キャラクターデータを取得

      for (const auto& character : characterArray)
      {
         //キャラクターデータを設定する
         GameParameters::CharacterData charData;
         charData.name = characterType;
         charData.position = { character["position"]["x"].get<float>(), character["position"]["y"].get<float>(), character["position"]["z"].get<float>() };
         charData.rotation = character["rotation"].get<float>();
         charData.scale = { character["scale"]["x"].get<float>(), character["scale"]["y"].get<float>(), character["scale"]["z"].get<float>() };
         charData.stageParam.objectNumber = 0;
         if (charData.name == "Enemy")
         {
            int enemyNumber = character["enemyNumber"].get<int>();
            m_characterData[characterType].emplace_back(CharacterFactory::CreateEnemy(m_parent, m_commonResources, enemyNumber, m_map, charData, 0, this));
         }
         else
         {
            m_character[characterType] = charData;
         }
      }
   }

   // m_characterDataを使ってキャラクターを生成する処理
   for (const auto& pair : m_character)
   {
      const GameParameters::CharacterData& charData = pair.second;
      auto character = CharacterFactory::CreateCharacter(m_parent, m_commonResources, m_map, charData, 0, this);
      if (character)
      {
         m_characterData[charData.name].push_back(std::move(character));
      }
      else
      {
         // 不明なキャラクタータイプ
         continue;
      }
   }

}

/// <summary>
/// JSONデータをバースしてキャラクターデータを変換
/// </summary>
/// <param name="file">ファイルパス</param>
/// <param name="stageNumber">ステージ番号</param>
void LoadJson::ConvertJsonToCharacter(std::ifstream file, const int& stageNumber)
{
   nlohmann::json data;
   file >> data;
   file.close();

   //データがない場合は終了
   if (data.is_null()) return;

   // JSONデータをパースしてキャラクターデータに変換
   for (const auto& chara : data["character"].items())
   {
      const std::string& characterType = chara.key();  // キャラクタータイプを取得
      const auto& characterArray = chara.value();  // キャラクターデータを取得

      for (const auto& character : characterArray)
      {
         //キャラクターデータを設定する
         GameParameters::CharacterData charData;
         charData.name = characterType;
         charData.position = { character["position"]["x"].get<float>(), character["position"]["y"].get<float>(), character["position"]["z"].get<float>() };
         charData.rotation = character["rotation"].get<float>();
         charData.scale = { character["scale"]["x"].get<float>(), character["scale"]["y"].get<float>(), character["scale"]["z"].get<float>() };
         charData.stageParam.objectNumber = stageNumber;
         if (charData.name == "Enemy")
         {
            int enemyNumber = character["enemyNumber"].get<int>() - 1;
            m_characterData[characterType].emplace_back(CharacterFactory::CreateEnemy(m_parent, m_commonResources, enemyNumber, m_map, charData, stageNumber, this));
         }
         else
         {
            m_character[characterType] = charData;
         }
      }
   }

   // m_characterDataを使ってキャラクターを生成する処理
   for (const auto& pair : m_character)
   {
      const GameParameters::CharacterData& charData = pair.second;
      auto character = CharacterFactory::CreateCharacter(m_parent, m_commonResources, m_map, charData, stageNumber, this);
      if (character)
      {
         m_characterData[charData.name].push_back(std::move(character));
      }
      else
      {
         // 不明なキャラクタータイプ
         continue;
      }
   }
}

/// <summary>
/// 敵の巡回ルートを取得する
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
void LoadJson::LoadGraphData(const int& stageNumber)
{
   //ファイルパスを設定する
   const std::string& filename = "Resources\\EnemyRouteData\\Stage" + std::to_string(stageNumber) + "PatrolRoot.json";

   // ファイルを開く
   std::ifstream file(filename);
   if (!file.is_open())
   {
      // ファイルが開けませんでした
      return;
   }

   nlohmann::json jsonData;
   file >> jsonData;
   file.close();

   // 一時データをクリア
   m_patrolRootMap.clear();
   m_adjacencyList.clear();
   m_initialVertexNumber.clear();

   // 巡回ルートを読み込む
   if (jsonData.contains("rootList"))
   {
      std::vector<DirectX::SimpleMath::Vector3> patrolRootMap;
      for (const auto& root : jsonData["rootList"])
      {
         float x = root["position"]["x"].get<float>();
         float y = root["position"]["y"].get<float>();
         float z = root["position"]["z"].get<float>();
         m_patrolRootMap.emplace_back(DirectX::SimpleMath::Vector3(x, y, z));
      }
   }

   // 敵の初期位置と隣接リストを読み込む
   if (jsonData.contains("adjacency_list"))
   {
      for (const auto& enemy : jsonData["adjacency_list"])
      {
         int enemyNumber = enemy["enemy_number"].get<int>();
         int initialNumber = enemy["intial_number"].get<int>();

         // 初期頂点番号の保存
         if (m_initialVertexNumber.size() <= enemyNumber)
         {
            m_initialVertexNumber.resize(enemyNumber + 1);
         }
         m_initialVertexNumber[enemyNumber].resize(1);
         m_initialVertexNumber[enemyNumber][0] = initialNumber;

         //隣接リスト
         GameParameters::PatrolRoute patrolRoute;
         if (enemy.contains("connected_vertices"))
         {
            for (const auto& vertexList : enemy["connected_vertices"])
            {
               std::vector<int> edges;
               for (const auto& v : vertexList)
               {
                  edges.push_back(v.get<int>());
               }
               patrolRoute.adjacencyList.push_back(edges);
            }
         }
         m_adjacencyList[enemyNumber] = patrolRoute;
      }
   }
   return;
}


/// <summary>
/// リソースをロードする
/// </summary>
/// <param name="name">読み込むリソース</param>
/// <returns></returns>
std::unordered_map<std::string, std::string> LoadJson::LoadResources(const std::string& typeName)
{
   //ファイルパスを設定
   const std::string& filename = "Resources\\GameData\\ResourcesList.json";

   std::unordered_map<std::string, std::string> result;

   // ファイルを開く
   std::ifstream file(filename);
   if (!file.is_open())
   {
      // ファイルが開けませんでした
      return result;
   }

   nlohmann::json jsonData;
   file >> jsonData;
   file.close();

   // "Texture"セクションから直接nameでアクセス
   if (jsonData["Texture"] == typeName)
   {
      for (auto it = jsonData["Texture"].begin(); it != jsonData["Texture"].end(); it++)
      {
         result.emplace(it.key(), it.value());
      };
   }
   if (jsonData["Model"] == typeName)
   {
      for (auto it = jsonData["Model"].begin(); it != jsonData["Model"].end(); it++)
      {
         result.emplace(it.key(), it.value());
      };
   }
   if (jsonData.contains(typeName))
   {
      for (auto it = jsonData[typeName].begin(); it != jsonData[typeName].end(); it++)
      {
         result.emplace(it.key(), it.value());
      };
   }

   return result;
}

/// <summary>
/// ギミックとデバイスの紐づけ
/// </summary>
void LoadJson::BindGimmicksToDevices()
{
   for (const auto& devicePair : m_deviceObjects)
   {
      int id = devicePair.first;
      auto gimmickIt = m_gimmickObjects.find(id);
      const auto& devices = devicePair.second;

      if (gimmickIt != m_gimmickObjects.end())
      {
         const auto& gimmicks = gimmickIt->second;

         for (const auto& device : devices)
         {
            for (const auto& gimmick : gimmicks)
            {
               device->RegisterGimmick(gimmick);
            }
         }
      }
   }
}

void LoadJson::ParseAndRegisterObjects(const std::string& jsonString, const GameParameters::StageParamger& stageParam)
{
   nlohmann::json jsonData = nlohmann::json::parse(jsonString);

   for (const auto& obj : jsonData["objects"].items())
   {
      const std::string& objectType = obj.key();
      const auto& objects = obj.value();

      for (const auto& objInstance : objects)
      {
         // ファクトリーでオブジェクトを生成
         auto mapObject = MapObjectFactory::CreateObject(m_commonResources, stageParam, objectType, objInstance);
         if (!mapObject) continue;

         // ギミックと装置を配列に格納
         if (objInstance.contains("gimmickID"))
         {
            auto gimmick = mapObject->AsGimmick();
            int gimmickID = objInstance.value("gimmickID", 1);
            m_gimmickObjects[gimmickID].push_back(gimmick);
         }
         if (objInstance.contains("leverID"))
         {
            auto device = mapObject->AsDevice();
            int deviceID = objInstance.value("leverID", 1);
            m_deviceObjects[deviceID].push_back(device);
         }

         if (mapObject.get()->GetType() != IMapObject::Type::DOOR)
         {
            m_mapObjects.push_back(std::move(mapObject));
         }
         else
         {
            m_commonMapObjects.push_back(std::move(mapObject));
         }
      }
   }
}
