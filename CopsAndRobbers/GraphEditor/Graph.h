/*
	@file	Graph.h
	@brief	グラフクラス
*/
#pragma once
#ifndef GRAPH_DEFINED
#define GRAPH_DEFINED
#include <memory>
#include <vector>
//#include <algorithm>
#include <stralign.h>
#include "Libraries/yamadaLib/Graphics.h"
#include "GraphEditor/Vertex.h"
#include <unordered_map>

class Graph
{	
public:
	struct PatrolRoute    //各敵の巡回ルート
	{
		std::vector<std::vector<int>> adjacencyList;
	};
public:
	//指定されたインデックスのvertexを取得する
	const Vertex& GetVertex(int vertexIndex) { return m_vertexList[vertexIndex]; }
	//頂点リストを取得する
	const std::vector<Vertex>& GetVertexList() { return m_vertexList; }
	//隣接リストを取得する
	const std::unordered_map<int, PatrolRoute>& GetAdjacencyList() { return m_adjacencyList; }
	//巡回ルートを取得する
	const std::vector< DirectX::SimpleMath::Vector3>& GetPatrolRootMap() { return m_patrolRootMap; }
	//敵の数を取得する
	int GetEnemyIndex() { return m_enemyIndex; }
	//敵の初期頂点番号を取得する
	int GetInitailVertex(int number) { 
	   if (m_initialVertexNumber.size() > 0)
	   {
		  return m_initialVertexNumber[number][0];
	   }
    }
public:
	//コンストラクタ
	Graph();
	//デストラクタ
	~Graph();

	//グラフを初期化
	void Clear();
	//頂点を生成する
	bool CreateVertex(const int enemyIndex, const Vertex& vertex);
	// 頂点リストの頂点位置をスケーリングする
	void ScaleVertexPositionOfVertexList(const float& scale);
	// 隣接リストにエッジの頂点を追加する
	bool AddVertexOfEdgeToAdjacencyList(const int& enemyNumber, const int& vertexIndex, const int& vertexOfEdge);
	// 頂点リストから頂点を削除する
	void RemoveVertexFromVertexListByIndex(const int& index);
	// 隣接リストからエッジの頂点を削除する
	void RemoveVertexOfEdgeFromAdjacencyListByIndex(const int& enemyNumber, const int& index);
	// 頂点を削除する
	bool RemoveVertexFromVertexList(const Vertex& vertex);
	// 頂点リストから頂点を削除する
	bool RemoveVertexFromVertexList(const int& vertexIndex);

	// 隣接リストのエッジの頂点を削除する
	bool RemoveVertexOfEdgeFromAdjacencyList(const int& enemyNumber, const int& vertexNumber);
	// 隣接リストからインデックス番目の頂点のエッジインデックス番目のエッジを削除する
	void RemoveEgdeFromAdjacencyListByIndex(const int& enemyNumber, const int& index, const int& edgeIndex);
	// 頂点番号を再設定する
	void ReNumber();
	// JSON形式でグラフデータを保存する
	bool SaveGraphDataByJSON(int index);
	// JSON形式でグラフデータを読み込む
	bool LoadGraphDataByJSON(const int stageNumber);
	// データを変換する
	void ParseJSON();
	//空の隣接リストを生成する
	void NewAdjacencyList(const int& enemyNumber);
	//特定の敵番号の隣接リストを削除する
	void ClearNumberAdjacenyList(const int& enemyNumber);
	//初期頂点番号を生成する
	void CreateInitailVertex(const int& enemyNumber, const int vertexNumber);
private:
	//頂点リスト(全ての敵が共有する頂点配列)
	std::vector<Vertex> m_vertexList;
	//隣接リスト<敵の番号,頂点番号,繋がっている頂点番号>
	std::unordered_map<int, PatrolRoute> m_adjacencyList;

	// 入力グラフデータ
	std::vector<std::string> m_graphData;
	//ステージのすべての巡回ルート
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRootMap;
	//各敵の巡回ルート隣接リスト<敵の番号、ルート番号、繋がっているルート番号>
	std::unordered_map<int, std::vector<int>> m_patrolAdjacencyList;
	//敵の数
	int m_enemyIndex;
	//敵の初期頂点番号
	std::vector<std::vector<int>> m_initialVertexNumber;
};
#endif // GRAPH_DEFINED

