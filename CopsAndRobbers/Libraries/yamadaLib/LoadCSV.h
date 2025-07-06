/*
	@file	LoadCSV.h
	@brief	csvファイル読み込みクラス
*/
#pragma once
#include <unordered_map>
#include "Game/Interface/IMapObject.h"
#include "Libraries/yamadaLib/GameParameter.h"

class LoadCSV
{
public:
   //オブジェクトデータ配列を取得する
   std::vector<std::unique_ptr<IMapObject>> GetMapObjects() { return std::move(m_mapObjects); }
   //マップデータ配列を取得する
   std::vector<std::vector<GameParameters::CreateObjectParameter>> GetMapData() { return std::move(m_map); }
   //接続ノード配列を取得する
   std::vector<GameParameters::StageTransition> GetStageTransition() { return std::move(m_stageTransitions); }
public:
   LoadCSV();
   LoadCSV(const int stageNumber);
   ~LoadCSV();
   //オブジェクトを生成する
   void CraeteObject(const GameParameters::StageParamger& stageParam);

   //オブジェクトのタイプごとに生成
   void CreateObjectsByType(const GameParameters::StageParamger& stageParam, int targetObjectID, float objectHeight, float yOffset);

   // visitedフラグをリセット
   void ResetVisited();

private:
   //csvデータを読み込む
   void Load(const std::string& filename, const int& stageNumber);
   void ParseCell(const std::string& cell, GameParameters::CreateObjectParameter& param, const int& stageNumber, const int& fromWidth, const int& fromheight);
private:
   //マップオブジェクト配列
   std::vector<std::unique_ptr<IMapObject>> m_mapObjects;
   std::vector<std::vector<GameParameters::CreateObjectParameter>> m_map;
   //接続ノード配列
   std::vector<GameParameters::StageTransition> m_stageTransitions;
   //マップサイズ
   int m_width;
   int m_height;
};
