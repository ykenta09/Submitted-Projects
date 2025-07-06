#include "pch.h"
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include "GraphScene.h"
#include "Model.h"
#include "Libraries/MyLib/Geometry.h"

 const float GraphScene::VERTEX_RADIUS  = 0.5f;

// コンストラクタ
 GraphScene::GraphScene(CommonResources* resources)
	 :
	 m_commonResources(resources),
	 m_editMode(false),
	 m_graphics(nullptr),							// DirectXGraphicsクラス
	 m_device(nullptr),								// Deviceクラス
	 m_context(nullptr),								// DeviceContextクラス
	 m_keyboardState{},								// キーボードステート
	 m_mouseCursorPosition{},					// マウスカーソル位置
	 m_mouseState{},									// マウスステート
	 m_mouseStateTracker{},						// マウスステートトラッカー
	 m_worldMatrix{},									// ワールド
	 m_viewMatrix{},									// ビュー
	 m_projectionMatrix{},							// プロジェクション
	 m_cameraRotation{},							// カメラ回転
	 m_cameraPosition{},							// カメラ位置
	 m_cameraFocus{},								// カメラフォーカス
	 m_rotaionAngle(0.0f),							// 角度
	 m_distance(10.0f),								// 注視点から視点までの距離
	 m_zoom(1.0f),										// ズーム
	 m_fov(DirectX::XM_PI / 4.0f),				// フィールドオブビュー
	 m_nearPlane(0.1f),								// ニアプレーン
	 m_farPlane(0.0f),								// ファープレーン
	 m_drive{},											// ドライブ
	 m_path{},												// パス
	 m_fileName{},										// ファイル名
	 m_extension{},										// 拡張子
	 m_scale(1.0f),										// スケール
	 m_position{},										// 位置
	 m_graph(nullptr),								// グラフ
	 m_spriteString3D(nullptr),					// スプライトストリング3D
	 m_enemyNumber(0),							//敵番号
	 m_enemyIndex(1),							//敵の数
	 m_initialEditMode(false)					//初期頂点番号編集モード
 {
	// Graphicsクラスのインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
	// デバイスを取得する
	m_device = yamadalib::Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	// デバイスコンテキストを取得する
	m_context = yamadalib::Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
}

// デストラクタ
GraphScene::~GraphScene()
{
	// Graphクラスのインスタンスをリセットする
	m_graph.reset();
}

// 初期化する
void GraphScene::Initialize()
{
	// Graphクラスのインスタンスを生成する
	m_graph = std::make_unique<Graph>();
	// SpriteString3Dクラスのインスタンスを生成する
	m_spriteString3D = std::make_unique<SpriteString3D>();
	//グラフデータを読み込む
	LoadGraphData();
	m_enemyIndex = m_graph->GetEnemyIndex();
}

