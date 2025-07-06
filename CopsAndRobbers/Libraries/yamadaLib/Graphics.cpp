/*
	@file	Graphis.h
	@brief	描画関係のクラス
*/
#include "pch.h"
#include "Game/Screen.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/CommonResources.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include <iostream>


std::unique_ptr<yamadalib::Graphics> yamadalib::Graphics::m_graphics = nullptr;


/// <summary>
/// グラフィックスのインスタンスを取得する
/// </summary>
/// <returns></returns>
yamadalib::Graphics* const yamadalib::Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		//グラフィックスのインスタンスを生成する
		m_graphics.reset(new Graphics());
	}
	return m_graphics.get();
}


/// <summary>
/// コンストラクタ
/// </summary>
yamadalib::Graphics::Graphics()
	:
	m_commonResources{},
	m_deviceResources{},
	m_spriteBatch(nullptr),
	m_spriteFont(nullptr),
	m_device{},
	m_commonStates{},
	m_context{},
	m_view{},
	m_projection{},
	m_primitiveBatchColor{},
	m_basicEffectTexture{},
	m_basicEffectColor{},
	m_effectFactory{},
	m_layoutColor{}
{

	//デバイスリソースのインスタンスを生成する
	m_deviceResources = std::make_unique<DX::DeviceResources>(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT_D24_UNORM_S8_UINT);

}


/// <summary>
/// デストラクタ
/// </summary>
yamadalib::Graphics::~Graphics()
{
   Clear();
}


/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
void yamadalib::Graphics::Initialize(CommonResources* resources)
{
    m_commonResources = resources;
	m_context = m_deviceResources->GetD3DDeviceContext();				//デバイスコンテキストを取得する
	m_device = m_deviceResources->GetD3DDevice();						//デバイスを取得する
	m_commonStates = std::make_unique<DirectX::CommonStates>(m_device);	//コモンステートを生成する

	// プリミティブバッチの生成する
	m_primitiveBatchColor = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_deviceResources->GetD3DDeviceContext());
	m_primitiveBatchTexture = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(m_deviceResources->GetD3DDeviceContext());
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context);				//スプライトバッチを生成する
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device, L"resources\\Fonts\\SegoeUI_18.spritefont");
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);			//エフェクトファクトリーを生成する
	m_basicEffectColor = std::make_unique<DirectX::BasicEffect>(m_device);					// エフェクトの初期化
	m_basicEffectColor->SetVertexColorEnabled(true);								//頂点カラーを有効にする

	//ライトの設定
	m_basicEffectColor->SetAmbientLightColor(DirectX::Colors::DimGray); // 暗い色を使用
	// ディレクショナルライトの例
	m_basicEffectColor->SetLightEnabled(0, true);
	m_basicEffectColor->SetLightDirection(0, DirectX::SimpleMath::Vector3(1.0f, -1.0f, 1.0f));
	m_basicEffectColor->SetLightDiffuseColor(0, DirectX::Colors::Gray); // 暗い色を使用

	m_basicEffectTexture = std::make_unique<DirectX::BasicEffect>(m_device);
	m_basicEffectTexture->SetTextureEnabled(true);

	//カスタムレンダーを生成
	m_customRender = std::make_unique<CustomModelRenderer>(m_commonResources, this);
	// Graphicsクラスにライト情報を保持するメンバ変数を追加
	DirectX::SimpleMath::Vector3 m_lightDirection = { 0.5f, -1.0f, 0.25f };
	DirectX::SimpleMath::Vector4 m_lightDiffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	DirectX::SimpleMath::Vector4 m_ambientColor = { 0.01f, 0.01f, 0.01f, 1.0f };

	// CustomModelRendererにライト情報を設定
	m_customRender->SetLightParameters(m_lightDirection, m_lightDiffuse, m_ambientColor);

	//頂点シェーダ―を取得する
	void const* shaderByteCode;
	size_t byteCodeLength;

	// VertexPositionColor 用の入力レイアウト	
	m_basicEffectColor->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	m_device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
	   DirectX::VertexPositionColor::InputElementCount,
	   shaderByteCode, byteCodeLength,
	   m_layoutColor.ReleaseAndGetAddressOf()
	);
	m_device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
	   DirectX::VertexPositionColor::InputElementCount,
	   shaderByteCode, byteCodeLength,
	   m_layoutColor.GetAddressOf()
	);

	// VertexPositionTexture 用の入力レイアウト
	m_basicEffectTexture->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	m_device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_layoutTexture.ReleaseAndGetAddressOf()
	);
}

