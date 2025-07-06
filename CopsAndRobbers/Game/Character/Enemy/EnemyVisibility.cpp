/*
	@file	EnemyVisibility.cpp
	@brief	敵の視界クラス
*/
#include "pch.h"
#include "Game/Character/Enemy/EnemyVisibility.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Map/Map.h"
#include <cassert>
#include <Game/Screen.h>
#include <Libraries/MyLib/Geometry.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="enemy">敵本体のポインタ</param>
EnemyVisibility::EnemyVisibility(Enemy* enemy)
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_enemy(enemy)
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyVisibility::~EnemyVisibility()
{
	// do nothing.
}

/// <summary>
/// 初期化する
/// </summary>
void EnemyVisibility::Initialize()
{
   m_vectorV2 = DirectX::SimpleMath::Vector2(0, 0);
   m_vectorV3 = DirectX::SimpleMath::Vector2(0, 0);
   m_length = VISIBILITIY_DISTANCE;
}

/// <summary>
///  更新する
/// </summary>
void EnemyVisibility::Update()
{
   m_vectorV1 = DirectX::SimpleMath::Vector2(m_enemy->GetPosition().x, m_enemy->GetPosition().z);
}

/// <summary>
/// 視界を描画する
/// </summary>
/// <param name="map">マップのポインタ</param>
void EnemyVisibility::DrawVisibility(Map* map)
{
   if (map == nullptr) return;

   //描画処理をバッチ処理でまとめて処理
   m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
      [this, map](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
      {
         //敵の正面の角度を取得
         m_angle = DirectX::XMConvertToDegrees(-m_enemy->GetAngle());
         //扇形の色
         const DirectX::XMVECTOR SemiTransparentRed = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.1f);

         for (int i = 0; i < SEGMENTS; i++)
         {
            float currentAngleStart, currentAngleEnd;
            // 視界の角度計算
            CalculateAngles(i, currentAngleStart, currentAngleEnd);

            //正面方向の視界の長さベクトルを取得する
            DirectX::SimpleMath::Vector2 direction = CalculateDirection();
            
            // 視界のベクトル計算
            CalculateVectors(currentAngleStart, currentAngleEnd, direction, m_vectorV2, m_vectorV3);

            //障害物との交差判定用フラグ
            bool pointFlag1 = false, pointFlag2 = false;
            // 三角形の頂点設定
            SetTriangleVertices(map, m_vectorV2, m_vectorV3, pointFlag1, pointFlag2);

            // 視界の扇型を描画
            DrawSector(SemiTransparentRed);
         }
      });
}


/// <summary>
/// 視界の角度計算
/// </summary>
/// <param name="segmentIndex">セグメント番号</param>
/// <param name="currentAngleStart">セグメント開始角度</param>
/// <param name="currentAngleEnd">セグメント終了角度</param>
void EnemyVisibility::CalculateAngles(int segmentIndex, float& currentAngleStart, float& currentAngleEnd)
{
   //１セグメントあたりの角度幅を計算
   float angleStep = 60.0f / SEGMENTS;
   //セグメントの始点角度を計算
   currentAngleStart = m_angle - (FIELD_OF_VIEW / 2.0f) + segmentIndex * angleStep;
   //セグメントの終点角度を計算
   currentAngleEnd = m_angle - (FIELD_OF_VIEW / 2.0f) + (segmentIndex + 2) * angleStep;
   m_angle += angleStep;
}


/// <summary>
/// 視界の向きと大きさの計算
/// </summary>
/// <returns></returns>
DirectX::SimpleMath::Vector2 EnemyVisibility::CalculateDirection()
{
   DirectX::SimpleMath::Vector2 direction(0.0f, 1.0f);
   //視界の長さだけスケーリング
   direction *= m_length;
   //回転行列で回して、実際の視界扇形の各辺の向きを決める
   return direction;
}

/// <summary>
/// 視界のベクトル計算
/// </summary>
/// <param name="currentAngleStart">セグメントの開始角度</param>
/// <param name="currentAngleEnd">セグメントの終了角度</param>
/// <param name="direction">正面方向</param>
/// <param name="vectorV2">扇形の左端</param>
/// <param name="vectorV3">扇形の右端</param>
void EnemyVisibility::CalculateVectors(float currentAngleStart, float currentAngleEnd, const DirectX::SimpleMath::Vector2& direction, DirectX::SimpleMath::Vector2& vectorV2, DirectX::SimpleMath::Vector2& vectorV3)
{
   //開始角度文だけZ軸周りに回転する行列を生成
   DirectX::SimpleMath::Matrix rotation1 = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(currentAngleStart));
   //終了角度分だけZ軸周りに回転する行列を生成
   DirectX::SimpleMath::Matrix rotation2 = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(currentAngleEnd));

   //正面ベクトルを開始角度分だけ回転し、敵の現在位置を原点として加算
   //扇形の左端のワールド座標を求める
   vectorV2 = DirectX::SimpleMath::Vector2::Transform(direction, rotation1) + m_vectorV1;
   //正面ベクトルを終了角度分だけ回転し、現在位置を原点として加算
   //扇形の右端のワールド座標を求める
   vectorV3 = DirectX::SimpleMath::Vector2::Transform(direction, rotation2) + m_vectorV1;
}