// 更新する
void GraphScene::Update(float timer)
{
	UNREFERENCED_PARAMETER(timer);
	// キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	//キーボードの状態を更新する
	m_keyboardTracker.Update(m_keyboardState);
	// マウスの状態を取得する
	m_mouseState = m_commonResources->GetInputManager()->GetMouse()->GetState();
	// マウストラッカーを更新する
	m_mouseStateTracker.Update(m_mouseState);

	//編集モードを有効無効にする
	if (m_editMode == false && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::P))
	{
		m_editMode = true;
		m_commonResources->GetInputManager()->SetMouseMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
	else if (m_editMode == true && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::P))
	{
		m_editMode = false;
		m_commonResources->GetInputManager()->SetMouseMode(DirectX::Mouse::MODE_RELATIVE);
	}


	if (!m_editMode) return;

	// グラフデータをクリアする
	if (m_keyboardState.C)
	{
		ClearAll();
	}
	// グラフデータを読み込む
	if (m_keyboardState.L)
	{
		// グラフデータを読み込む
		LoadGraphData();
	}
	// グラフデータを保存する
	if (m_keyboardState.K)
	{
		//  グラフデータを保存する
		SaveGraphData();
	}
	//初期頂点番号を作成する
	if (m_initialEditMode == false && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::I))
	{
		m_initialEditMode = true;
	}
	else if (m_initialEditMode == true && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::I))
	{
		m_initialEditMode = false;
	}

	//新しい経路を追加する
	if (m_keyboardState.N)
	{
		NewEnemyAdjacencyList();
	}
	//経路を削除する
	if (m_keyboardState.Delete)
	{
		ClearEnemyAdjacencyList();
	}
	//敵番号を切り替える
	if (m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Left))
	{
		ChangeEnemyAdjacencyList(false);
	}
	if (m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Right))
	{
		ChangeEnemyAdjacencyList(true);
	}

	// 平面を初期化する
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);
	// マウスカーソルのスクリーン位置を取得する
	m_mouseCursorPosition = DirectX::SimpleMath::Vector2(roundf((float)m_mouseState.x), roundf((float)m_mouseState.y));

	// スクリーン位置から光線を生成する
	DirectX::SimpleMath::Ray ray = CreateRayFromScreenPoint(m_mouseCursorPosition);
	// 光線と平面との交差点を取得する
	IntersectRayPlane(ray, plane, &m_vertex);

	//初期頂点番号を設定する
	if (m_initialEditMode == true && m_mouseStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED && m_mouseState.leftButton == true)
	{
		InitalVertexNumber();
		return;
	}

	// 左マウスボタンをクリックした場合頂点を追加する
	if (m_mouseStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED && m_mouseState.leftButton == true)
	{
		// 「頂点」または「エッジ」を追加する
		AddVertexOrEdge();
	}

	// 右マウスボタンをクリックした位置に頂点を削除する
	if (m_mouseStateTracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
	{
		// 頂点と頂点に隣接するエッジを削除する
		RemoveVertexOrEdge();
	}
}

// マウスカーソル位置からスクリーンへの光線を生成する
DirectX::SimpleMath::Ray GraphScene::CreateRayFromScreenPoint(const DirectX::SimpleMath::Vector2& mousePosition)
{
	// ビューポートを取得する
	DirectX::SimpleMath::Viewport viewport(m_graphics->GetDeviceResources()->GetScreenViewport());
	// スクリーンのマウスカーソル位置をVector3に設定する
	DirectX::SimpleMath::Vector3 screenPosition(mousePosition.x, mousePosition.y, 0.0f);
	// スクリーン座標を3次元座標に変換する
	DirectX::SimpleMath::Vector3 worldPoint = viewport.Unproject(
		screenPosition,
		m_graphics->GetProjectionMatrix(),
		m_graphics->GetViewMatrix(),
		DirectX::SimpleMath::Matrix::Identity
	);

	// ビュー行列から逆行列を取得する
	DirectX::SimpleMath::Matrix matrix = m_graphics->GetViewMatrix().Invert();
	// カメラ位置を取得する
	DirectX::SimpleMath::Vector3 cameraPosition(matrix._41, matrix._42, matrix._43);
	// カメラの位置からワールド空間のマウスカーソル位置への方向を取得する
	DirectX::SimpleMath::Vector3 direction = worldPoint - cameraPosition;
	// 正規化する
	direction.Normalize();
	// 光線を生成する
	DirectX::SimpleMath::Ray ray(cameraPosition, direction);
	// 光線を返す
	return ray;
}

// 描画する
void GraphScene::Render()
{
	const DirectX::XMVECTORF32 xaxis = { 100.f, 0.0f, 0.0f };
	const DirectX::XMVECTORF32 yaxis = { 0.0f, 0.0f, 100.f };
	//編集モードのみ描画する
	if (m_editMode)
	{
		// グリッドを描画する
		//DrawGrid(xaxis, yaxis, DirectX::g_XMZero, 50, 50, DirectX::Colors::LightGray);
		auto debugString = m_commonResources->GetDebugString();
		debugString->AddString("EnemyNumber, %d", m_enemyNumber);
		debugString->AddString("EnemyIndex, %d", m_enemyIndex);
		debugString->AddString("EnemyInitailVertex, %d", m_graph->GetInitailVertex(m_enemyNumber));
		// グラフを描画する
		DrawGraph();
	}

#ifdef DEBUG

#endif // DEBUG


}

// 後処理を行う
void GraphScene::Finalize()
{
	// Graphクラスのインスタンスをリセットする
	m_graph.reset();
}

