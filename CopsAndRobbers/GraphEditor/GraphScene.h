#pragma once
#ifndef GRAPH_SCENE_DEFINED
#define GRAPH_SCENE_DEFINED
#include "DeviceResources.h"
//#include "ArcBall.h"
#include "Game.h"
#include "Libraries/MyLib/SpriteString3D.h"
#include "Graph.h"	

class CommonResources;

class GraphScene
{
	// エラーインデックス
	static const int ERROR_INDEX = -1;
	static const float VERTEX_RADIUS;
	const bool ON = true;		
	const bool OFF = false;		

public:
	bool GetEditMode() { return m_editMode; }	
public:
	// コンストラクタ
	GraphScene(CommonResources* resources);
	// デストラクタ
	~GraphScene();
	// 初期化する
	void Initialize();
	// 更新する
	void Update(float timer);
	// 描画する
	void Render();
	// 後処理を行う
	void Finalize();
	//グラフを読み込む(編集モード用)
	void LoadGraphData();
	//頂点リストを取得する
	const std::vector<Vertex>& GetVertexList() { return m_graph->GetVertexList(); }
	//巡回ルートを取得する
	const std::vector<DirectX::SimpleMath::Vector3>& GetPatrolRootMap() { return m_graph->GetPatrolRootMap(); }
	//隣接リストを取得する
	const std::unordered_map<int , Graph::PatrolRoute>& GetAdjacencyList() { return m_graph->GetAdjacencyList(); };
	//ステージの敵の最大数を取得する(使いどころがなければ削除する関数)
	int GetMaxEnemyIndex();
	//ステージの敵の最大数を取得する
	int GetEnemyIndex() { return m_enemyIndex; }
	//敵の初期頂点番号を取得する
	int GetInitalVertexNumbeer(const int& number) { return m_graph->GetInitailVertex(number); }
private:
	// グリッドを描画する
	void DrawGrid(
		DirectX::FXMVECTOR xAxis, 
		DirectX::FXMVECTOR yAxis, 
		DirectX::FXMVECTOR origin, 
		size_t xdivs, 
		size_t ydivs, 
		DirectX::GXMVECTOR m_color
	);
	// プロジェクションを生成する
	void CreateProjection();

	//グラフを保存する
	void SaveGraphData();

	// SpriteString3Dクラスのインスタンスにすべての頂点番号を追加する
	void AddAllVertexNumberToSpriteString3D();
	// グラフを描画する
	void DrawGraph();

	// 頂点またはエッジを追加する
	void AddVertexOrEdge();
	// 頂点リストに指定された頂点ベクトルが存在するかどうかを調べる
	int ExistingVertexInVertexList(const DirectX::SimpleMath::Vector3& vertexVector);
	// SpriteString3Dクラスのインスタンスに頂点番号を追加する
	void AddVertexNumberToSpriteString3D(const int& vertexNumber);
	// マウスカーソル位置からスクリーンへの光線を計算する
	DirectX::SimpleMath::Ray CreateRayFromScreenPoint(const DirectX::SimpleMath::Vector2& mousePosition);

	// 頂点またはエッジを削除する
	void RemoveVertexOrEdge();
	// エッジを削除する
	void RemoveEdge(int number, const DirectX::SimpleMath::Vector3& vertexVector);

	// パス名を分離する(20231120)
	void SplitPath(wchar_t* modelName);
	//すべてクリアする
	void ClearAll();

	//敵の隣接リストを切り替える
	void ChangeEnemyAdjacencyList(bool increaseNumbers);
	//現在の敵番号の隣接リストを削除する
	void ClearEnemyAdjacencyList();
	//新しい敵の経路追加する
	void NewEnemyAdjacencyList();
	//敵の初期頂点番号を作成する
	void InitalVertexNumber();
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// DirectXGraphicsクラスのインスタンスへのポインタ
	yamadalib::Graphics* m_graphics;
	// Deviceクラスのインスタンスへのポインタ
	ID3D11Device* m_device;
	// DeviceContextクラスのインスタンスへのポインタ
	ID3D11DeviceContext* m_context;
	// マウスカーソル位置
	DirectX::SimpleMath::Vector2 m_mouseCursorPosition;
	// キーボードステート
	DirectX::Keyboard::State m_keyboardState;
	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	// マウスステート
	DirectX::Mouse::Mouse::State m_mouseState;
	// マウスステートトラッカー
	DirectX::Mouse::ButtonStateTracker m_mouseStateTracker;

	// ワールド
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// ビュー
	DirectX::SimpleMath::Matrix m_viewMatrix;
	// プロジェクション
	DirectX::SimpleMath::Matrix m_projectionMatrix;
	// カメラ回転
	DirectX::SimpleMath::Quaternion m_cameraRotation;
	// カメラ位置
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// カメラフォーカス
	DirectX::SimpleMath::Vector3 m_cameraFocus;
	// 角度
	float m_rotaionAngle;
	// 注視点から視点までの距離
	float m_distance;
	// ズーム
	float m_zoom;
	// フィールドオブビュー
	float m_fov;
	// ニアプレーン
	float m_nearPlane;
	// ファープレーン
	float m_farPlane;

	// グラフ
	std::unique_ptr<Graph> m_graph;
	// スプライトストリング3D
	std::unique_ptr<SpriteString3D> m_spriteString3D;

	// 頂点
	DirectX::SimpleMath::Vector3 m_vertex;
	// 頂点終点インデックス
	int m_vertexEndIndex;
	// 頂点始点インデックス
	int m_vertexStartIndex;
	// エッジ編集モード
	bool m_edgeMode;

	// ドライブ名
	wchar_t m_drive[_MAX_DRIVE];
	// パス名
	wchar_t m_path[MAX_PATH];
	// ファイル名
	wchar_t m_fileName[_MAX_FNAME];
	// 拡張子
	wchar_t m_extension[_MAX_EXT];
	// スケール
	float m_scale;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	//編集モード
	bool m_editMode;
	//初期頂点番号作成モード
	bool m_initialEditMode;
	//ステージの敵の数
	int m_enemyIndex;
	//敵の番号
	int m_enemyNumber;
};

#endif		// GRAPH_SCENE_DEFINED