/// <summary>
///  線分V1-V2, V1-V3を描画
/// </summary>
/// <param name="vectorV2">扇形の左端の2D座標</param>
/// <param name="vectorV3">扇形の右端の2D座標</param>
void EnemyVisibility::DrawViewLines(const DirectX::SimpleMath::Vector2& vectorV2, const DirectX::SimpleMath::Vector2& vectorV3)
{
   //敵の座標から扇形左端への線分を描画
   m_graphics->DrawLine(m_vectorV1, vectorV2 - m_vectorV1, DirectX::Colors::White);
   //敵の座標から扇形右端への線分を描画
   m_graphics->DrawLine(m_vectorV1, vectorV3 - m_vectorV1, DirectX::Colors::White);
}

/// <summary>
/// 三角形の頂点設定
/// </summary>
/// <param name="map">マップのポインタ</param>
/// <param name="vectorV2">扇形の左端の2D座標</param>
/// <param name="vectorV3">扇形の右端の2D座標</param>
/// <param name="pointFlag1">左端側で交点が見つかったかどうか</param>
/// <param name="pointFlag2">右端で交点が見つかったかどうか</param>
void EnemyVisibility::SetTriangleVertices(Map* map, const DirectX::SimpleMath::Vector2& vectorV2, const DirectX::SimpleMath::Vector2& vectorV3, bool& pointFlag1, bool& pointFlag2)
{
   //オブジェクトの境界ボックスと判定を行う
   for (const auto& object : map->GetBoundingBoxes())
   {
      DirectX::SimpleMath::Vector2 corners[4];
      // オブジェクトのコーナーを計算
      CalculateObjectCorners(object, corners);

      //障害物の4辺をペアで定義
      std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> edges[4] = {
         {corners[0], corners[1]},
         {corners[1], corners[2]},
         {corners[2], corners[3]},
         {corners[3], corners[0]}
      };

      //各辺ごとに視界扇形の両辺ベクトルとの交差判定を行う
      for (const auto& edge : edges)
      {
         const DirectX::SimpleMath::Vector2& objectV1 = edge.first;
         const DirectX::SimpleMath::Vector2& objectV2 = edge.second;

         // 交差判定と三角形の頂点設定
         CheckIntersection(m_vectorV1, vectorV2, objectV1, objectV2, m_intersectionPoint1, pointFlag1);
         CheckIntersection(m_vectorV1, vectorV3, objectV1, objectV2, m_intersectionPoint2, pointFlag2);
      }
   }
}

/// <summary>
/// オブジェクトのコーナーを計算
/// </summary>
/// <param name="object">対象の境界ボックス</param>
/// <param name="corners">結果を格納する配列</param>
void EnemyVisibility::CalculateObjectCorners(const DirectX::BoundingBox& object, DirectX::SimpleMath::Vector2 corners[4])
{
   //境界ボックスの中心座標と半径を使って、XZ平面上の4隅の座標を計算する
   corners[0] = DirectX::SimpleMath::Vector2(object.Center.x - object.Extents.x, object.Center.z - object.Extents.z);
   corners[1] = DirectX::SimpleMath::Vector2(object.Center.x + object.Extents.x, object.Center.z - object.Extents.z);
   corners[2] = DirectX::SimpleMath::Vector2(object.Center.x + object.Extents.x, object.Center.z + object.Extents.z);
   corners[3] = DirectX::SimpleMath::Vector2(object.Center.x - object.Extents.x, object.Center.z + object.Extents.z);
}

/// <summary>
/// 交差判定と三角形の頂点設定
/// </summary>
/// <param name="start">視界の中心点</param>
/// <param name="end">視界の終点</param>
/// <param name="objectV1">障害物辺の始点</param>
/// <param name="objectV2">障害物辺の終点</param>
/// <param name="trianglePoint">三角形の頂点候補</param>
/// <param name="pointFlag">交点があったかどうか</param>
void EnemyVisibility::CheckIntersection(const DirectX::SimpleMath::Vector2& start, const DirectX::SimpleMath::Vector2& end, const DirectX::SimpleMath::Vector2& objectV1, const DirectX::SimpleMath::Vector2& objectV2, DirectX::SimpleMath::Vector2& trianglePoint, bool& pointFlag)
{
   //線分の当たり判定を行う
   if (IntersectLines2D(start, end, objectV1, objectV2))
   {
      //交差していれば交点を計算
      DirectX::SimpleMath::Vector2 intersectionPoint = IntersectPointLines2D(start, end, objectV1, objectV2);
      //一番距離が短い交点に上書きする
      if ((intersectionPoint - m_vectorV1).Length() < (trianglePoint - m_vectorV1).Length())
      {
         trianglePoint = intersectionPoint;
         pointFlag = true;
      }
   }
   else if (!pointFlag)
   {
      //交点が見つからなかった
      trianglePoint = end;
   }
}


/// <summary>
/// 視界の扇型を描画
/// </summary>
/// <param name="color">描画する色</param>
void EnemyVisibility::DrawSector(const DirectX::XMVECTOR& color)
{
   //扇形を描画する
   m_graphics->DrawFilledSector(*m_graphics->GetPrimitiveBatch(),
      DirectX::SimpleMath::Vector3(m_enemy->GetPosition().x, 0.01f, m_enemy->GetPosition().z),
      color,
      m_intersectionPoint1,
      m_intersectionPoint2);
}