//グラフデータを読み込む
void GraphScene::LoadGraphData()
{
   // グラフデータをクリアする
   m_graph->Clear();
   // 最大頂点番号をゼロリセットする
   Vertex::ResetMaxNumber(0);
   //// すべての文字配列を削除する
   m_spriteString3D->RemoveAllString();
   // JSON形式のデータを読み込む
   m_graph->LoadGraphDataByJSON(m_commonResources->GetBlackBoard()->GetStageNumber());
   // 読み込んだJSON形式のデータをオブジェクトに変換する
   m_graph->ParseJSON();
   // SpriteString3Dクラスのインスタンスにすべての頂点番号を追加する
   AddAllVertexNumberToSpriteString3D();
}

//ステージデータから敵の数を取得する
int GraphScene::GetMaxEnemyIndex()
{
	// グラフデータをクリアする
	m_graph->Clear();
	// 最大頂点番号をゼロリセットする
	Vertex::ResetMaxNumber(0);
	//// すべての文字配列を削除する
	m_spriteString3D->RemoveAllString();
	// JSON形式のデータを読み込む
	m_graph->LoadGraphDataByJSON(m_commonResources->GetBlackBoard()->GetStageNumber());
	// 読み込んだJSON形式のデータをオブジェクトに変換する
	m_graph->ParseJSON();
	// SpriteString3Dクラスのインスタンスにすべての頂点番号を追加する
	AddAllVertexNumberToSpriteString3D();

	return m_graph->GetEnemyIndex();
}

 //グラフデータを保存する
void GraphScene::SaveGraphData()
{
	if (MessageBox(NULL, L"頂点情報を保存しますか？", L"頂点情報の保存", MB_YESNO) == IDYES)
	{
		// オブジェクトをJSON形式のデータとして保存する
		m_graph->SaveGraphDataByJSON(m_enemyIndex);
	}
}

// SpriteString3Dクラスのインスタンスにすべての頂点番号を追加する
void GraphScene::AddAllVertexNumberToSpriteString3D()
{
	// SpriteString3Dクラスのインスタンスに頂点番号(文字列)を追加する
	for (int index = 0; index < m_graph->GetVertexList().size(); index++)
	{
		wchar_t buffer[10];
		// 頂点番号をフォーマット化する
		swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%3d", m_graph->GetVertex(index).GetVertexNumber());
		// SpriteString3Dクラスのインスタンスに文字列を追加する
		m_spriteString3D->AddString(
			buffer, 
			DirectX::SimpleMath::Vector3(
				m_graph->GetVertex(index).GetPosition().x + 1, 
				3.0f, 
				m_graph->GetVertex(index).GetPosition().z
			),
		   DirectX::Colors::Blue
		);
	}
}

// グリッドを描画する
void GraphScene::DrawGrid(
	DirectX::FXMVECTOR xAxis,
	DirectX::FXMVECTOR yAxis,
	DirectX::FXMVECTOR origin,
	size_t xdivs,
	size_t ydivs,
	DirectX::GXMVECTOR color
)
{
	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	// プリミティブ描画を開始する
	m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
		[this, xdivs, ydivs, &xAxis, &origin, &yAxis, &color](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
		{
			for (int index = 0; index <= xdivs; ++index)
			{
				float percent = float(index) / float(xdivs);
				percent = (percent * 2.0f) - 1.0f;
				// スケールを計算する
				DirectX::XMVECTOR scale = DirectX::XMVectorScale(xAxis, percent);
				scale = DirectX::XMVectorAdd(scale, origin);
				// 頂点1を設定する
				DirectX::VertexPositionColor v1(DirectX::XMVectorSubtract(scale, yAxis), color);
				// 頂点2を設定する
				DirectX::VertexPositionColor v2(DirectX::XMVectorAdd(scale, yAxis), color);
				// 直線を描画する
				m_graphics->GetPrimitiveBatch()->DrawLine(v1, v2);
			}

			for (int index = 0; index <= ydivs; index++)
			{
				float percent = float(index) / float(ydivs);
				percent = (percent * 2.0f) - 1.0f;
				// スケールを計算する
				DirectX::XMVECTOR scale = DirectX::XMVectorScale(yAxis, percent);
				scale = DirectX::XMVectorAdd(scale, origin);
				// 頂点1を設定する
				DirectX::VertexPositionColor v1(DirectX::XMVectorSubtract(scale, xAxis), color);
				// 頂点2を設定する
				DirectX::VertexPositionColor v2(DirectX::XMVectorAdd(scale, xAxis), color);
				// 直線を描画する
				m_graphics->GetPrimitiveBatch()->DrawLine(v1, v2);
			}
		});
}

