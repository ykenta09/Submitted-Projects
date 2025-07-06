/*
	@file	CameraManager.h
	@brief  カメラのマネージャクラス
*/
#pragma once
#ifndef CAMERA_MANAGER_DEFINED
#define CAMERA_MANAGER_DEFINED

#include "Game/Interface/ICamera.h"
#include "Game/Camera/FixedCamera.h"
#include "Game/Camera/TopDownViewCamera.h"
#include "Game/Camera/TPSCamera.h"

// 前方宣言
class CommonResources;

class CameraManager
{
public:
   //現在のカメラのビュー行列を取得する
   DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_currentCamera->GetViewMatrix(); }
   //現在のプロジェクション行列を取得する
   DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return m_currentCamera->GetProjectionMatrix(); }
   //カメラ座標を取得する
   const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_currentCamera->GetEyePosition(); }

   //現在のカメラを取得する
   ICamera* GetCurrentCamera() const { return m_currentCamera; }
public:
   //コンストラクタ
   CameraManager();
   //デストラクタ
	~CameraManager();
	//更新する
	void Update(
	   const DirectX::SimpleMath::Vector3& newTarget = DirectX::SimpleMath::Vector3::Zero,
	   const DirectX::SimpleMath::Matrix& rotate = DirectX::SimpleMath::Matrix::Identity,
	   const std::vector<DirectX::BoundingBox>& boundingBoxes = std::vector<DirectX::BoundingBox>());
	//後始末をする
	void Finalize();
public:
   //現在のカメラをTPSカメラに切り替える
   void SetTPSCamera() { m_currentCamera = m_tpsCamera.get(); };
   //現在のカメラを見下ろしカメラに切り替える
   void SetTopDownViewCamera() { m_currentCamera = m_topDownViewCamera.get(); }
   //現在のカメラをTPSカメラに切り替える
   void SetFixedCamera() { m_currentCamera = m_fixedCamera.get(); }
private:
   ICamera* m_currentCamera;								 //現在アクティブなカメラ
   std::unique_ptr<TPSCamera> m_tpsCamera;					 //TPSカメラ
   std::unique_ptr<TopDownViewCamera> m_topDownViewCamera;   //見下ろしカメラ
   std::unique_ptr<FixedCamera> m_fixedCamera;				 //固定カメラ

};
#endif // CAMERA_MANAGER_DEFINED
