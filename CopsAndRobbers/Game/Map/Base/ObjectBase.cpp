/*
	@file	ObjectBase.cpp
	@brief	
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Base/ObjectBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialQuaternion">初期回転</param>
/// <param name="scale">スケール</param>
ObjectBase::ObjectBase(
   const DirectX::SimpleMath::Vector3& initialPosition,
   const DirectX::SimpleMath::Quaternion& initialQuaternion,
   const DirectX::SimpleMath::Vector3& scale)
   :
   m_graphics(yamadalib::Graphics::GetInstance())		 //グラフィックスのインスタンスを取得
{
   m_parameter.position = initialPosition;		   //オブジェクトの初期座標を設定
   m_parameter.rotation = initialQuaternion;	   //オブジェクトの初期回転を設定
   m_parameter.scale = scale;					   //オブジェクトのスケールを設定
}


/// <summary>
/// デストラクタ
/// </summary>
ObjectBase::~ObjectBase()
{
}

