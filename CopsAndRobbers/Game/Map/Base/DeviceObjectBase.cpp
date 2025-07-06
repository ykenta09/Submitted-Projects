/*
	@file	DeviceObjectBase.cpp
	@brief	
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Map/Base/DeviceObjectBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialQuaternion">初期回転</param>
/// <param name="scale">スケール</param>
DeviceObjectBase::DeviceObjectBase(
   const DirectX::SimpleMath::Vector3& initialPosition,
   const DirectX::SimpleMath::Quaternion& initialQuaternion,
   const DirectX::SimpleMath::Vector3& scale)
   :
   m_isInsideControlZone(false),                         //装置の操作範囲にいるかのフラグを初期化
   m_isTriggered(false),                                 //装置の起動中フラグを初期化
   m_isEnabled(false),                                   //装置をOFFで初期化する
   m_graphics(yamadalib::Graphics::GetInstance())		 //グラフィックスのインスタンスを取得
{
   m_parameter.position = initialPosition;		   //オブジェクトの初期座標を設定
   m_parameter.rotation = initialQuaternion;	   //オブジェクトの初期回転を設定
   m_parameter.scale = scale;					   //オブジェクトのスケールを設定
}


/// <summary>
/// デストラクタ
/// </summary>
DeviceObjectBase::~DeviceObjectBase()
{
}


/// <summary>
/// キーボードの入力処理
/// </summary>
/// <param name="key"></param>
void DeviceObjectBase::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
   //装置の操作範囲外なら処理しない
   if (!m_isInsideControlZone)return;
   //装置が起動中は入力を受け付けない
   if (m_isTriggered) return;

   //登録したギミックを起動
   if (key == DirectX::Keyboard::E)
   {
      m_isTriggered = true;            //装置を起動中にする
      m_isEnabled = !m_isEnabled;      //装置のON・OFFを切り替える
   }
}

/// <summary>
/// ギミックオブジェクトを登録する
/// </summary>
/// <param name="gimmick">ギミックオブジェクト</param>
void DeviceObjectBase::RegisterGimmick(IGimmick* gimmick)
{
   m_targets.push_back(gimmick);
}


/// <summary>
/// ギミックを起動する
/// </summary>
/// <param name="flag">ギミックの機動フラグ</param>
void DeviceObjectBase::Trigger(bool flag)
{
   if (flag)//アクティブ
   {
      for (auto const target : m_targets)
      {
         //処理が終了していないギミックを処理する
         if(!target->IsFinished())  
            target->Active();
      }
   }
   else //非アクティブ
   {
      for (auto const target : m_targets)
      {
         //処理が終了していないギミックを処理する
         if (!target->IsFinished())
            target->Deactive();
      }
   }
}

/// <summary>
/// 登録されたギミック処理が終了したか
/// </summary>
void DeviceObjectBase::AreAllGimmicksFinished()
{
   for (const auto& target : m_targets)
   {
      if (!target->IsFinished())
      {
         m_isTriggered = true;
         return;
      }     
   }
   //終了フラグをリセットする
   GimmicksReset();
   m_isTriggered = false;
}


/// <summary>
/// 登録したギミックの終了フラグをリセットする
/// </summary>
void DeviceObjectBase::GimmicksReset()
{
   for (const auto& target : m_targets)
   {
      target->Reset();
   }
}
