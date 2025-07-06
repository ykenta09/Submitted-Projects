/*
	@file	Enemy.h
	@brief	敵クラス
*/
#ifndef ENEMY_DEFINED
#define ENEMY_DEFINED
#pragma once
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Enemy/State/EnemyChasing.h"
#include "Game/Character/Enemy/State/EnemyWaiting.h"
#include "Game/Character/Enemy/State/EnemyWandering.h"
#include "Game/Character/Enemy/State/EnemyWanderingReturn.h"
#include "Game/Character/Enemy/State/EnemyTransmission.h"
#include "Game/Character/Enemy/EnemyVisibility.h"
#include "Game/Character/Enemy/State/EnemyRouteSearch.h"
#include "Game/Character/Enemy/State/EnemyTitle.h"
#include "Game/Astar/AStar.h"
#include "Game/Map/Map.h"
#include <unordered_map>
#include "Game/Astar/MultiStageAStar.h"
#include <GraphEditor/Graph.h>

// 前方宣言
class CommonResources;
class GraphScene;
class EnemyVisibility;

class Enemy : public CharacterBase
{
public:
   static constexpr float MOVE_SPEED = { 6.60f };			//敵の移動速度
   static constexpr float STOPPING_DISTANCE = { 1.5f };		//敵の追いかけるのをやめる距離
   static constexpr float WANDER_INTERVAL = { 3.0f };		//巡回ルート変える間隔
   static constexpr float WAIT_INTERVAL = { 2.0f };			//待機時間
   static constexpr float TIME_TO_SEARCH = { 0.5f };		//経路探索までの時間
public:
   //現在の座標を取得する
   DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }	
   //現在の座標を設定する
   void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_currentPosition = pos; }   

   //敵の初期回転角を取得する
   const float& GetInitialAngle() { return m_initialAngle; }   

   //敵の回転角を取得する
   const float& GetAngle() { return m_currentAngle; }	
   //敵の回転角を設定する
   const void SetAngle(const float angle) { m_currentAngle = angle; }  
   //敵の次の移動方向を設定する
   const void SetNextAngle(const float angle) { m_nextAngle = angle; } 
   //敵の次の移動方向を取得する
   const float& GetNextAngle() { return m_nextAngle; }					

   //押し出し処理
   void SetPushPos(const DirectX::SimpleMath::Vector3& push) { m_currentPosition += push;}   

   //初期座標を取得する
   DirectX::SimpleMath::Vector3 GetInitialPosition() { return m_initialPosition; }   

   //敵番号を設定する
   void SetEnemyNumber(const int& number) { m_enemyNumber = number; }   
   //敵番号を取得する
   const int& GetEnemyNumber() { return m_enemyNumber; }				

   //ターゲットの方向を取得する
   DirectX::SimpleMath::Vector3 GetTargetDirection() { return m_targetDirection; }

   //最短経路をクリアする
   void ResetShortestPath() { m_shortestPath.clear(); }				
   //最短経路を取得する
   std::vector<int> GetShortestPath() const { return m_shortestPath; }   
   //最短経路を設定する
   void SetShortestPath(const std::vector<int>& shortestPath) { m_shortestPath = shortestPath; }  
   // ウェイポイントを取得する
   int GetWayPointIndex() const { return m_wayPointIndex; }				
   // ウェイポイントを設定する
   void SetWayPointIndex(const int& wayPointIndex) { m_wayPointIndex = wayPointIndex; }   

   // ステージ間経路をセット
   void SetMultiStagePath(const std::vector<GameParameters::MultiStageNode>& path) { m_multiStagePath = path; }
   const std::vector<GameParameters::MultiStageNode>& GetMultiStagePath() const { return m_multiStagePath; }
   //AStarを取得する
   MultiStageAStar* GetMultiStageAStar() const { return m_multiStageAStar.get(); }


   //現在の状態を取得する
   IState* GetCurrentState() const { return m_currentState; }					
   //敵の徘徊状態を取得する
   EnemyWandering* GetEnemyWandering() const { return m_enemyWandering.get(); }					    
   //敵の徘徊に戻る状態を取得する
   EnemyWanderingReturn* GetEnemyWanderingReturn() const { return m_enemyWanderingReturn.get(); }   
   //敵の追跡状態を取得する
   EnemyChasing* GetEnemyChasing() const { return m_enemyChasing.get(); }						   
   //敵の待機状態を取得する
   EnemyWaiting* GetEnemyWaiting() const { return m_enemyWaiting.get(); }					
   //敵の経路探索状態を取得する
   EnemyRouteSearch* GetEnemyRouteSearch() const { return m_enemyRouteSearch.get(); }	
   //敵のタイトル状態を取得する
   EnemyTitle* GetEnemyTitle() const { return m_enemyTitle.get(); }								   
   //自身を取得する
   Enemy* AsEnemy()  { return m_enemy; }														  
