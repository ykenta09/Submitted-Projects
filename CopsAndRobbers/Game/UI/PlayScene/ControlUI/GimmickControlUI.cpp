/*
	@file	ControlUI.cpp
	@brief	操作方法のUIクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Game/Screen.h"
#include "Game/UI/PlayScene/ControlUI/GimmickControlUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Map/Map.h"
#include "Game/Map/Base/DeviceObjectBase.h"


/// <summary>
/// コンストラクタ
/// </summary>
GimmickControlUI::GimmickControlUI()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_controlsPosition(DirectX::SimpleMath::Vector3::Zero),
    m_isVisible(false)
{
}


/// <summary>
/// デストラクタ
/// </summary>
GimmickControlUI::~GimmickControlUI()
{
}


/// <summary>
/// 初期化する
/// </summary>
void GimmickControlUI::Initialize()
{
	//UIテクスチャを読み込む
    LoadResources();
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="playTime"></param>
void GimmickControlUI::Update()
{
   m_isVisible = false;
   //デバイスオブジェクト配列を取得
   auto devices =  GameManager::GetInstance()->GetMap()->GetDeviceObjects();
   for (const auto& device : devices)
   {
	  //操作可能な装置があれば座標を取得
	  if (device->GetInsideControlZone())
	  {
		 m_controlsPosition = device->GetPosition();
		 m_isVisible = true;
	  }
   }
}


/// <summary>
/// 描画する
/// </summary>
void GimmickControlUI::Render()
{
   if (!m_isVisible) return;
   //操作方法UIを描画する
   ControlsUI();
}


/// <summary>
/// 後始末する
/// </summary>
void GimmickControlUI::Finalize()
{
	// do nothing.
}

/// <summary>
/// 操作方法のUI
/// </summary>
void GimmickControlUI::ControlsUI()
{
   //敵の行列を生成
   DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_controlsPosition + CONTROLS_OFFSET);
   //移動キーの描画
   m_graphics->DrawTextureBillboard(m_controlTexture.Get(), world, false);
}


/// <summary>
/// リソースを取得する
/// </summary>
void GimmickControlUI::LoadResources()
{
   m_controlTexture = yamadalib::Resources::GetInstance()->GetTexture("GimmickControl");
}
