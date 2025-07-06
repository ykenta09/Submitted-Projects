/*
	@file	GameParameter.h
	@brief	ゲームパラメーター
*/
#pragma once
#include <unordered_map>

namespace GameParameters
{
   static constexpr int MIN_STAGE_NUM = 1;		   //最小ステージ数
   static constexpr int MAX_STAGE_NUM = 3;		   //最大ステージ数

   //ステージパラメータ
   struct StageParamger
   {
	  int objectNumber;							   //オブジェクトがあるステージ番号
	  DirectX::SimpleMath::Vector3 stagePos;	   //ステージの座標
   };

   //キャラクターのタイプ
   enum class CharacterType
   {
	  Player = 0,
	  Fellow,
	  Enemy,
   };

   //オブジェクトのパラメータ
   struct ObjectParameter
   {
	  DirectX::SimpleMath::Vector3 initialPos;			 //オブジェクトの初期座標
	  DirectX::SimpleMath::Quaternion initialRot;		 //オブジェクトの初期回転
	  DirectX::SimpleMath::Vector3 scale;				 //スケール
	  StageParamger stageParam;							 //ステージのパラメータ
   };

   struct PatrolRoute    //各敵の巡回ルート
   {
	  std::vector<std::vector<int>> adjacencyList;
   };


   //csvマップのオブジェクト生成時のパラメータ
   struct CreateObjectParameter
   {
	  int objectID;	   //オブジェクトID
	  int modelNum;	   //モデル番号
	  bool visited;	   //訪問フラグ
   };

   //接続ノードの構造体
   struct StageTransition
   {
	  int fromStage = 0;    //移動元のステージ番号
	  int fromNode = 0;     //移動元ステージの出口ノード頂点番号
	  int toStage = 0;      //移動先のステージ番号
	  int toNode = 0;       //移動先ステージの入口ノード頂点番号
   };

   struct MultiStageNode
   {
	  int stage = 0;
	  int node = 0;
	  // 経路復元用
	  int parentStage = -1;
	  int parentNode = -1;
   };


   // キャラクターデータを格納する構造体
   struct CharacterData
   {
	  std::string name;		  
	  DirectX::SimpleMath::Vector3 position;
	  float rotation;
	  DirectX::SimpleMath::Vector3 scale;
	  StageParamger stageParam;
   };

   //敵データを格納する構造体
   struct EnemyData
   {
	  CharacterData charaData;			  //キャラクターデータ
	  int enemyNumber;					  //敵の番号
	  std::vector<DirectX::SimpleMath::Vector3> patrolRouteMap;		//巡回頂点マップ
	  std::unordered_map<int, PatrolRoute> patrolAdjacencyList;	//巡回マップの隣接リスト
	  int initalNumber;				   //初期地点の頂点番号
   };
}

