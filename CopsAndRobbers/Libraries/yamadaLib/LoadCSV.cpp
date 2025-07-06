/*
	@file	LoadCSV.h
	@brief	CSV管理クラス
*/
#include "pch.h"
#include "LoadCSV.h"
#include <fstream>
#include "Game/Factory/MapObjectFactory.h"

#include <nlohmann/json.hpp>


/// <summary>
/// コンストラクタ
/// </summary>
LoadCSV::LoadCSV()
{
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
LoadCSV::LoadCSV(const int stageNumber)
{
   Load("Resources\\MapData\\csv\\Stage" + std::to_string(stageNumber) + ".csv", stageNumber);
}


/// <summary>
/// デストラクタ
/// </summary>
LoadCSV::~LoadCSV()
{
}

//csvファイルを読み込む
void LoadCSV::Load(const std::string& filename, const int& stageNumber)
{
   std::string str;
   // テキストファイルを開く
   std::ifstream fin(filename);

   m_width = 80;
   m_height = 80;

   // 配列をクリアする
   m_map.clear();
   m_map.resize(m_height, std::vector<GameParameters::CreateObjectParameter>(m_width, { 1, 0, false }));

   if (!fin.is_open())  return;

   // ファイルを読み込む
   for (int i = 0; i < m_height; i++)
   {
	  if (!std::getline(fin, str)) break;

	  for (int j = 0; j < m_width; j++)
	  {
		 int cellStartIndex = (j * 8) + j;
		 if (cellStartIndex >= str.size())
		 {
			continue;
		 } 

		 std::string cell = str.substr(cellStartIndex, 8);
		 ParseCell(cell, m_map[i][j], stageNumber, j, i);
	  }
   }
   // ファイルを閉じる
   fin.close();
}


/// <summary>
/// セルデータを解析する関数
/// </summary>
/// <param name="cell">セル</param>
/// <param name="param">オブジェクトパラメータ</param>
void LoadCSV::ParseCell(const std::string& cell, GameParameters::CreateObjectParameter& param, const int& stageNumber, const int& fromWidth, const int& fromHeight)
{ 
   // 無効なデータは無視
   if (cell.size() != 8) return;

   // オブジェクトIDとモデル番号を設定
   param.objectID = std::stoi(cell.substr(0, 1));      // 最初の1文字がobjectID
   param.modelNum = std::stoi(cell.substr(1, 2));      // 次の2文字がmodelNumber
   param.visited = false;
   if(std::stoi(cell.substr(3, 1)) == 0) return;

   //接続ノード配列を作成する
   GameParameters::StageTransition transition;
   transition.toStage = std::stoi(cell.substr(3, 1));
   //移動先ノードの番号を計算する
   int height = std::stoi(cell.substr(6, 2));
   int width = std::stoi(cell.substr(4, 2));
   int toNodeNumber = m_width * height + width;
   transition.toNode = toNodeNumber;
   int fromNodeNumber = m_width * fromHeight + fromWidth;
   transition.fromNode = fromNodeNumber;
   transition.fromStage = stageNumber;
   m_stageTransitions.push_back(transition);
}


/// <summary>
/// オブジェクトを生成する
/// </summary>
/// <param name="stageParam">ステージパラメータ</param>
void LoadCSV::CraeteObject(const GameParameters::StageParamger& stageParam)
{
   // 壁（objectID=2, 高さ=10.0f）
   CreateObjectsByType(stageParam, 2, 10.0f, 0.0f);

   // visitedフラグをリセット
   ResetVisited();

   // 床（objectID=1, 高さ=1.0f）
   CreateObjectsByType(stageParam, 1, 0.5f, -0.5f);
}


/// <summary>
/// オブジェクトのタイプごとに生成
/// </summary>
/// <param name="stageParam">ステージsパラメータ</param>
/// <param name="targetObjectID">オブジェクトID</param>
/// <param name="objectHeight">オブジェクトの高さ</param>
/// <param name="yOffset">y軸の座標調整</param>
void LoadCSV::CreateObjectsByType(const GameParameters::StageParamger& stageParam, int targetObjectID, float objectHeight, float yOffset)
{
   for (int i = 0; i < m_height; i++)
   {
      for (int j = 0; j < m_width; j++)
      {
         if (m_map[i][j].visited || m_map[i][j].objectID != targetObjectID)
            continue;

         int targetModelNumber = m_map[i][j].modelNum;
         int width = 0, height = 0;

         // 横方向
         for (int x = j; x < m_width; ++x)
         {
            if (m_map[i][x].objectID == targetObjectID &&
               m_map[i][x].modelNum == targetModelNumber &&
               !m_map[i][x].visited)
            {
               width++;
            }
            else break;
         }
         // 縦方向
         bool isColumnValid = true;
         for (int y = i; y < m_height && isColumnValid; y++)
         {
            for (int x = j; x < j + width; x++)
            {
               if (m_map[y][x].objectID != targetObjectID ||
                  m_map[y][x].modelNum != targetModelNumber ||
                  m_map[y][x].visited)
               {
                  isColumnValid = false;
                  break;
               }
            }
            if (isColumnValid) height++;
         }
         // visitedマーク
         for (int y = i; y < i + height; ++y)
            for (int x = j; x < j + width; ++x)
               m_map[y][x].visited = true;

         // 生成
         float centerX = j + (width - 1) / 2.0f - (m_width / 2.0f);
         float centerZ = i + (height - 1) / 2.0f - (m_height / 2.0f);
         DirectX::SimpleMath::Vector3 pos = { centerX, yOffset, centerZ };
         DirectX::SimpleMath::Vector3 scl = { static_cast<float>(width), objectHeight, static_cast<float>(height) };
         auto object = MapObjectFactory::CreateObject(stageParam, targetObjectID, targetModelNumber, pos, scl);
         if (object) m_mapObjects.push_back(std::move(object));
      }
   }
}


/// <summary>
/// visitedフラグをリセット
/// </summary>
void LoadCSV::ResetVisited()
{
   for (int i = 0; i < m_height; ++i)
      for (int j = 0; j < m_width; ++j)
         m_map[i][j].visited = false;
}
