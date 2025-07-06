/*
	@file	Vertex.h
	@brief	グラフ頂点クラス
*/
#pragma once
#ifndef VERTEX_DEFINED
#define VERTEX_DEFINED
#include "SimpleMath.h"

class Vertex
{	
public:
	//最大頂点番号を取得する
	static int GetMaxNumber() { return s_maxVertexNumber; }
	//最大頂点番号を再設定する
	static void ResetMaxNumber(int maxNumber) { s_maxVertexNumber = maxNumber; }

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
	Vertex(const DirectX::SimpleMath::Vector3& position);
	//デストラクタ
	~Vertex();
private:
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
#endif // VERTEX_DEFINED