public:
	//キーボードの通知をする
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys);
	void PushPosition(const DirectX::SimpleMath::Vector3& position);
public:
	//コンストラクタ
	Enemy(
		IComponent* parent,						
		CommonResources* resources,
	    Map* map,
	    const GameParameters::EnemyData data,
	    const int& stageNumber);
	//デストラクタ
	~Enemy();

	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target);
	//描画する
	void Render();
	//後始末する
	void Finalize();
	//当たり判定の初期化
	void InitializeCollision();
	//衝突時の処理
	void HandleCollision(const CollisionObject* other);
	//視野内か
	bool InView(bool object);
	//レイとマップオブジェクトの判定
	bool RayMapObject();
	//状態を変更する
	void ChangeState(IState* newState);
	//索敵
	void Search();
private:
   //モデル情報を適用する
   void ApplyModelAndTexture();
private:
   // === ステート管理  ===
	Enemy* m_enemy;													//自身のポインタ
	IState* m_currentState;											//現在の状態
	std::unique_ptr<EnemyWaiting> m_enemyWaiting;					//敵の待機状態
	std::unique_ptr<EnemyWandering> m_enemyWandering;				//敵の徘徊状態
	std::unique_ptr<EnemyWanderingReturn> m_enemyWanderingReturn;	//徘徊に戻る状態
	std::unique_ptr<EnemyChasing> m_enemyChasing;					//敵の追跡状態
	std::unique_ptr<EnemyRouteSearch> m_enemyRouteSearch;			//敵の経路探索状態
	std::unique_ptr<EnemyVisibility> m_visibility;				    //敵の視界
	std::unique_ptr<EnemyTitle> m_enemyTitle;					    //敵のタイトル状態
	std::unique_ptr<MultiStageAStar> m_multiStageAStar;			    //ASar

	// === 経路探索システム ===
	Map* m_map;															   //マップ
	std::vector<int> m_shortestPath;									   //最短経路
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRootMap;			   //ステージのすべての巡回ルート
	std::unordered_map<int, GameParameters::PatrolRoute> m_adjacencyList;  //隣接リスト<敵の番号,頂点番号,繋がっている頂点番号>
	int m_wayPointIndex;												   // ウェイポイントインデックス
	std::vector<GameParameters::MultiStageNode> m_multiStagePath; // ステージ間対応の経路

	CollisionObject* m_collisionObject;	//当たり判定のパラメータ

	// === 視界 ===
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_detectionTexture;	// 発見テクスチャ
	bool m_viewType;													    //視界のタイプ

	// === 方向 ===
	DirectX::SimpleMath::Vector3 m_targetDirection;	//ターゲットまでの方向ベクトル
	DirectX::SimpleMath::Vector3 m_enemyDirection;	//敵の向いている方向

	// === 設定パラメータ ===
	int m_enemyNumber;			 //敵番号
	int m_initialVertexNumber;	 //敵の初期頂点番号
	float m_nextAngle;			 //次のフレームで向く角度
	float m_searchTime;			 //探索までの時間を管理
};
#endif // ENEMY_DEFINED
