/*
	@file	Fellow.h
	@brief	仲間クラス
*/
#ifndef FELLOW_DEFINED
#define FELLOW_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Character/Fellow/State/FellowIdle.h"
#include "Game/Character/Fellow/State/FellowChasing.h"
#include "Game/Character/Fellow/State/FellowRouteSearch.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"
#include "Game/Astar/MultiStageAStar.h"

#include "Game/Map/Map.h"

// 前方宣言
class CommonResources;

class Fellow : public CharacterBase
{
public:
   static constexpr float MOVE_SPEED = { 6.50f };			   //仲間の移動速度
   static constexpr float STOPPING_DISTANCE = { 2.3f };		   //仲間の追いかけるのをやめる距離
public:
    //初期座標を取得する
    DirectX::SimpleMath::Vector3 GetInitialPosition() { return m_initialPosition; }	
	//現在の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }		
	//現在の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_currentPosition = pos; }	

	//現在の回転角を取得する
	const float& GetAngle() { return m_currentAngle; };
	//現在の回転角を設定する
	void SetAngle(const float& angle) { m_currentAngle = angle; }	
	//スケール取得する
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }	
	//押し出し処理
	void SetPushPos(const DirectX::SimpleMath::Vector3& push) { m_currentPosition += push; }	
	//動くことができるかどうか
	bool GetFellowMove() { return m_fellow->GetFellowIdle()->GetFellowMove(); }

	//自身のポインタを取得する
	Fellow* AsFellow()  { return m_fellow; }
	//現在の状態を取得する
	IState* GetCurrentState() const { return m_currentState; }		
	//アイドル状態を取得する
	FellowIdle* GetFellowIdle() { return m_fellowIdle.get(); }				
	//探索状態を取得する
	FellowRouteSearch* GetFellowRouteSearch() const { return m_fellowSearch.get(); }	
	//追跡状態を取得する
	FellowChasing* GetFellowChasing() { return m_fellowChasing.get(); }		

	//ウェイポイントを取得する
	int GetWayPointIndex() const { return m_wayPointIndex; }							
	//ウェイポイントを設定する
	void SetWayPointIndex(const int& wayPointIndex) { m_wayPointIndex = wayPointIndex; }	

	//ターゲット座標を取得する
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_targetPosition; }

	// ステージ間経路をセット
	void SetMultiStagePath(const std::vector<GameParameters::MultiStageNode>& path) { m_multiStagePath = path; }
	const std::vector<GameParameters::MultiStageNode>& GetMultiStagePath() const { return m_multiStagePath; }
	//AStarを取得する
	MultiStageAStar* GetMultiStageAStar() const { return m_multiStageAStar.get(); }
public:
	//コンストラクタ
	Fellow(
	   IComponent* parent,									   //親
	   CommonResources* resources,							   //共通リソース
	   Map* map,											   //マップポインタ
	   const GameParameters::CharacterData data,
	   const int& stageNumber);			   //スケール
	//デストラクタ
	~Fellow();
	void Initialize();
	void Update(
		const float& elapsedTime,							//経過時間
		const DirectX::SimpleMath::Vector3& position,		//現在の座標
		const float& angle,									//現在の回転角
		const DirectX::SimpleMath::Vector3& target);		//目標の座標
	void Render();
	void Finalize();

	void InitializeCollision();											//当たり判定の初期化
	void HandleCollision(const CollisionObject* other);					//衝突時の処理
	void ChangeState(IState* newState);									//状態を変更する
	void PushPosition(const DirectX::SimpleMath::Vector3& position);	//押し出し処理
private:
   //モデル情報を適用する
   void ApplyModelAndTexture();
private:
    CommonResources* m_commonResources;					 // 共通リソース
	Fellow* m_fellow;	//自身のポインタ

	IState* m_currentState;								//現在の状態
	std::unique_ptr<FellowIdle> m_fellowIdle;			//仲間待機状態
	std::unique_ptr<FellowChasing> m_fellowChasing;		// 仲間追跡状態
	std::unique_ptr<FellowRouteSearch> m_fellowSearch;	//仲間の探索状態
	std::unique_ptr<MultiStageAStar> m_multiStageAStar;	//ASar

	std::vector<GameParameters::MultiStageNode> m_multiStagePath; // ステージ間対応の経路

	//衝突判定
	CollisionObject* m_collisionObject;	//衝突判定のパラメータ

	std::vector<int> m_shortestPath;	//最短経路の番号配列
	int m_wayPointIndex;	//ウェイポイント
	float m_searchTime;	//探索までの時間を管理

	Map* m_map;	//マップ
};
#endif // FELLOW_DEFINED
