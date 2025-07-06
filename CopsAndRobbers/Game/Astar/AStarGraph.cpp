#include <pch.h>
#include "Game/Astar/AStarGraph.h"
#include "Libraries/MyLib/Graphics.h"
#include "Game/Map/Base/GimmickObjectBase.h"
#include "Game/Collision/CollisionObject.h"
#include <algorithm>
#include <cstddef>


/// <summary>
/// グリッドの幅と高さを設定し、m_gridのサイズを設定
/// </summary>
/// <param name="map">マップ</param>
/// <param name="stageNumber">ステージ番号</param>
AStarGraph::AStarGraph(Map* map)
   : 
   m_map(map)
{
   //頂点リストと隣接リストを作成する
   CreateVertexList();
   //動的グラフをベースグラフで初期化
   InitializeDynamicGraph();
   //ギミックオブジェクトをリストに反映
   ApplyDynamicGimmicks();

   m_graphics = yamadalib::Graphics::GetInstance(); // Graphicsインスタンスを取得
   m_stageTransition = m_map->GetStageTransition();
}


/// <summary>
/// 頂点リストを作成する
/// </summary>
void AStarGraph::CreateVertexList()
{
   //頂点リストと隣接リストをクリア
   m_baseVertexList.clear();
   m_baseAdjacencyList.clear();

   //各ステージの頂点リストを作成
   for (int stageNum = GameParameters::MIN_STAGE_NUM; stageNum < GameParameters::MAX_STAGE_NUM; stageNum++)
   {
      //マップデータを取得する
      const auto& mapData = m_map->GetMapData(stageNum);
      //ステージの中心座標を取得する
      DirectX::SimpleMath::Vector3 stageCenter = m_map->GetStageCenterPosition(stageNum);
      //マップのサイズを設定する
      m_width = static_cast<int>(mapData.size());
      m_height = static_cast<int>(mapData.size());

      m_baseVertexList[stageNum].resize(m_width * m_height);
      m_baseAdjacencyList[stageNum].resize(m_width * m_height);

      for (int z = 0; z < m_height; z++)
      {
         for (int x = 0; x < m_width; x++)
         {
            //セルデータを取得
            const auto& cell = mapData[z][x];
            int vertexNumber = z * m_width + x;

            //座標計算(中心を原点)
            float worldX = static_cast<float>(x - m_width / 2) + stageCenter.x;
            float worldZ = static_cast<float>(z - m_height / 2) + stageCenter.z;
            DirectX::SimpleMath::Vector3 position(worldX, 0.0f, worldZ);

            //CellVertexの作成
            CellVertex vertex(position);
            vertex.SetVertexNumber(vertexNumber);
            vertex.SetState(cell.objectID);

            //グリッド情報を頂点リストに登録する
            m_baseVertexList[stageNum][vertexNumber] = vertex;

            //隣接リストを作成する頂点が通行可能ならリストを作成する
            if (cell.objectID == 1)
            {
               AddAdjacentVertices(stageNum, x, z);
            }
         }
      }
   }
}


/// <summary>
/// 隣接リストを登録する関数
/// </summary>
/// <param name="x">頂点リストのｘ</param>
/// <param name="z">頂点リストのｙ</param>
void AStarGraph::AddAdjacentVertices(int stageNumber, int x, int z)
{
   //現在の頂点番号
   int currentVertexNumber = z * m_width + x;
   // 隣接方向の相対座標（上下左右）
   constexpr int dx[] = { 0,  0, -1,  1};
   constexpr int dz[] = { -1,  1,  0,  0};
   //ステージデータを取得
   const auto& mapData = m_map->GetMapData(stageNumber);

   for (int i = 0; i < 4; i++)
   {
      int neighborX = x + dx[i];
      int neighborZ = z + dz[i];
      //有効なセルか確認
      if (IsValidCell(neighborX, neighborZ))
      {
         const auto& neighborCell = mapData[neighborZ][neighborX];
         //セルが通行可能か確認
         if (neighborCell.objectID == 1)
         {
            //隣接頂点の頂点番号を設定し、隣接リストに格納
            int neighborVertexNumber = neighborZ * m_width + neighborX;
            m_baseAdjacencyList[stageNumber][currentVertexNumber].push_back(neighborVertexNumber);
         }
      }
   }
}

