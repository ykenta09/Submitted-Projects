/*
	@file	MouseControl.cpp
	@brief	マウス制御クラス
*/
#include "pch.h"
#include "MouseControl.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include "Libraries/yamadaLib/Math.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="commonResources"></param>
MouseControl::MouseControl(CommonResources* commonResources)
	:
	m_commonResources{ commonResources },
    m_yaw{},
    m_pitch{},
	m_mouseMatrix{ DirectX::SimpleMath::Matrix::Identity }
{
}

/// <summary>
/// デストラクタ
/// </summary>
MouseControl::~MouseControl()
{
	// do nothing.
}

/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
void MouseControl::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	//回転角を初期化する
	m_yaw = 0.0f;
	m_pitch = 0.0f;
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void MouseControl::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	//マウスの状態を取得する
	m_mouseState = m_commonResources->GetInputManager()->GetMouseState();

	//クォータニオンの計算をする
	QuaternionCalculation();
}


/// <summary>
/// クォータニオンの計算をする
/// </summary>
void MouseControl::QuaternionCalculation()
{
	// マウスの相対移動量を取得して回転を更新する
	float deltaYaw = DirectX::XMConvertToRadians(static_cast<float>(-m_mouseState.x)) * 0.08f;
	float deltaPitch = DirectX::XMConvertToRadians(static_cast<float>(-m_mouseState.y)) * 0.03f;

	//キー操作
	OnKeyPressed();

	// マウス座標から現在の角度を更新
	m_yaw += deltaYaw;
	m_pitch += deltaPitch;
	//Y軸の回転に制限を課す
	m_pitch = yamadalib::Math::Clamp(m_pitch, -0.3f, 0.3f);

	// クォータニオンを生成する
	DirectX::SimpleMath::Quaternion pitchQuat = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, m_pitch);
	DirectX::SimpleMath::Quaternion yawQuat = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, m_yaw);

	// クォータニオンを結合して行列に変換
	DirectX::SimpleMath::Quaternion combinedQuat = pitchQuat * yawQuat;
	m_mouseMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(combinedQuat);
}

/// <summary>
/// キーボード操作
/// </summary>
void MouseControl::OnKeyPressed()
{
   //キーボードの状態を取得する
   m_keyboardState = DirectX::Keyboard::Get().GetState();
   if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Left)) //左を向く
   {
	  m_yaw += 0.06f;
   }
   else if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Right))  //右を向く
   {
	  m_yaw -= 0.06f;
   }
   if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Up))	//上を向く
   {
	  m_pitch += 0.03f;
   }
   else if (m_keyboardState.IsKeyDown(DirectX::Keyboard::Down))	  //下を向く
   {
	  m_pitch -= 0.03f;
   }
}
