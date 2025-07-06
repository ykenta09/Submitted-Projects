/*
	@file	FellowIdle.cpp
	@brief	プレイヤーの待機状態クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Fellow/State/FellowIdle.h"
#include "Game/Map/Map.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fellow">仲間本体のポインタ</param>
FellowIdle::FellowIdle(Fellow* fellow, Map* map)
	:
	m_fellow(fellow),
    m_map(map),
    m_isMove(false)
{
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
FellowIdle::~FellowIdle()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void FellowIdle::Initialize()
{
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void FellowIdle::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	//動ける状態なら探索する
	if (m_isMove)
	{
	   //最初の索敵
	   m_fellow->ChangeState(m_fellow->GetFellowRouteSearch());
	   m_fellow->GetFellowRouteSearch()->SetSearchTime(0);
	   m_fellow->GetFellowRouteSearch()->SetAstar(m_fellow->GetTargetPosition());
	}
	else
	{
	   MoveFellow();
	}
}

/// <summary>
/// 描画する
/// </summary>
void FellowIdle::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}

/// <summary>
/// 後始末する
/// </summary>
void FellowIdle::Finalize()
{
}


/// 牢屋が空いたら追跡状態に変更
/// </summary>
void FellowIdle::MoveFellow()
{
   //修正箇所
   auto prisons = m_map->GetObjectsByType<Prison>(m_fellow->GetObjectStageNumber());
   for (auto* prison : prisons)
   {
	  if (prison->GetOpenEndPrison())
	  {
		 m_isMove = true;
	  }
   }
}