/// <summary>
/// 座標からグラフデータへ変換する
/// </summary>
/// <param name="position">変換する座標</param>
/// <returns></returns>
CellVertex* AStarGraph::GetVertexFromPosition(const int& stageNumber, const DirectX::SimpleMath::Vector3& position)
{
   DirectX::SimpleMath::Vector3 stageCenter = m_map->GetStageCenterPosition(stageNumber);
   DirectX::SimpleMath::Vector3 localPosition = position - stageCenter;
   // セルの中心に近いセルへマッピング
   int xIndex = static_cast<int>(std::round(localPosition.x + (m_width / 2.0f)));
   int zIndex = static_cast<int>(std::round(localPosition.z + (m_height / 2.0f)));

   if (xIndex < 0 || xIndex >= m_width || zIndex < 0 || zIndex >= m_height)
      return nullptr;

   int vertexIndex = zIndex * m_width + xIndex;
   if (vertexIndex >= 0 && vertexIndex < static_cast<int>(m_vertexList[stageNumber].size()))
      return &m_vertexList[stageNumber][vertexIndex];

   return nullptr;
}

/// <summary>
/// セルが有効かどうかを確認する関数
/// </summary>
/// <param name="x">調べるセル</param>
/// <param name="z">調べるセル</param>
/// <returns></returns>
bool AStarGraph::IsValidCell(int x, int z) const
{
   return x >= 0 && x < m_width && z >= 0 && z < m_height;
}

/// <summary>
/// 移動方向に床のあるセルか確認する関数
/// </summary>
/// <param name="stageNumber"></param>
/// <param name="position"></param>
/// <returns></returns>
bool AStarGraph::IsFloorCell(int stageNumber, const DirectX::SimpleMath::Vector3& position) 
{
   CellVertex* cell = GetVertexFromPosition(stageNumber, position);
   //セルがない場合も移動可能(ステージ間移動をかのうにするため)
   if (!cell) return true;
   //セルが存在し、床ならtrue
   return cell->GetState() == 1;
}

/// <summary>
/// 動的グラフをベースグラフで初期化
/// </summary>
void AStarGraph::InitializeDynamicGraph()
{
   //ベースリストを取得する
   m_vertexList = m_baseVertexList;
   m_adjacencyList = m_baseAdjacencyList;
}

/// <summary>
/// 動的オブジェクトを更新（外部から呼び出す用）
/// </summary>
void AStarGraph::UpdateDynamicObjects()
{
   //動的グラフをベースグラフで初期化
   InitializeDynamicGraph();
   //ギミックオブジェクトを反映する
   ApplyDynamicGimmicks();
}

/// <summary>
/// ギミックオブジェクトをリストに反映
/// </summary>
void AStarGraph::ApplyDynamicGimmicks()
{
   //ギミックオブジェクトを取得する
   const std::vector<GimmickObjectBase*> gimmicks = m_map->GetGimmickObjects();

   for (const auto& gimmick : gimmicks)
   {
      //ギミックを反映
      ApplySingleGimmick(gimmick);
   }

   //隣接リストを再構築
   RebuildAdjacencyList();
}