// 頂点と隣接リストを描画する
void GraphScene::DrawGraph()
{
	// 頂点リストを取得する
	const std::vector<Vertex>& vertexList = m_graph->GetVertexList();
	// 隣接リストを取得する
	const std::unordered_map<int, Graph::PatrolRoute>& adjacencyList = m_graph->GetAdjacencyList();
	// 頂点リストまたは隣接リストのサイズが0の場合
	if (vertexList.size() == 0 || adjacencyList.size() == 0)
		return;

	// プリミティブ描画を開始する
	m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
		[this, vertexList, adjacencyList](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
		{
			for (int index = 0; index < vertexList.size(); index++)
			{
				DirectX::XMVECTOR edgeColor = DirectX::Colors::Blue;
				for (int edgeIndex = 0; edgeIndex < adjacencyList.at(m_enemyNumber).adjacencyList.at(index).size(); edgeIndex++)
				{
					// 隣接リストの頂点間に直線を描画する
					m_graphics->DrawLine(
						m_graph->GetVertex(index).GetPosition(),
						m_graph->GetVertex(adjacencyList.at(m_enemyNumber).adjacencyList.at(index)[edgeIndex]).GetPosition() - m_graph->GetVertex(index).GetPosition(),
						edgeColor
					);
				}
				// 頂点の既定色を設定する
				DirectX::XMVECTOR vertexColor = DirectX::Colors::Blue;
				if (m_edgeMode == ON && (int)index == m_vertexStartIndex)
				{
					// 頂点を選択したときの色を設定する
					vertexColor = DirectX::Colors::Red;
				}
				// 頂点を描画する
				m_graphics->DrawCircle(
				   DirectX::SimpleMath::Vector2(vertexList[index].GetPosition().x,vertexList[index].GetPosition().z),
				   0.5f,
				   vertexColor
				);
			}
		});
	// SpriteStringクラスの文字列を描画する
	m_spriteString3D->Render();
}

// プロジェクションを生成する
void GraphScene::CreateProjection()
{
	// ウィンドウサイズを取得する
	auto size = m_graphics->GetDeviceResources()->GetOutputSize();
	// プロジェクションを生成する
	m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(size.right) / float(size.bottom), m_nearPlane, m_farPlane);
}

// 頂点またはエッジを追加する
void GraphScene::AddVertexOrEdge()
{
	// マウスポインタの位置を頂点ベクトルに設定する
   DirectX::SimpleMath::Vector3 vertexVector = DirectX::SimpleMath::Vector3(
	  std::round(m_vertex.x * 100) / 100,
	  std::round(m_vertex.y * 100) / 100,
	  std::round(m_vertex.z * 100) / 100);
   // 頂点リストに頂点ベクトルが存在するかどうかを調べる
	int vertexIndex = ExistingVertexInVertexList(vertexVector);

	// 頂点リストにマウスポインターの位置が存在していない場合
	if (vertexIndex == ERROR_INDEX)
	{
		// エッジ編集モードがONの場合
		if (m_edgeMode == ON)
		{
			// エッジ編集モードをOFFにする
			m_edgeMode = OFF;
			return;
		}
		// 頂点リストに頂点を追加する
		m_graph->CreateVertex(m_enemyIndex, Vertex(vertexVector));
		// 頂点番号を追加する
		AddVertexNumberToSpriteString3D((int)m_graph->GetVertexList().size());
	}

	// 頂点リストにマウスポインターの位置が存在している場合
	if (vertexIndex > ERROR_INDEX)
	{
		// エッジ編集モードがONの場合
		if (m_edgeMode == ON)
		{
			// 頂点終点インデックスを設定する
			m_vertexEndIndex = vertexIndex;
			if (m_vertexStartIndex != m_vertexEndIndex)
			{
				// 隣接リストにエッジの頂点を追加する(双方向に登録する)
				m_graph->AddVertexOfEdgeToAdjacencyList(m_enemyNumber, m_vertexStartIndex, m_vertexEndIndex);
				m_graph->AddVertexOfEdgeToAdjacencyList(m_enemyNumber, m_vertexEndIndex, m_vertexStartIndex);
			}
			// エッジ編集モードをOFFにする
			m_edgeMode = OFF;
		}
		else
		{
			// 頂点起点インデックスを設定する
			m_vertexStartIndex = vertexIndex;
			// エッジ編集モードをONにする
			m_edgeMode = ON;
		}
	}
}

