#include "pch.h"
#include "Game/Astar/CellVertex.h"


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
CellVertex::CellVertex()
   :
   m_position(DirectX::SimpleMath::Vector3::Zero),
   m_vertexNumber(0),
   m_parentIndex(0),
   m_state(0),
   m_score(0.0f),
   m_heuristic(0.0f),
   m_cost(0.0f)
{
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">作成する座標</param>
CellVertex::CellVertex(const DirectX::SimpleMath::Vector3& position)
   :
   m_position(position),
   m_vertexNumber(0),
   m_parentIndex(0),
   m_state(0),
   m_score(0.0f),
   m_heuristic(0.0f),
   m_cost(0.0f)
{
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="vertexNumber">頂点番号</param>
/// <param name="parentIndex">親ノードの頂点インデックス</param>
/// <param name="position">座標</param>
/// <param name="cost">コスト</param>
/// <param name="heuristic">ヒューリスティックコスト</param>
/// <param name="score">スコア</param>
CellVertex::CellVertex(
   int vertexNumber, 
   int parentIndex, 
   const DirectX::SimpleMath::Vector3& position,
   const float& cost, 
   const float& heuristic,
   const float& score)
   :
   m_vertexNumber(vertexNumber),
   m_parentIndex(parentIndex),
   m_position(position),
   m_cost(cost),
   m_heuristic(heuristic),
   m_score(score)
{
}


/// <summary>
/// デストラクタ
/// </summary>
CellVertex::~CellVertex()
{
}

/// <summary>
/// クリア
/// </summary>
void CellVertex::Clear()
{
}
