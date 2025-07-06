/*
	@file	FrustumCuller.h
	@brief	������J�����O�N���X
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

   // ���E�{�b�N�X����������ɂ��邩����
   bool IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingBox& box) const;

   bool IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingSphere& sphere) const;

   //�r���[�ˉe�s�񂩂王����𐶐�
   DirectX::BoundingFrustum CreateViewFrustum(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
};
#endif // FRUSTUM_CULLER_DEFIEND