/// 頂点リストに指定された頂点ベクトルが存在するかどうかを調べる
int GraphScene::ExistingVertexInVertexList(const DirectX::SimpleMath::Vector3& vertexVector)
{
	// 頂点インデックスをリセットする
	int vertexIndex = ERROR_INDEX;
	for (const Vertex& vertex : m_graph->GetVertexList())
	{
		// 頂点インデックスをインクリメントする
		vertexIndex++;
		// 右クリックしたマウスポインターの位置が既存の頂点リストに存在するかどうか
		if (InsideCircle(2, vertex.GetPosition(), vertexVector))
		{
			// 頂点インデックスを設定する
			return vertexIndex;
		}
	}
	// 頂点は存在しない
	return ERROR_INDEX;
}

// SpriteString3Dクラスのインスタンスに頂点番号を追加する
void GraphScene::AddVertexNumberToSpriteString3D(const int& vertexNumber)
{
	using namespace DirectX::SimpleMath;

	// SpriteString3Dクラスのインスタンスに頂点番号(文字列)を追加する
	wchar_t buffer[10];
	// 頂点番号をフォーマット化する
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%3d", vertexNumber - 1);
	// SpriteString3Dクラスのインスタンスに文字列を追加する
	m_spriteString3D->AddString(buffer, 
		Vector3(m_graph->GetVertex(vertexNumber - 1).GetPosition().x + 1, 
			0.0f, 
			m_graph->GetVertex(vertexNumber - 1).GetPosition().y)
	);
}

// 頂点またはエッジを削除する
void GraphScene::RemoveVertexOrEdge()
{
	// 平面を初期化する
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);

	// マウスカーソルのスクリーン位置を取得する
	m_mouseCursorPosition = DirectX::SimpleMath::Vector2((float)m_mouseState.x, (float)m_mouseState.y);
	// スクリーン位置からレイに変換する
	DirectX::SimpleMath::Ray ray = CreateRayFromScreenPoint(m_mouseCursorPosition);
	// レイと平面との交差点を取得する
	IntersectRayPlane(ray, plane, &m_vertex);
	// 3D空間の頂点を取得する
	DirectX::SimpleMath::Vector3 vertexVector = m_vertex;

	// 頂点リストに指定された頂点ベクトルが存在するかどうかを調べる
	int vertexIndex = ExistingVertexInVertexList(vertexVector);
	// 頂点リストにマウスポインターの位置が存在していない場合エッジを削除する
	if (vertexIndex == ERROR_INDEX)
	{
		// 単独でエッジを削除する
		RemoveEdge(m_enemyNumber, vertexVector);
	}
	// 頂点上にマウスポインターが存在している場合頂点および頂点に隣接するエッジを削除する
	else if (vertexIndex > ERROR_INDEX)
	{
		// 頂点リストから頂点を取得する
		Vertex vertex = m_graph->GetVertex(vertexIndex);
		// 頂点を指定し隣接リストを削除する
		m_graph->RemoveVertexOfEdgeFromAdjacencyList(m_enemyNumber, vertexIndex);
		// 頂点リストに指定された頂点ベクトルが存在するかどうかを調べる
		vertexIndex = ExistingVertexInVertexList(vertexVector);
		// 頂点リストから頂点を削除する
		m_graph->RemoveVertexFromVertexList(vertexIndex);
		// SpriteString3Dクラスの文字列配列からすべての配列の要素を削除する
		m_spriteString3D->RemoveAllString();
		// SpriteString3Dクラスの文字列配列にすべての頂点番号を追加する
		AddAllVertexNumberToSpriteString3D();
		// 頂点番号を再設定する
		m_graph->ReNumber();
	}
}