/// <summary>
/// プリミティブ描画(VertexPositionColor用)
/// </summary>
/// <param name="states">コモンステート</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
/// <param name="drawFunction"></param>
void yamadalib::Graphics::DrawPrimitivesColor(DirectX::CommonStates& states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, std::function<void(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)> drawFunction)
{
	m_context->OMSetBlendState(states.AlphaBlend(), nullptr, 0xFFFFFFFF);   // ブレンディング状態を設定する
	m_context->OMSetDepthStencilState(states.DepthRead(), 1);				// 深度ステンシル状態を設定する
	m_context->RSSetState(states.CullCounterClockwise());				    // 裏面をカリングする

	m_basicEffectColor->SetWorld(DirectX::SimpleMath::Matrix::Identity);    // ワールド行列を設定する
	m_basicEffectColor->SetView(view);										// ビュー行列を設定する
	m_basicEffectColor->SetProjection(projection);							// プロジェクション行列を設定する
	m_basicEffectColor->Apply(m_context);									// コンテキストを設定する

	m_context->IASetInputLayout(m_layoutColor.Get());						// 入力レイアウトを設定する

	m_primitiveBatchColor->Begin();											// プリミティブバッチを開始する
	drawFunction(*m_primitiveBatchColor);									// 描画関数を呼び出す
	m_primitiveBatchColor->End();											// プリミティブバッチを終了する
}

/// <summary>
/// プリミティブ描画(VertexPositionTexture用)
/// </summary>
/// <param name="states">コモンステート</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
/// <param name="texture">テクスチャ</param>
/// <param name="drawFunction"></param>
void yamadalib::Graphics::DrawPrimitivesTexture(DirectX::CommonStates& states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture ,std::function<void(DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>&)> drawFunction)
{
	m_context->OMSetBlendState(states.AlphaBlend(), nullptr, 0xFFFFFFFF);		// ブレンディング状態を設定する
	//m_context->OMSetDepthStencilState(states.DepthNone(), 1);				// 深度ステンシル状態を設定する
	m_context->RSSetState(states.CullCounterClockwise());						 // 裏面をカリングする
	ID3D11SamplerState* sampler = { states.LinearClamp() };
	m_context->PSSetSamplers(0, 1, &sampler);

	m_basicEffectTexture->SetWorld(DirectX::SimpleMath::Matrix::Identity);							// ワールド行列を設定する
	m_basicEffectTexture->SetView(view);										// ビュー行列を設定する
	m_basicEffectTexture->SetProjection(projection);							// プロジェクション行列を設定する
	m_basicEffectTexture->SetTexture(texture.Get());
	m_basicEffectTexture->Apply(m_context);									// コンテキストを設定する

	m_context->IASetInputLayout(m_layoutTexture.Get());						// 入力レイアウトを設定する

	m_primitiveBatchTexture->Begin();											// プリミティブバッチを開始する
	drawFunction(*m_primitiveBatchTexture);									// 描画関数を呼び出す
	m_primitiveBatchTexture->End();											// プリミティブバッチを終了する
}

/// <summary>
/// モデルを描画する
/// </summary>
/// <param name="model">モデル</param>
/// <param name="world">ワールド行列</param>
void yamadalib::Graphics::DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world)
{
	//モデルを描画する
	model->Draw(m_context, *m_commonStates.get(), world, m_view, m_projection);
}


/// <summary>
/// 境界ボックスを描画
/// </summary>
/// <param name="box">境界ボックス</param>
void yamadalib::Graphics::DrawBoundingBox(const DirectX::BoundingBox& box)
{
	//境界ボックスを描画
	DX::Draw(m_primitiveBatchColor.get(), box, DirectX::Colors::White);
}

/// <summary>
/// 境界球を描画する
/// </summary>
/// <param name="sphere">境界球</param>
void yamadalib::Graphics::DrawBoundingSphere(const DirectX::BoundingSphere& sphere)
{
	//境界ボックスを描画
	DX::Draw(m_primitiveBatchColor.get(), sphere, DirectX::Colors::Blue);
}

/// <summary>
/// XZ平面の線分を描画する	
/// </summary>
/// <param name="position">開始座標</param>
/// <param name="vector">線分のベクトル</param>
/// <param name="color">カラー</param>
void yamadalib::Graphics::DrawLine(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
   // 頂点カラーを設定する
   DirectX::VertexPositionColor vertex[2] =
   {
	   { DirectX::SimpleMath::Vector3(position.x, 0.0f, position.y), color },
	   { DirectX::SimpleMath::Vector3(position.x + vector.x, 0.0f, position.y + vector.y), color }
   };
   // 線分を描画する
   m_primitiveBatchColor->DrawLine(vertex[0], vertex[1]);
}

/// <summary>
/// XY平面の線分を描画する
/// </summary>
/// <param name="position">開始座標</param>
/// <param name="vector">線分のベクトル</param>
/// <param name="color">カラー</param>
void yamadalib::Graphics::DrawLine2D(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
   // 頂点カラーを設定する
   DirectX::VertexPositionColor vertex[2] =
   {
	   { DirectX::SimpleMath::Vector3(position.x, position.y, 0.0f), color },
	   { DirectX::SimpleMath::Vector3(position.x + vector.x, position.y + vector.y, 0.0f), color }
   };
   // 線分を描画する
   m_primitiveBatchColor->DrawLine(vertex[0], vertex[1]);
}


