/*
	@file	GameManager.cpp
	@brief	ゲームマネージャークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "FrustumCuller.h"


/// <summary>
/// 視錐台と境界ボックスの判定
/// </summary>
/// <param name="frustum">視錐台</param>
/// <param name="box">判定を行う境界ボックス</param>
/// <returns></returns>
bool FrustumCuller::IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingBox& box) const
{
   DirectX::BoundingBox worldAABB = box;
   return frustum.Contains(worldAABB) != DirectX::ContainmentType::DISJOINT;
}

/// <summary>
/// 視錐台と境界球の判定
/// </summary>
/// <param name="frustum"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool FrustumCuller::IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingSphere& sphere) const
{
   //境界球をワールド空間で使用
   DirectX::BoundingSphere worldSphere = sphere;

   //視錐台と境界球の交差判定
   return frustum.Contains(worldSphere) != DirectX::ContainmentType::DISJOINT;
}




/// <summary>
/// 視錐台を生成する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
/// <returns></returns>
DirectX::BoundingFrustum FrustumCuller::CreateViewFrustum(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
   //視錐台を宣言
   DirectX::BoundingFrustum frustum;
   // プロジェクション行列から視錐台を生成
   DirectX::BoundingFrustum::CreateFromMatrix(frustum, proj, true);
   // ビュー行列の逆行列で視錐台をワールド空間に変換
   frustum.Transform(frustum, view.Invert());
   // ワールド空間の視錐台を返す
   return frustum;
}

