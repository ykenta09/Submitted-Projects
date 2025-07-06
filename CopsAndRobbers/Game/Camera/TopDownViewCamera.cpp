/*
	@file	TopDownViewCamera.h
	@brief	見下ろしカメラクラス
*/
#include "pch.h"
#include "Game/Camera/TopDownViewCamera.h"
#include "Game/Screen.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="eye">カメラの座標</param>
/// <param name="target">カメラの注視点</param>
/// <param name="up">カメラの高さ</param>
TopDownViewCamera::TopDownViewCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& up)
	:
	m_view{},
	m_projection{},
	m_eye{ eye },
	m_target{ target },
	m_up{ up }
{
    //ビュー行列を計算する
	CalculateViewMatrix();
	//プロジェクション行列を計算する
	CalculateProjectionMatrix();
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="newTarget">カメラが注視するターゲット座標</param>
/// <param name="rotate">カメラの回転行列</param>
/// <param name="boundingBoxes">カメラの衝突判定境界ボックスの配列</param>
void TopDownViewCamera::Update(
   const DirectX::SimpleMath::Vector3& newTarget, 
   const DirectX::SimpleMath::Matrix& rotate, 
   const std::vector<DirectX::BoundingBox>& boundingBoxes)
{
   UNREFERENCED_PARAMETER(newTarget);
   UNREFERENCED_PARAMETER(rotate);
   UNREFERENCED_PARAMETER(boundingBoxes);

   // キーボードの状態を取得する
   m_keyboardState = DirectX::Keyboard::Get().GetState();
   //キーボードの状態を更新する
   m_keyboardTracker.Update(m_keyboardState);

   // カメラの座標移動
   if (m_keyboardState.W)
   {
	  m_eye += DirectX::SimpleMath::Vector3::Forward * CAMERA_MOVE_SPEED;
	  m_target += DirectX::SimpleMath::Vector3::Forward * CAMERA_MOVE_SPEED;
   }
   if (m_keyboardState.A)
   {
	  m_eye += DirectX::SimpleMath::Vector3::Left * CAMERA_MOVE_SPEED;
	  m_target += DirectX::SimpleMath::Vector3::Left * CAMERA_MOVE_SPEED;
   }
   if (m_keyboardState.S)
   {
	  m_eye += DirectX::SimpleMath::Vector3::Backward * CAMERA_MOVE_SPEED;
	  m_target += DirectX::SimpleMath::Vector3::Backward * CAMERA_MOVE_SPEED;
   }
   if (m_keyboardState.D)
   {
	  m_eye += DirectX::SimpleMath::Vector3::Right * CAMERA_MOVE_SPEED;
	  m_target += DirectX::SimpleMath::Vector3::Right * CAMERA_MOVE_SPEED;
   }

   //ビュー行列を計算する
   CalculateViewMatrix();
}

/// <summary>
/// ビュー行列を計算する
/// </summary>
void TopDownViewCamera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

/// <summary>
/// プロジェクション行列を計算する
/// </summary>
void TopDownViewCamera::CalculateProjectionMatrix()
{
	//ウィンドウサイズ
	const float width = static_cast<float>(Screen::WIDTH);
	const float height = static_cast<float>(Screen::HEIGHT);

	//画面縦横比
	const float aspectRatio = width / height;

	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView
	(
		FOV, aspectRatio, NEAR_PLANE, FAR_PLANE
	);
}

/// <summary>
/// カメラの座標を計算する
/// </summary>
/// <param name="rotate">カメラの回転行列</param>
void TopDownViewCamera::CalculateEyePosition(const DirectX::SimpleMath::Matrix& rotate)
{
	//既定の進行方向ベクトル
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Forward;

	// カメラがターゲットからどれくらい離れているか
	forward.y = CAMERA_HIGHT;
	forward.z = CAMERA_DISTANCE;

	// ターゲットの向いている方向に追従する
	forward = DirectX::SimpleMath::Vector3::Transform(forward, rotate);

	// カメラ座標を計算する
	m_eye = m_target + forward;
}