/// <summary>
/// 線分を描画する
/// </summary>
/// <param name="position">開始座標</param>
/// <param name="vector">線分のベクトル</param>
/// <param name="color">カラー</param>
void yamadalib::Graphics::DrawLine(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& vector, const DirectX::FXMVECTOR& color)
{
	// 頂点カラーを設定する
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, position.y, position.z), color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, position.y + vector.y, position.z + vector.z), color }
	};
	// 線分を描画する
	m_primitiveBatchColor->DrawLine(vertex[0], vertex[1]);
}

/// <summary>
/// XZ平面の円を描画する
/// </summary>
/// <param name="center">描画座標</param>
/// <param name="radius">半径</param>
/// <param name="color">カラー</param>
/// <param name="split">分割する数</param>
void yamadalib::Graphics::DrawCircle(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
	// 角度を初期化する
	float angle = 0.0f;
	// 始点を宣言する
	DirectX::SimpleMath::Vector2 position1 = center + DirectX::SimpleMath::Vector2(cosf(angle), sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// 始点を設定する
	   DirectX::SimpleMath::Vector2 position0 = position1;
		// 角度を計算する
		angle += DirectX::XM_2PI / (float)split;
		// 終点を計算する
		position1 = center + DirectX::SimpleMath::Vector2(cosf(angle), sinf(angle)) * radius;
		// 円を描画する
		DrawLine(position0, position1 - position0, color);
	}
}

/// <summary>
/// XY平面の円を描画する
/// </summary>
/// <param name="center">描画座標</param>
/// <param name="radius">半径</param>
/// <param name="color">カラー</param>
/// <param name="split">分割する数</param>
void yamadalib::Graphics::DrawCircle2D(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
   // 角度を初期化する
   float angle = 0.0f;
   // 始点を宣言する
   DirectX::SimpleMath::Vector2 position1 = center + DirectX::SimpleMath::Vector2(cosf(angle), sinf(angle)) * radius;
   for (int index = 0; index < split; index++)
   {
	  // 始点を設定する
	  DirectX::SimpleMath::Vector2 position0 = position1;
	  // 角度を計算する
	  angle += DirectX::XM_2PI / (float)split;
	  // 終点を計算する
	  position1 = center + DirectX::SimpleMath::Vector2(cosf(angle), sinf(angle)) * radius;
	  // XY平面上で円を描画する
	  // Z軸方向は無視し、XとYのみを使用
	  DrawLine2D(position0, position1 - position0, color);
   }
}


/// <summary>
/// 円を描画する
/// </summary>
/// <param name="center">描画座標</param>
/// <param name="radius">半径</param>
/// <param name="color">カラー</param>
/// <param name="split">分割する数</param>
void yamadalib::Graphics::DrawCircle(const DirectX::SimpleMath::Vector3& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
	// 角度を初期化する
	float angle = 0.0f;
	// 終点を計算する
	DirectX::SimpleMath::Vector3 position1 = center + DirectX::SimpleMath::Vector3(cosf(angle), center.y, sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// 始点を設定する
	   DirectX::SimpleMath::Vector3 position0 = position1;
		// 角度を計算する
		angle += DirectX::XM_2PI / (float)split;
		// 終点を計算する
		position1 = center + DirectX::SimpleMath::Vector3(cosf(angle), center.y, sinf(angle)) * radius;
		// 円を描画する
		DrawLine(position0, position1 - position0, color);
	}
}


/// <summary>
/// テクスチャを描画する
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <param name="position">描画座標</param>
/// <param name="srcRect">テクスチャ切り取り</param>
/// <param name="color">カラー</param>
/// <param name="rotation">回転</param>
/// <param name="origin">基準点</param>
/// <param name="scale">スケール</param>
void yamadalib::Graphics::DrawTexture(
   const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture,
   const DirectX::SimpleMath::Vector2& position,
   const RECT* srcRect,
   const DirectX::XMVECTORF32& color,
   const float rotation,
   const DirectX::SimpleMath::Vector2& origin,
   const DirectX::SimpleMath::Vector2& scale)
{
	RECT rectToUse;
	if (srcRect != nullptr) {
		// 指定されたRECTを使用
		rectToUse = *srcRect;
	}
	else {
		// テクスチャ全体のRECTを取得して使用
		rectToUse = GetTextureRect(texture);
	}
	// 描画開始
	m_spriteBatch->Begin();
	m_spriteBatch->Draw(texture.Get(), position, &rectToUse, color, rotation, origin, scale);
	m_spriteBatch->End();
}

