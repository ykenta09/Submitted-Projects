/*
	@file	TopDownViewCamera.h
	@brief	上からの見下ろしカメラクラス
*/
#pragma once
#include "Game/Interface/ICamera.h"

class TopDownViewCamera : public ICamera
{
private:
   static constexpr float CAMERA_DISTANCE = 0.0f;	      //ターゲットからのデフォルトの距離
   static constexpr float CAMERA_HIGHT = 20.0f;			 //ターゲットからのデフォルトの高さ
   static constexpr float FOV = DirectX::XMConvertToRadians(45.0f);	        //画角
   static constexpr float NEAR_PLANE = 1.0f;	         //近い投影面
   static constexpr float FAR_PLANE = 1000.0f;	         //遠い投影面
   static constexpr float CAMERA_MOVE_SPEED = 0.3f;	     //カメラの移動速度
public:
   //ビュー行列を取得する
   const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
   //プロジェクション行列を取得する
   const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }
   //カメラ座標を取得する
   const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }
   //注視点を取得する
   const DirectX::SimpleMath::Vector3& GetTargetPostion() const { return m_target; }
   //カメラの頭の方向を取得する
   const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; }
public:
   //コンストラクタ
   TopDownViewCamera(
	  const DirectX::SimpleMath::Vector3& eye = DirectX::SimpleMath::Vector3(0.0f, 40.0f, 15.0f),
	  const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
	  const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY
   );
   //デストラクタ
   ~TopDownViewCamera() = default;

   //更新処理
   void Update(
	  const DirectX::SimpleMath::Vector3& newTarget = DirectX::SimpleMath::Vector3::Zero,
	  const DirectX::SimpleMath::Matrix& rotate = DirectX::SimpleMath::Matrix::Identity,
	  const std::vector<DirectX::BoundingBox>& boundingBoxes = std::vector<DirectX::BoundingBox>());

private:
   // ビュー行列を計算する
   void CalculateViewMatrix();
   //プロジェクション行列を計算する
   void CalculateProjectionMatrix();
   //カメラの座標を計算する
   void CalculateEyePosition(const DirectX::SimpleMath::Matrix& rotate);
private:
   DirectX::SimpleMath::Matrix m_view;		    //ビュー行列
   DirectX::SimpleMath::Matrix m_projection;	//プロジェクション行列
   DirectX::SimpleMath::Vector3 m_eye;			//カメラ座標
   DirectX::SimpleMath::Vector3 m_target;		//注視点
   DirectX::SimpleMath::Vector3 m_up;			//カメラの頭の方向

   float m_distance;		  //カメラの距離
   float m_prevScrollValue;   //マウスホイールの１フレーム前のスクロール値

   DirectX::Keyboard::State m_keyboardState;				    // キーボードステート
   DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;   //キーボードトラッカー
};

