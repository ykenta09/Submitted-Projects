/*
	@file	DebugBoundingDraw.h
	@brief	デバック境界ボックス描画クラス
*/
#include "pch.h"
#include "Game/Screen.h"
#include "Libraries/MyLib/DebugBoundingDraw.h"
#include "Game/CommonResources.h"
#include <Libraries/Microsoft/DebugDraw.h>

std::unique_ptr<mylib::DebugBoundingDraw> mylib::DebugBoundingDraw::m_debugBoundingDraw = nullptr;

//グラフィックスのインスタンスを取得する
mylib::DebugBoundingDraw* const mylib::DebugBoundingDraw::GetInstance()
{
	if (m_debugBoundingDraw == nullptr)
	{
		//グラフィックスのインスタンスを生成する
		m_debugBoundingDraw.reset(new DebugBoundingDraw());
	}
	return m_debugBoundingDraw.get();
}


//コンストラクタ
mylib::DebugBoundingDraw::DebugBoundingDraw()
	:
	m_commonResources{}
{
}


//初期化する
void mylib::DebugBoundingDraw::Initialize(CommonResources* resources)
{
	using namespace DirectX;

	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// エフェクトの初期化
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;
	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// 入力レイアウトの作成
	device->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_layout.ReleaseAndGetAddressOf()
	);

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_commonResources->GetDeviceResources()->GetD3DDeviceContext());
}

//プリミティブバッチ開始	
void mylib::DebugBoundingDraw::DrawPrimitiveBegin(
	DirectX::CommonStates& states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection)
{
	using namespace DirectX::SimpleMath;

	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	
	context->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states.DepthNone(), 0);
	context->RSSetState(states.CullNone());

	m_effect->SetWorld(Matrix::Identity);
	m_effect->SetView(view);
	m_effect->SetProjection(projection);
	m_effect->Apply(context);

	context->IASetInputLayout(m_layout.Get());

	m_batch->Begin();
}

//プリミティブバッチを終了する
void mylib::DebugBoundingDraw::DrawPrimitiveEnd()
{
	//プリミティブバッチを終了する
	m_batch->End();
}



