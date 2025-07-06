/*
	@file	FrustumCuller.h
	@brief	視錐台カリングクラス
*/
#pragma once
#ifndef FRUSTUM_CULLER_DEFIEND
#define FRUSTUM_CULLER_DEFIEND
#include <pch.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>

class FrustumCuller
{
public:
   FrustumCuller() = default;

   // 境界ボックスが視錐台内にあるか判定
   bool IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingBox& box) const;

   bool IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingSphere& sphere) const;

   //ビュー射影行列から視錐台を生成
   DirectX::BoundingFrustum CreateViewFrustum(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
};
#endif // FRUSTUM_CULLER_DEFIEND