// エッジを削除する
void GraphScene::RemoveEdge(int number, const DirectX::SimpleMath::Vector3& vertexVector)
{
	// 頂点リストを取得する
	const std::vector<Vertex>& vertexList = m_graph->GetVertexList();
	// 隣接リストを取得する
	const std::unordered_map<int, Graph::PatrolRoute>& adjacencyList = m_graph->GetAdjacencyList();
	// 隣接リスト一覧のループを構成する
	for (int index = 0; index < (int)adjacencyList.at(number).adjacencyList.size(); index++)
	{
		// 隣接リストからエッジインデックスのループを構成する
		for (int edgeIndex = 0; edgeIndex < (int)adjacencyList.at(m_enemyNumber).adjacencyList.at(index).size(); edgeIndex++)
		{
			// 線分と円の交差判定を行い、交差している場合そのエッジを削除する
			if (IntersectCircleLine(
			    vertexVector,
				VERTEX_RADIUS,
			    vertexList[index].GetPosition(),
				vertexList[adjacencyList.at(m_enemyNumber).adjacencyList.at(index)[edgeIndex]].GetPosition()))
			{
				// 隣接リストからindex番目の頂点のedgeIndex番目のエッジを削除する
				m_graph->Graph::RemoveEgdeFromAdjacencyListByIndex(m_enemyNumber, index, edgeIndex);
			}
		}
	}

}

// パス名を分離する
void GraphScene::SplitPath(wchar_t* modelName)
{
	// パス名を分割する
	_wsplitpath_s(modelName, m_drive, _MAX_DRIVE, m_path, MAX_PATH, m_fileName, _MAX_FNAME, m_extension, _MAX_EXT);
}

//すべてクリアする
void GraphScene::ClearAll()
{
	//①モデルをリセットする
	if (m_model != nullptr) m_model.reset();
	//②グラフをクリアする
	m_graph->Clear();
	//③最大頂点番号をゼロリセットする
	Vertex::ResetMaxNumber(0);
	//④すべての3D文字列配列を削除する
	m_spriteString3D->RemoveAllString();
}


//敵の隣接リストを切り替える
void GraphScene::ChangeEnemyAdjacencyList(bool increaseNumbers)
{
	if (increaseNumbers)
	{
		if (m_enemyNumber < m_enemyIndex)
			m_enemyNumber++;
	}
	else
	{
		if(m_enemyNumber > 0)
		m_enemyNumber--;
	}
	m_graph->NewAdjacencyList(m_enemyNumber);
}

//現在の敵番号の隣接リストを削除する
void GraphScene::ClearEnemyAdjacencyList()
{
	if (MessageBox(NULL, L"現在の番号のデータを削除しますか？", L"データを削除する", MB_YESNO) == IDYES)
	{
		m_graph->ClearNumberAdjacenyList(m_enemyNumber);
		//敵の総数を減らす
		if (m_enemyIndex > 0)
			m_enemyIndex--;
		//敵総数より番号が超えていたら下げる
		if (m_enemyNumber > m_enemyIndex)
			m_enemyNumber--;
	}
}

//新しい敵の隣接リストを追加する
void GraphScene::NewEnemyAdjacencyList()
{
	if (MessageBox(NULL, L"新しい敵の経路を作成しますか？", L"経路を追加", MB_YESNO) == IDYES)
	{
		m_enemyIndex++;
		m_graph->CreateInitailVertex(m_enemyIndex, m_graph->GetVertexList().front().GetVertexNumber());
	}
}

//敵の初期頂点座標を作成する
void GraphScene::InitalVertexNumber()
{
	// マウスポインタの位置を頂点ベクトルに設定する
	DirectX::SimpleMath::Vector3 vertexVector = DirectX::SimpleMath::Vector3(roundf(m_vertex.x), 0.0f, roundf(m_vertex.z));
	// 頂点リストに頂点ベクトルが存在するかどうかを調べる
	int vertexIndex = ExistingVertexInVertexList(vertexVector);

	// 頂点リストにマウスポインターの位置が存在している場合
	if (vertexIndex > ERROR_INDEX)
	{
		m_graph->CreateInitailVertex(m_enemyNumber, vertexIndex);
	}
	else
	{
		MessageBox(NULL, L"敵の初期頂点番号編集中です", L"警告！", MB_YESNO);
	}
}
