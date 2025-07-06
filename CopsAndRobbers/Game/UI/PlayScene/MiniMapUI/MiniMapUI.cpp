/*
	@file	MiniMapUI.cpp
	@brief	ミニマップUIクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include <Libraries/Microsoft/ReadData.h>
#include <Game/Particle/BinaryFile.h>
#include "Libraries/MyLib/BlackBoard.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/GameParameter.h"


///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> MiniMapUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


/// <summary>
///  コンストラクタ
/// </summary>
/// <param name="resources">共通リソース</param>
/// <param name="player">プレイヤー</param>
/// <param name="fellow">仲間</param>
/// <param name="map">マップ</param>
MiniMapUI::MiniMapUI(CommonResources* resources, Player* player, Fellow* fellow, Map* map)
   :
   m_commonResources(resources),
   m_player(player),
   m_fellow(fellow),
   m_map(map),
   m_graphics(yamadalib::Graphics::GetInstance()),
   m_backTexturePosition(BACK_TEXTURE_POSITION),
   m_playerTexturePosition(PLAYER_TEXTURE_POSITION),
   m_isCompositionTextureInitialized(false)
{
}


/// <summary>
/// デストラクタ
/// </summary>
MiniMapUI::~MiniMapUI()
{
   //後始末する
   Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void MiniMapUI::Initialize()
{
	//UIテクスチャを読み込む
    m_miniMapBackTexture = yamadalib::Resources::GetInstance()->GetTexture("MiniMapBack");
	m_circleTexture = yamadalib::Resources::GetInstance()->GetTexture("MaskCircle");

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);				//スプライトバッチを生成する
	
	// レンダーテクスチャのサイズ設定（ミニマップ背景サイズ）
	RECT miniMapRect = { 0, 0, BACK_RECT.right, BACK_RECT.bottom };

	//マスク準備用レンダーテクスチャの作成
	for (int i = 1; i <= GameParameters::MAX_STAGE_NUM; i++)
	{
	   m_renderTexture[i] = std::make_unique<DX::RenderTexture>(m_commonResources->GetDeviceResources()->GetBackBufferFormat());
	   m_renderTexture[i]->SetDevice(device);
	   m_renderTexture[i]->SetWindow(miniMapRect);
	}

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	m_states = std::make_unique<DirectX::CommonStates>(device);

	//シェーダーを作成
	CreateShader();

	//ミニマップ用のビューポート
	m_miniMapViewpor = {
	0.0f, 0.0f,
	RENDER_TEXTURE_SIZE.x, RENDER_TEXTURE_SIZE.y,
	0.0f, 1.0f
	};

	//メイン画面のビューポート
	m_mainViewport = {
	0.0f, 0.0f,
	static_cast<float>(m_commonResources->GetDeviceResources()->GetOutputSize().right),
	static_cast<float>(m_commonResources->GetDeviceResources()->GetOutputSize().bottom),
	0.0f, 1.0f
	};
}


/// <summary>
/// 更新する
/// </summary>
void MiniMapUI::Update()
{
   //現在いるステージ番号を取得する
   m_currentStageNumber = m_commonResources->GetBlackBoard()->GetStageNumber();
   //指定したステージの中心座標を取得する
   m_miniMapCenter = m_map->GetStageCenterPosition(m_commonResources->GetBlackBoard()->GetStageNumber());

   //円形テクスチャの座標更新
   m_circlePosition = WorldToMiniMap(m_player->GetPosition(), m_miniMapCenter, MAP_SPACE) + CIRCLE_POSITION_OFSET;
}


/// <summary>
/// 描画する
/// </summary>
void MiniMapUI::Render()
{   
   // ミニマップとマップオブジェクトの描画
   DrawMiniMapBack();
   DrawMiniMapObject();

   //マスク用のテクスチャ描画とマスクテクスチャの描画
   UpdateCompositionTexture();
   RenderShader();
}


/// <summary>
/// 後始末する
/// </summary>
void MiniMapUI::Finalize()
{
	// デバイスリソースの解放
   for (int i = 1; i <= GameParameters::MAX_STAGE_NUM; i++)
   {
	  m_renderTexture[i]->ReleaseDevice();
   }
   m_spriteBatch.reset();
}


/// <summary>
/// シェーダーを作成する
/// </summary>
void MiniMapUI::CreateShader()
{
   ID3D11Device1* device = m_commonResources->GetDeviceResources()->GetD3DDevice();

   //	コンパイルされたシェーダファイルを読み込み
   BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/MiniMapShader/MiniMapVS.cso");
   BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/MiniMapShader/MiniMapGS.cso");
   BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/MiniMapShader/MiniMapPS.cso");

   //	インプットレイアウトの作成
   device->CreateInputLayout(&INPUT_LAYOUT[0],
	  static_cast<UINT>(INPUT_LAYOUT.size()),
	  VSData.GetData(), VSData.GetSize(),
	  m_inputLayout.GetAddressOf());

   //	頂点シェーダ作成
   if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
   {//	エラー
	  MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
	  return;
   }
   //	ジオメトリシェーダ作成
   if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
   {//	エラー
	  MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
	  return;
   }
   //	ピクセルシェーダ作成
   if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
   {//	エラー
	  MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
	  return;
   }

   //	シェーダーにデータを渡すためのコンスタントバッファ生成
   D3D11_BUFFER_DESC bd;
   ZeroMemory(&bd, sizeof(bd));
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof(ConstBuffer);
   bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bd.CPUAccessFlags = 0;
   device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}


/// <summary>
/// ワールド座標からミニマップ座標に変換する
/// </summary>
/// <param name="worldPos">ワールド座標</param>
/// <param name="mapCenter">マップの中心</param>
/// <param name="mapScale">マップのサイズ</param>
/// <returns></returns>
DirectX::SimpleMath::Vector2 MiniMapUI::WorldToMiniMap(const DirectX::SimpleMath::Vector3& worldPos, const DirectX::SimpleMath::Vector3& mapCenter, float mapScale)
{
   //ワールド座標からミニマップ上の相対位置を計算
   DirectX::SimpleMath::Vector3 offset = worldPos - mapCenter;

   return DirectX::SimpleMath::Vector2(offset.x * mapScale, offset.z * mapScale);
} 


/// <summary>
/// ミニマップ背景描画
/// </summary>
void MiniMapUI::DrawMiniMapBack()
{
   //幅と高さを設定する
   float width = static_cast<float>(BACK_RECT.right - BACK_RECT.left);
   float height = static_cast<float>(BACK_RECT.bottom - BACK_RECT.top);
   //ミニマップの背景テクスチャを描画する
   m_graphics->DrawTexture(m_miniMapBackTexture, m_backTexturePosition, &BACK_RECT,
	  DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(width / 2 , height / 2), MINIMAP_SCALE);
}


/// <summary>
/// ミニマップオブジェクト描画
/// </summary>
void MiniMapUI::DrawMiniMapObject()
{
   //幅と高さを設定する
   float width = static_cast<float>(OBJECT_RECT.right - OBJECT_RECT.left);
   float height = static_cast<float>(OBJECT_RECT.bottom - OBJECT_RECT.top);

   // 特定ステージのみ参照する場合
   const std::vector<std::unique_ptr<IMapObject>>& stageObjs = m_map->GetStageObjects(m_currentStageNumber);
   for (int i = 0; i < stageObjs.size(); i++)
   {
	  IMapObject* obj = stageObjs[i].get();
	  // objを参照
	  CollisionObject* collisionObj = obj->GetCollisionObject();
	  if (collisionObj)
	  {
		 //パラメータを取得する
		 const auto& param = collisionObj->GetCollisionParameter();
		 if (param.objectID == CollisionManager::ObjectType::FLOOR) continue;

		 //オブジェクトの境界ボックスから座標を取得
		 DirectX::SimpleMath::Vector3 objectPos = param.collision.box.Center;
		 //ワールド座標からミニマップ座標に変換する
		 m_mapTexturePosition = WorldToMiniMap(objectPos, m_miniMapCenter, MAP_SPACE) + m_backTexturePosition;
		 //スケール調整
		 DirectX::SimpleMath::Vector2 scale =
			DirectX::SimpleMath::Vector2(param.collision.box.Extents.x * MAP_SPACE / MAP_SCALE_X_DIVISOR,
			   (param.collision.box.Extents.z) * MAP_SPACE / MAP_SCALE_Z_DIVISOR);
		 //スケールが異常値にならないように制限する
		 scale.x = yamadalib::Math::Clamp(scale.x, MIN_ICON_SCALE, MAX_ICON_SCALE);
		 scale.y = yamadalib::Math::Clamp(scale.y, MIN_ICON_SCALE, MAX_ICON_SCALE);
		 //オブジェクトをミニマップに描画する
		 m_graphics->DrawTexture(m_miniMapBackTexture, m_mapTexturePosition, &OBJECT_RECT, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(width / 2, height / 2), scale);
	  }
   }
   RECT charaRect = { 317, 76 , 338, 106 };

   //プレイヤーアイコンの描画
   m_playerTexturePosition = WorldToMiniMap(m_player->GetPosition(), m_miniMapCenter, MAP_SPACE) + m_backTexturePosition;
   m_graphics->DrawTexture(m_miniMapBackTexture, m_playerTexturePosition, &charaRect, DirectX::Colors::White, m_player->GetAngle(), DirectX::SimpleMath::Vector2(width / 2, height / 2), PLAYER_FELLOW_ICON_SCALE);

   //仲間のアイコンの描画
   //m_fellowTexturePosition = WorldToMiniMap(m_fellow->GetPosition(), m_miniMapCenter, MAP_SPACE) + m_backTexturePosition;
   //m_graphics->DrawTexture(m_miniMapBackTexture, m_fellowTexturePosition, &FELLOW_RECT, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(width / 2, height / 2), PLAYER_FELLOW_ICON_SCALE);
}

/// <summary>
/// 累積テクスチャを作成する
/// </summary>
void MiniMapUI::UpdateCompositionTexture()
{
   //デバイスコンテキストを取得する
   auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

   //レンダーターゲットの切り替え
   ID3D11RenderTargetView* oldRTV = nullptr;
   ID3D11DepthStencilView* oldDSV = nullptr;
   context->OMGetRenderTargets(1, &oldRTV, &oldDSV);

   //レンダーテクスチャをアクティブに設定
   ID3D11RenderTargetView* rtv = m_renderTexture[m_currentStageNumber]->GetRenderTargetView();
   context->OMSetRenderTargets(1, &rtv, nullptr);
   //ビューポート設定
   context->RSSetViewports(1, &m_miniMapViewpor);

   //レンダーテクスチャが初期化されていなければ初期化する
   if (!m_isCompositionTextureInitialized)
   {
	  InitializeCompositionTexture(context, rtv, m_miniMapViewpor);
	  m_isCompositionTextureInitialized = true;
   }

   //円形テクスチャを描画
   m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());  // アルファブレンドを有効にする
   m_spriteBatch->Draw(m_circleTexture.Get(), m_circlePosition, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, CIRCLE_SCALE);
   m_spriteBatch->End();

   //レンダーターゲットの結果を元に戻す
   context->OMSetRenderTargets(1, &oldRTV, oldDSV);
   //メインレンダーターゲットのビューポートを設定
   context->RSSetViewports(1, &m_mainViewport);
}


/// <summary>
///  累積テクスチャを初期化する
/// </summary>
/// <param name="context">デバイスコンテキスト</param>
/// <param name="rtv">レンダーターゲットビュー</param>
/// <param name="viewport">ビューポート</param>
void MiniMapUI::InitializeCompositionTexture(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, const D3D11_VIEWPORT& viewport)
{
   //レンダーターゲットを m_compositionTexture に設定
   context->OMSetRenderTargets(1, &rtv, nullptr);
   //ビューポートの設定
   context->RSSetViewports(1, &viewport);
   //描画に使用する色を設定 (ここでは透明色で初期化)
   float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 透明
   //レンダーターゲットをクリア
   context->ClearRenderTargetView(rtv, clearColor);

   //レンダーターゲットを元に戻す (必要に応じて)
   ID3D11RenderTargetView* nullRTV[] = { nullptr };
   context->OMSetRenderTargets(1, nullRTV, nullptr);
}

/// <summary>
/// シェーダーを描画する
/// </summary>
void MiniMapUI::RenderShader()
{
   //デバイスコンテキストを取得する
   auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

   //ピクセルシェーダーの結果をミニマップビューポート内に描画
   context->IASetInputLayout(m_inputLayout.Get());

   //頂点座標をビューポート範囲内に調整
   DirectX::VertexPositionColorTexture vertex[4] =
   {
	   DirectX::VertexPositionColorTexture(MINIMAP_MASK_VERTEX, DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
   };

   //シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
   ConstBuffer cbuff;
   cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matWorld = DirectX::SimpleMath::Matrix::CreateScale(MINIMAP_MASK_SCALE);
   cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

   //受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
   context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

   //シェーダーにバッファを渡す
   ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
   context->VSSetConstantBuffers(0, 1, cb);
   context->GSSetConstantBuffers(0, 1, cb);
   context->PSSetConstantBuffers(0, 1, cb);

   //画像用サンプラーの登録
   ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
   context->PSSetSamplers(0, 1, sampler);

   //半透明描画指定
   ID3D11BlendState* blendstate = m_states->NonPremultiplied();
   //透明判定処理
   context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
   //深度バッファに書き込み参照する
   context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
   //カリングは左周り
   context->RSSetState(m_states->CullNone());

   //シェーダをセットする
   context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
   context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
   context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

   //ピクセルシェーダにテクスチャを登録する。
   ID3D11ShaderResourceView* textures[] = {
	   m_renderTexture[m_currentStageNumber]->GetShaderResourceView()
   };
   context->PSSetShaderResources(0, 1, textures);

   //描画
   m_batch->Begin();
   m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
   m_batch->End();

   //シェーダの登録を解除しておく
   context->VSSetShader(nullptr, nullptr, 0);
   context->GSSetShader(nullptr, nullptr, 0);
   context->PSSetShader(nullptr, nullptr, 0);
}