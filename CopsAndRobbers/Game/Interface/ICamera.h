/*
	@file	ICamera.h
	@brief	カメラのインターフェイスクラス
*/
#pragma once
#include <SimpleMath.h>
#ifndef ICAMERA_DEFINED
#define ICAMERA_DEFINED

// 前方宣言
class CommonResources;

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual void Update(
	   const DirectX::SimpleMath::Vector3& newTarget = DirectX::SimpleMath::Vector3::Zero,
	   const DirectX::SimpleMath::Matrix& rotate = DirectX::SimpleMath::Matrix::Identity,
	   const std::vector<DirectX::BoundingBox>& boundingBoxes = std::vector<DirectX::BoundingBox>()) = 0;

	//ビュー行列を取得する
	virtual const DirectX::SimpleMath::Matrix& GetViewMatrix() const = 0;
	//プロジェクション行列を取得する
	virtual const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const = 0;
	//カメラ座標を取得する
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const = 0;

};
#endif // ICAMERA_DEFINED
