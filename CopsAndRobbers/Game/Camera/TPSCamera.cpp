/*
	@file	TPSCamera.h
	@brief	TPSカメラクラス
*/
#include "pch.h"
#include "Game/Camera/TPSCamera.h"
#include "Game/Screen.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="target">カメラの注視点</param>
TPSCamera::TPSCamera(const DirectX::SimpleMath::Vector3& target)
	:
	m_view{},
	m_projection{},
	m_eye{},
	m_target{ target },
	m_up{ DirectX::SimpleMath::Vector3::UnitY }
{
    //カメラの座標を計算する
	CalculateEyePosition(DirectX::SimpleMath::Matrix::Identity);
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
void TPSCamera::Update(
   const DirectX::SimpleMath::Vector3& newTarget,
   const DirectX::SimpleMath::Matrix& rotate,
   const std::vector<DirectX::BoundingBox>& boundingBoxes)
{
	//ターゲットを更新する
    m_target += (newTarget - m_target) * 0.3f;
    //カメラ座標を計算する
	CalculateEyePosition(rotate);
	// 障害物との衝突を処理する
	HandleCameraCollision(boundingBoxes);
	//ビュー行列を更新する
	CalculateViewMatrix();
}


/// <summary>
/// ビュー行列を計算する
/// </summary>
void TPSCamera::CalculateViewMatrix()
{
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

/// <summary>
/// プロジェクション行列を計算する
/// </summary>
void TPSCamera::CalculateProjectionMatrix()
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
void TPSCamera::CalculateEyePosition(const DirectX::SimpleMath::Matrix& rotate)
{
	//既定の進行方向ベクトル
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Forward;

	// カメラがターゲットからどれくらい離れているか
	forward.y = CAMERA_HIGHT;
	forward.z = CAMERA_DISTANCE;

	// ターゲットの向いている方向に追従する
	forward = DirectX::SimpleMath::Vector3::Transform(forward, rotate);

	//カメラ座標を計算する
	m_eye = m_target + forward;
}


/// <summary>
/// マップの壁との当たり判定
/// </summary>
/// <param name="boundingBoxes">衝突判定境界ボックスるの配列</param>
void TPSCamera::HandleCameraCollision(const std::vector<DirectX::BoundingBox>& boundingBoxes)
{
   // カメラの視点からターゲット方向へのレイを作成
   DirectX::SimpleMath::Ray cameraRay(m_target, m_eye - m_target);
   cameraRay.direction.Normalize(); // 方向を正規化

   float closestHitDistance = FLT_MAX; // 最も近い障害物までの距離
   bool hasHit = false;                // 衝突判定フラグ
   const float maxRayDistance = 8.0f;  // レイの最大長さ
   // 各壁との衝突判定を実行
   for (const auto& boundingBox : boundingBoxes) // 各要素を取得
   {
	  float distanceToObstacle;
	  // レイと障害物の衝突判定
	  if (boundingBox.Intersects(cameraRay.position, cameraRay.direction, distanceToObstacle))
	  {
		 // 最も近い障害物までの距離を更新
		 if (distanceToObstacle < closestHitDistance && distanceToObstacle <= maxRayDistance)
		 {
			closestHitDistance = distanceToObstacle;
			hasHit = true;
		 }
	  }
   }

   // 衝突があった場合、カメラ位置を調整
   if (hasHit)
   {
	  // 近すぎないように調整（0.1fはオフセット、0.5fは最小距離）
	  float adjustedDistance = std::max(closestHitDistance - 0.1f, 0.5f);
	  // カメラの新しい位置を計算（レイの最大距離内に制限）
	  m_eye = cameraRay.position + cameraRay.direction * std::min(adjustedDistance, maxRayDistance);
   }
}

