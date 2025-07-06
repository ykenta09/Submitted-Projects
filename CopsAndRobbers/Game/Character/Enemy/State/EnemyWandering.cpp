/*
	@file	EnemyWandering.cpp
	@brief	敵の追跡クラス
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyWandering.h"
#include <random>
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵のポインタ</param>
EnemyWandering::EnemyWandering(Enemy* enemy)
	:
	m_enemy(enemy),
	m_patrolRouteMap{},					
	m_patrolAdjacencyList{},			
	m_passedRoutNumber{},
	m_graphScene{},
	m_enemyNumber( 0 ),
    m_currentRootNumber( 0 ),
    m_previousRootNumber( 0 ),
    m_moveProgress( 0 ),
    m_initalNumber(0),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_noFoundRoute( false ),
	m_wanderTarget(DirectX::SimpleMath::Vector3::Zero)
{
}


/// <summary>
/// デストラクタ
/// </summary>
EnemyWandering::~EnemyWandering()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void EnemyWandering::Initialize()
{	
	//巡回経路を取得する
	InitialPatrolRoute();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void EnemyWandering::Update(const float& elapsedTime)
{
	//巡回地点に移動完了したら巡回目標を更新する
	if (IsArriveDestination())
	{
		m_enemy->ChangeState(m_enemy->GetEnemyWaiting());
		UpDaatePatrolRoute();
	}
	else
	{
		Move(m_wanderTarget, elapsedTime);
		//移動割合の計算
		MovementRatio();
	}
}


/// <summary>
/// 描画する
/// </summary>
void EnemyWandering::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}


/// <summary>
/// 後始末する
/// </summary>
void EnemyWandering::Finalize()
{
	// do nothing.
}

/// <summary>
/// 受け取った地点に移動をする
/// </summary>
/// <param name="target">移動する目標地点の座標</param>
/// <param name="elpsedTime">経過時間</param>
void EnemyWandering::Move(const DirectX::SimpleMath::Vector3& target, const float& elpsedTime)
{
	// ターゲットまでの方向ベクトルを計算する
	DirectX::SimpleMath::Vector3 direction = target - m_enemy->GetPosition();
	// 一定の距離に近づくと止まる
	if (direction.Length() <= Enemy::STOPPING_DISTANCE) return;
	direction.Normalize();
	// 敵の速度を計算する
	DirectX::SimpleMath::Vector3 enemyVelocity = direction * Enemy::MOVE_SPEED * elpsedTime;
	// 敵の座標を更新する
	m_enemy->SetPosition(m_enemy->GetPosition() + enemyVelocity);

	// 敵の進行方向に向ける
	if (direction.LengthSquared() > 0.0f)  // 方向ベクトルがゼロでない場合
	{
	   // 新しい方向を計算する
		m_enemy->SetAngle(atan2(direction.x, direction.z));
	}
}


/// <summary>
/// 巡回経路を更新する
/// </summary>
void EnemyWandering::UpDaatePatrolRoute()
{
	//次の経路番号を選択するための候補リスト
	const std::vector<int> availableRoutes = m_patrolAdjacencyList[m_enemyNumber].adjacencyList[m_currentRootNumber];
	//検索した配列の要素番号
	int count = 0;
	
	//巡回する経路が見つからなかった場合
	if (!m_noFoundRoute)
	{
		//通る通路が存在する場合、通った経路を最初の要素に登録する
		m_passedRoutNumber.insert(m_passedRoutNumber.begin(), m_currentRootNumber);
		for (int route : availableRoutes)
		{
			//m_passedRoutNumbeに含まれていない番号を検索
			if (std::find(m_passedRoutNumber.begin(), m_passedRoutNumber.end(), route) == m_passedRoutNumber.end())
			{
			    //前回の経路番号を更新する
			    m_previousRootNumber = m_currentRootNumber;
				//次の経路番号を隣接リストから取得する
			    m_currentRootNumber = m_patrolAdjacencyList[m_enemyNumber].adjacencyList[m_currentRootNumber][count];
				//移動目標地点を設定する
				m_wanderTarget = m_patrolRouteMap[m_currentRootNumber];
				//移動方向の回転を計算する
				DirectX::SimpleMath::Vector3 direction = m_wanderTarget - m_enemy->GetPosition();
				//次の移動方向の角度を設定する
				m_enemy->SetNextAngle(std::atan2(direction.x, direction.z));
				//目標地点が見つかったら処理を終了
				return;
			}
			count++;
		}
	}
	//巡回する経路が存在しなかったら、通った通路を巡回する
	m_noFoundRoute = true;
	if (!m_passedRoutNumber.empty())
	{
	    m_currentRootNumber = m_passedRoutNumber.front();
		m_wanderTarget = m_patrolRouteMap[m_currentRootNumber];
		m_passedRoutNumber.erase(m_passedRoutNumber.begin());
	}
	else
	{
		m_noFoundRoute = false;
	}
}


/// <summary>
/// 巡回経路の目標地点に到着したか
/// </summary>
/// <returns></returns>
bool EnemyWandering::IsArriveDestination()
{
	DirectX::SimpleMath::Vector3 patrolPosition = DirectX::SimpleMath::Vector3(m_wanderTarget.x, roundf(m_enemy->GetPosition().y), m_wanderTarget.z);
	// patrolPosition と enemyPosition の距離を計算
	float distance = DirectX::SimpleMath::Vector3::Distance(patrolPosition, m_enemy->GetPosition());

	// 距離が 1 以下かどうかをチェック
	if (distance <= 1.5f)
		return true;

	return false;
}


/// <summary>
/// 初期座標、巡回ルートを初期化する
/// </summary>
void EnemyWandering::InitialPatrolRoute()
{
   //巡回マップがない場合は処理しない
   if (m_patrolRouteMap.size() < 1) return;

	m_wanderTarget = m_patrolRouteMap[m_initalNumber];		   //初期移動先を取得する
	m_currentRootNumber = m_initalNumber;					   //初期頂点番号を設定する
	m_previousRootNumber = m_currentRootNumber;				   //移動目標の頂点番号を移動前の頂点番号に設定する
}


/// <summary>
/// 移動割合を計算する
/// </summary>
void EnemyWandering::MovementRatio()
{
   //巡回開始地点と目標地点
   const DirectX::SimpleMath::Vector3 startPosition = m_patrolRouteMap[m_previousRootNumber];
   const DirectX::SimpleMath::Vector3 endPosition = m_wanderTarget;

   // 開始地点から目標地点までの全距離
   float totalDistance = DirectX::SimpleMath::Vector3::Distance(startPosition, endPosition);
   if (totalDistance <= 0.0f) // 距離がゼロの場合、割合は0
	  return;

   //開始地点から目標地点までの全距離
   float totalDistatnce = DirectX::SimpleMath::Vector3::Distance(startPosition, endPosition);

   //開始地点から現在位置までの距離
   float travelDistance = DirectX::SimpleMath::Vector3::Distance(startPosition, m_enemy->GetPosition());

   //移動割合を計算
   m_moveProgress = travelDistance / totalDistatnce;
}


/// <summary>
/// グラフのデータを受け取る
/// </summary>
/// <param name="patrolMap">巡回マップ</param>
/// <param name="adjacenyList">隣接リスト</param>
/// <param name="initialNumber">初期頂点番号</param>
void EnemyWandering::SetGraphDate(
   std::vector<DirectX::SimpleMath::Vector3>& patrolMap,
   std::unordered_map<int, GameParameters::PatrolRoute> adjacenyList,
   int initialNumber)
{
   m_enemyNumber = m_enemy->GetEnemyNumber();
   m_patrolRouteMap = patrolMap;
   m_patrolAdjacencyList = adjacenyList;
   m_initalNumber = initialNumber;
}

