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


//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
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
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_noFoundRoute( false ),
	m_wanderTarget(DirectX::SimpleMath::Vector3::Zero)
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyWandering::~EnemyWandering()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void EnemyWandering::Initialize()
{	
	//巡回経路を取得する
	InitialPatrolRoute();
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
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

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void EnemyWandering::Render()
{

#ifdef _DEBUG

#endif // _DEBUG
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void EnemyWandering::Finalize()
{
	// do nothing.
}

//受け取った地点に移動をする
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


//巡回経路を更新する
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
				//direction.Normalize();
				//m_enemy->SetAngle(atan2(direction.x, direction.z));
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

//巡回経路の目標地点に到着したか
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

//初期座標、巡回ルートを初期化する
void EnemyWandering::InitialPatrolRoute()
{
   if (m_graphScene == nullptr) return;
	m_patrolRouteMap = m_graphScene->GetPatrolRootMap();					//経路マップの座標を取得する
	m_patrolAdjacencyList = m_graphScene->GetAdjacencyList();				//巡回隣接リストを取得する

	m_initalNumber = m_graphScene->GetInitalVertexNumbeer(m_enemyNumber);		//敵番号に対応した初期座標の頂点番号を取得する
	m_wanderTarget = m_patrolRouteMap[m_initalNumber];							//初期移動先を取得する
	m_currentRootNumber = m_initalNumber;
	m_previousRootNumber = m_currentRootNumber;
}

//移動割合を計算する
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

//グラフのデータを受け取る
void EnemyWandering::SetGraphDate(GraphScene* graph, int number)
{
	m_graphScene = graph;
	m_enemyNumber = number;
}