/// <summary>
/// テクスチャ全体のサイズを取得する
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <returns></returns>
RECT yamadalib::Graphics::GetTextureRect(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
    //テクスチャのリソースを取得する
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	texture->GetResource(resource.GetAddressOf());

	// リソースを2Dテクスチャに変換
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	resource.As(&texture2D);

	// テクスチャの情報（幅・高さなど）を取得
	D3D11_TEXTURE2D_DESC desc;
	texture2D->GetDesc(&desc);

	// 左上 (0, 0)、右下 (Width, Height) の矩形として返す
	return RECT{ 0, 0, static_cast<LONG>(desc.Width), static_cast<LONG>(desc.Height) };
}

/// <summary>
/// 扇型を描画する関数
/// </summary>
/// <param name="batch">プリミティブバッチ</param>
/// <param name="center">描画する座標</param>
/// <param name="color">カラー</param>
/// <param name="point2">扇の左端点</param>
/// <param name="point3">扇の右端点</param>
void yamadalib::Graphics::DrawFilledSector(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>& batch,
	const DirectX::SimpleMath::Vector3& center,
	const DirectX::XMVECTOR& color,
    const DirectX::SimpleMath::Vector2& point2,
    const DirectX::SimpleMath::Vector2& point3)
{
	batch.DrawTriangle(
	   DirectX::VertexPositionColor(center, color),
	   DirectX::VertexPositionColor(DirectX::SimpleMath::Vector3(point2.x, 0.01f, point2.y), color),
	   DirectX::VertexPositionColor(DirectX::SimpleMath::Vector3(point3.x, 0.01f, point3.y), color));
}

/// <summary>
/// テクスチャをビルボード描画
/// </summary>
/// <param name="texture">テクスチャ</param>
/// <param name="world">ワールド行列</param>
void yamadalib::Graphics::DrawTextureBillboard(ID3D11ShaderResourceView* texture, const DirectX::SimpleMath::Matrix& world, bool Depthflag)
{
	// ビルボードの頂点情報を設定する：今回は「VertexPositionTexture」を使用する
	m_vertices[0] = { DirectX::SimpleMath::Vector3(-0.5f, 0.5f,0.0f), DirectX::SimpleMath::Vector2(0.0f,0.0f) };	// 左上
	m_vertices[1] = { DirectX::SimpleMath::Vector3(0.5f, 0.5f,0.0f),DirectX::SimpleMath::Vector2(1.0f,0.0f) };	// 右上
	m_vertices[2] = { DirectX::SimpleMath::Vector3(-0.5f,-0.5f,0.0f),DirectX::SimpleMath::Vector2(0.0f,1.0f) };	// 左下
	m_vertices[3] = { DirectX::SimpleMath::Vector3(0.5f,-0.5f,0.0f),DirectX::SimpleMath::Vector2(1.0f,1.0f) };	// 右下

	// ビルボード行列の計算
	DirectX::SimpleMath::Matrix viewMatrix = GetViewMatrix();
	//ビルボードをアフィン行列を作る
	DirectX::SimpleMath::Matrix billboardMatrix = viewMatrix.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;

	// ビルボードの（原点からの）平行移動量
	DirectX::SimpleMath::Vector3 billboardTranslationValue{ 0.0f, 2.5f, 0.0f };

	// ビルボードをアフィン変換する
	// ビルボードの高さ調整
	DirectX::SimpleMath::Matrix worldBillboard = billboardMatrix *
	   DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, 2.5f, 0.0f)) * world;

	// ポリゴンを描画するために各種パラメータを設定する
	m_context->OMSetBlendState(m_commonStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);	// Tridentロゴは「NonPremultiplied」
	if(Depthflag)
	  m_context->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);					// 深度情報のreadとwrite
	else
	   m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);					// 深度情報のreadとwrite

	m_context->RSSetState(m_commonStates->CullCounterClockwise());						// ポリゴンの裏面は描画しない
	m_context->IASetInputLayout(m_layoutTexture.Get());								// 適切な入力レイアウトを指定する

	m_basicEffectTexture->SetWorld(worldBillboard);	// ワールド行列を指定する
	m_basicEffectTexture->SetView(m_view);				// ビュー行列を指定する
	m_basicEffectTexture->SetProjection(m_projection);	// 射影行列を指定する
	m_basicEffectTexture->SetTexture(texture);	// テクスチャを指定する
	m_basicEffectTexture->Apply(m_context);				// ベーシックエフェクトの変更を適用する

	// ビルボードを描画する
	m_primitiveBatchTexture->Begin();
	m_primitiveBatchTexture->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_primitiveBatchTexture->End();
}

/// <summary>
/// クリア関数
/// </summary>
void yamadalib::Graphics::Clear()
{
}
