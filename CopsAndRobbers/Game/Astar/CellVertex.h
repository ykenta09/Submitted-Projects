/*
	@file	Vertex.h
	@brief	セル頂点クラス
*/
#pragma once
#include "SimpleMath.h"

class CellVertex
{
public:
   //セルの状態
   int GetState() const { return m_state; }
   //セルの状態を設定する
   void SetState(int state) { m_state = state; }
   //頂点番号を取得する
   const int GetVertexNumber() const { return m_vertexNumber; }
   //頂点番号を設定する
   void SetVertexNumber(const int& vertexNumber) { m_vertexNumber = vertexNumber; }
   //頂点の座標を取得する
   DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
   //頂点の位置を設定する
   void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
   //親インデックスを取得する
   int GetParentVertexIndex() { return m_parentIndex; }
   //親インデックスを設定する
   void SetParentVertexIndex(const int& parentIndex) { m_parentIndex = parentIndex; }
   //開始ノードから現在のノードまでのコストを取得する
   float GetCost() const { return m_cost; }
   //コストを設定する
   void SetCost(const float& cost) { m_cost = cost; }
   //ヒューリスティックを取得する
   float GetHeuristic() const { return m_heuristic; }
   //ヒューリスティックを取得する
   void SetHeuristic(const float& heuristic) { m_heuristic = heuristic; }
   //スコアを取得する
   float GetScore() const { return m_score; }
   //スコアを設定する
   void SetScore(const float& score) { m_score = score; }
public:
   //コンストラクタ
   CellVertex();
   CellVertex(const DirectX::SimpleMath::Vector3& position);
   CellVertex(
	  int vertexNumber,
	  int parentIndex,
	  const DirectX::SimpleMath::Vector3& position,
	  const float& cost,
	  const float& heuristic,
	  const float& score
   );
   //デストラクタ
   ~CellVertex();

   //クリア
   void Clear();

private:
   //０：通行可能　１：障害物
   int m_state;       
   //最大頂点番号
   static int s_maxVertexNumber;
   //頂点番号
   int m_vertexNumber;
   //位置
   DirectX::SimpleMath::Vector3 m_position;
   //開始ノードから現在のノードのコスト
   float m_cost;
   //親ノードの頂点インデックス
   int m_parentIndex;
   //ヒューリスティックコスト
   float m_heuristic;
   //総スコア
   float m_score;
};

