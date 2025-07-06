/*
	@file	Graphics.h
	@brief	グラフィックスクラス
*/
#include "pch.h"
#include "Game/Screen.h"
#include "Libraries/MyLib/Graphics.h"

std::unique_ptr<mylib::Graphics> mylib::Graphics::m_graphics = nullptr;

//グラフィックスのインスタンスを取得する
mylib::Graphics* const mylib::Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		//グラフィックスのインスタンスを生成する
		m_graphics.reset(new Graphics());
	}
	return m_graphics.get();
}


//コンテキスト
mylib::Graphics::Graphics()
	:
	m_deviceResources{},				// デバイスリソース
	m_commonStates{},					// コモンステート
	m_spriteBatch{},							// スプライトバッチ
	m_spriteFont{},								// スプライトフォント
	m_basicEffect{},							// ベーシックエフェクト
	m_primitiveBatch{},						// プリミティブバッチ
	m_rasterrizerState{},					// ラスタライザーステート
	//m_effectFactory{},						// エフェクトファクトリ
	m_inputLayout{},							// 入力レイアウト
	m_screenW(0),								// スクリーン幅
	m_screenH(0),								// スクリーン高
	m_view{},											// ビュー行列
	m_projection{},								// 射影行列
	m_device{},										// デバイス
	m_context{}									// デバイスコンテキスト
{
}


mylib::Graphics::~Graphics()
{
}


//初期化する
void mylib::Graphics::Initialize()
{
	//デバイスを取得する
	m_device = m_deviceResources->GetD3DDevice();
	//デバイスコンテキストを取得する
	m_context = m_deviceResources->GetD3DDeviceContext();
	//コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(m_device);
	//スプライトバッチを生成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context);
	//ベーシックエフェクトを生成する
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_device);
	//スプライトフォントを生成する
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device, L"resources\\font\\SegoeUI_18.spritefont");
	//プリミティブバッチを生成する
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_context);
	//頂点カラーを有効にする
	m_basicEffect->SetVertexColorEnabled(true);
	//テクスチャを無効にする
	m_basicEffect->SetTextureEnabled(false);

	void const* shaderByteCode;
	size_t byteCodeLength;
	//頂点シェーダ―を取得する
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//入力レイアウトを生成する
	m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	// ラスタライザーディスクリプション
	CD3D11_RASTERIZER_DESC rasterizerStateDesc(
		D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, TRUE
	);
	// ラスタライズステートを生成する
	m_device->CreateRasterizerState(&rasterizerStateDesc, m_rasterrizerState.ReleaseAndGetAddressOf());
	//// エフェクトファクトリを生成する
	//m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);
}


//プリミティブバッチを開始
void mylib::Graphics::DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	// ブレンディング状態を設定する
	m_context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	// 深度ステンシル状態を設定する
	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);
	// カリングを行わない
	m_context->RSSetState(m_commonStates->CullNone());
	// ラスタライザー状態を設定する
	m_context->RSSetState(m_rasterrizerState.Get());

	// ビュー行列を設定する
	m_basicEffect->SetView(view);
	// プロジェクション行列を設定する
	m_basicEffect->SetProjection(projection);
	// ワールド行列を設定する
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	// コンテキストを設定する
	m_basicEffect->Apply(m_context);
	// 入力レイアウトを設定する
	m_context->IASetInputLayout(m_inputLayout.Get());
	// プリミティブバッチを開始する
	m_primitiveBatch->Begin();

}

//プリミティブバッチを終了する
void mylib::Graphics::DrawPrimitiveEnd()
{
	// プリミティブバッチを終了する
	m_primitiveBatch->End();
}




