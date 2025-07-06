/*
	@file	DebugBoundingDraw.h
	@brief	デバック境界ボックス描画クラス
*/	
#pragma once
#include "DeviceResources.h"

class CommonResources;

namespace mylib
{
	class DebugBoundingDraw final
	{
	public:
		//Graphicsクラスのインスタンスを取得する
		static mylib::DebugBoundingDraw* const GetInstance();
		//プリミティブバッチを取得する
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatch() const { return m_batch.get(); }
		//プリミティブバッチ開始
		void DrawPrimitiveBegin(
			DirectX::CommonStates& states,
			DirectX::SimpleMath::Matrix view,
			DirectX::SimpleMath::Matrix projection);
		//プリミティブバッチ終了
		void DrawPrimitiveEnd();
	public:
		DebugBoundingDraw();
		~DebugBoundingDraw() = default;
		void Initialize(CommonResources* resources);
	private:
	private:
		//DebugBoundingDrawクラスのインスタンスへのポインタ
		static std::unique_ptr<mylib::DebugBoundingDraw> m_debugBoundingDraw;
		// 共通リソース
		CommonResources* m_commonResources;

		//プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
		//ベーシックエフェクト
		std::unique_ptr<DirectX::BasicEffect> m_effect;
		//インプットレイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;

	};	



}
