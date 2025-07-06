/*
	@file	EnemyTransmission.cpp
	@brief	敵の伝達クラス
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyTransmission.h"
#include <random>
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


//敵の伝達距離
const float EnemyTransmission::TRANSMISSION_DISTANCE = 5.0f;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
EnemyTransmission::EnemyTransmission(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphScene{},
	m_enemyMatrix( DirectX::SimpleMath::Matrix::Identity ),
	m_graphics(yamadalib::Graphics::GetInstance())
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
EnemyTransmission::~EnemyTransmission()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void EnemyTransmission::Initialize()
{	
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void EnemyTransmission::Update(const float& elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
   Transmission();
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void EnemyTransmission::Render()
{

#ifdef _DEBUG

#endif // _DEBUG
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void EnemyTransmission::Finalize()
{
	// do nothing.
}

//周りに伝達する
void EnemyTransmission::Transmission()
{
}
