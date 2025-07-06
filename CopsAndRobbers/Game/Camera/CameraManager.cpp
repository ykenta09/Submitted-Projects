/*
	@file	CameraManager.cpp
	@brief	カメラマネージャクラス
*/
#include "pch.h"
#include "CameraManager.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>

/// <summary>
/// コンストラクタ
/// 複数のカメラ(TPSカメラ、トップダウンビューカメラ、固定カメラ)を生成
/// </summary>
CameraManager::CameraManager()
	:
   m_currentCamera{}
{
   m_tpsCamera = std::make_unique<TPSCamera>();
   m_topDownViewCamera = std::make_unique<TopDownViewCamera>();
   m_fixedCamera = std::make_unique<FixedCamera>(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 60.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 40.0f));
   //初期カメラ
   m_currentCamera = m_tpsCamera.get();
}

/// <summary>
/// デストラクタ
/// ソースの後始末を行う
/// </summary>
CameraManager::~CameraManager()
{
	Finalize();
}

/// <summary>
/// カメラの状態を更新する
/// </summary>
/// <param name="newTarget">カメラが注視するターゲット座標</param>
/// <param name="rotate">カメラの回転行列</param>
/// <param name="boundingBoxes">カメラの衝突判定境界ボックスの配列</param>
void CameraManager::Update(
   const DirectX::SimpleMath::Vector3& newTarget,
   const DirectX::SimpleMath::Matrix& rotate,
   const std::vector<DirectX::BoundingBox>& boundingBoxes)
{
   m_currentCamera->Update(newTarget, rotate, boundingBoxes);
}

/// <summary>
/// リソースの解放・後始末をする
/// </summary>
void CameraManager::Finalize()
{
}
