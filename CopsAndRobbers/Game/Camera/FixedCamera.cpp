/*
	@file	FixedCamera.h
	@brief	固定カメラクラス
*/
#include "pch.h"
#include "FixedCamera.h"
#include "Game/Screen.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="eye">カメラ座標</param>
/// <param name="target">カメラの注視点</param>
/// <param name="up">カメラの高さ</param>
FixedCamera::FixedCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& up)
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
void FixedCamera::Update(const DirectX::SimpleMath::Vector3& newTarget, const DirectX::SimpleMath::Matrix& rotate, const std::vector<DirectX::BoundingBox>& boundingBoxes)
{
   UNREFERENCED_PARAMETER(boundingBoxes);
   //ターゲットを更新する
   m_target = newTarget;
   //カメラの座標を計算する
   CalculateEyePosition(rotate);
   //ビュー行列を更新する
   CalculateViewMatrix();
}

/// <summary>
/// ビュー行列を計算する
/// </summary>
void FixedCamera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

/// <summary>
/// プロジェクション行列を計算する
/// </summary>
void FixedCamera::CalculateProjectionMatrix()
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
/// カメラ座標を計算する
/// </summary>
/// <param name="rotate">カメラの回転行列</param>
void FixedCamera::CalculateEyePosition(const DirectX::SimpleMath::Matrix& rotate)
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