/// <summary>
/// 単一ギミックの影響を適用
/// </summary>
/// <param name="gimmick">適用するギミックオブジェクト</param>
void AStarGraph::ApplySingleGimmick(const GimmickObjectBase* gimmick)
{
   //境界ボックスを取得する
   DirectX::BoundingBox box = gimmick->GetCollisionObject()->GetCollisionParameter().collision.box;

   //ギミックのタイプで通行可否を決定
   int cellState = 0;
   switch (gimmick->GetType())
   {
   case IMapObject::Type::FLOOR:
      cellState = 1;
      break;
   case IMapObject::Type::WALL:
      cellState = 2; 
      break;
   }

   //グリッド全体を走査してAABB内のセルを反映
   const int stageNumber = gimmick->GetObjectStageNumber();
   auto& vertexList = m_vertexList[stageNumber];
   //グリッドの原点・セルサイズを取得
   DirectX::SimpleMath::Vector3 stageCenter = m_map->GetStageCenterPosition(stageNumber);

   float cellSize = 1.0f; // セル1辺の長さ（必要に応じて設定）

   float localMinX = (box.Center.x - box.Extents.x - stageCenter.x) / cellSize;
   float localMaxX = (box.Center.x + box.Extents.x - stageCenter.x) / cellSize;
   float localMinZ = (box.Center.z - box.Extents.z - stageCenter.z) / cellSize;
   float localMaxZ = (box.Center.z + box.Extents.z - stageCenter.z) / cellSize;
   //境界ボックスのmin/maxをグリッドインデックスに変換
   int minX = static_cast<int>(std::ceil(localMinX + m_width / 2.0f));
   int maxX = static_cast<int>(std::floor(localMaxX + m_width / 2.0f));
   int minZ = static_cast<int>(std::ceil(localMinZ + m_height / 2.0f));
   int maxZ = static_cast<int>(std::floor(localMaxZ + m_height / 2.0f));
   //範囲内のセルに状態適用
   for (int z = minZ; z <= maxZ; ++z)
   {
      for (int x = minX; x <= maxX; ++x)
      {
         if (!IsValidCell(x, z)) continue;
         int vertexIndex = z * m_width + x;
         //オブジェクトの高さが地面に近い場合のみ反映する
         float cellY = vertexList[vertexIndex].GetPosition().y;
         if (std::abs(cellY - box.Center.y) >= 1.0f) continue;
         vertexList[vertexIndex].SetState(cellState);
      }
   }
}

/// <summary>
/// 隣接リストを再構築
/// </summary>
void AStarGraph::RebuildAdjacencyList()
{
   for (int stageNum = GameParameters::MIN_STAGE_NUM; stageNum < GameParameters::MAX_STAGE_NUM; stageNum++)
   {
      if (m_vertexList.find(stageNum) == m_vertexList.end()) continue;

      // ステージごとの隣接リストを再構築
      RebuildStageAdjacency(stageNum);
   }
}

/// <summary>
/// ステージ単位で隣接リストを再構築
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
void AStarGraph::RebuildStageAdjacency(int stageNumber)
{
   //サイズ設定
   m_width = static_cast<int>(std::sqrt(m_vertexList[stageNumber].size()));
   m_height = m_width;

   m_adjacencyList[stageNumber].clear();
   m_adjacencyList[stageNumber].resize(m_width * m_height);

   // 各セルの隣接関係を更新
   for (int z = 0; z < m_height; z++)
   {
      for (int x = 0; x < m_width; x++)
      {
         int vertexNumber = z * m_width + x;
         if (m_vertexList[stageNumber][vertexNumber].GetState() == 1) // 通行可能
         {
            UpdateAdjacencyForCell(stageNumber, x, z);
         }
      }
   }
}

/// <summary>
/// セルの隣接関係を更新
/// </summary>
/// <param name="stage">ステージバン後</param>
/// <param name="x">セルのx軸</param>
/// <param name="z">セルのz軸</param>
void AStarGraph::UpdateAdjacencyForCell(int stage, int x, int z) 
{
   int idx = z * m_width + x;
   m_adjacencyList[stage][idx].clear();

   if (m_vertexList[stage][idx].GetState() == 1)
   {
      // 通行可能かチェック
      // 上下左右のセルをチェック
      static constexpr int dx[] = { 0, 0, -1, 1 };
      static constexpr int dz[] = { -1, 1, 0, 0 };

      for (int i = 0; i < 4; ++i) 
      {
         int nx = x + dx[i], nz = z + dz[i];
         if (IsValidCell(nx, nz))
         {
            int nidx = nz * m_width + nx;
            if (m_vertexList[stage][nidx].GetState() == 1)
            {
               m_adjacencyList[stage][idx].push_back(nidx);
            }
         }
      }
   }
}
