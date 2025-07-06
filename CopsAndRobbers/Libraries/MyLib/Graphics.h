/*
	@file	Graphics.h
	@brief	グラフィックスクラス
*/	
#pragma once
#include "DeviceResources.h"

namespace mylib
{
	class Graphics final
	{
	public:
		//Graphicsクラスのインスタンスを取得する
		static mylib::Graphics* const GetInstance();

		//スクリーンサイズを取得する
		void GetScreenSize(int& width, int& height) const { width = m_screenW; height = m_screenH; }
		//スクリーンサイズを設定する
		void SetScreenSize(const int& width, const int& height) { m_screenW = width; m_screenH = height; }
		//デバイスリソースを取得する
		DX::DeviceResources* GetDeviceResources() const { return m_deviceResources.get(); }
		// プリミティブバッチを取得する
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatch() const { return m_primitiveBatch.get(); }
		// インプットレイアウトを取得する
		ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
		// ベイシックエフェクトを取得する
		DirectX::BasicEffect* GetBasicEffect() { return m_basicEffect.get(); }
		// コモンステートを取得する
		DirectX::CommonStates* GetCommonStates() const { return m_commonStates.get(); }
		// スプライトバッチを取得する
		DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
		// スプライトフォントを取得する
		DirectX::SpriteFont* GetFont() { return m_spriteFont.get(); }
		//// エフェクトファクトリを取得する
		//DirectX::EffectFactory* GetFX() const { return m_effectFactory.get(); }
		// ビュー行列を設定する
		void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
		// ビュー行列を取得する
		const DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_view; };
		// 射影行列を設定する
		void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
		// 射影行列を取得する
		const DirectX::SimpleMath::Matrix& GetProjectionMatrix() { return m_projection; };
	public:
		//プリミティブバッチ描画を開始
		void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
		//プリミティブバッチ描画を終了
		void DrawPrimitiveEnd();
		//モデルを描画する
		void DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& deptBuffer = true);
		Graphics();
		~Graphics();
		//初期化
		void Initialize();
	private:
		//Graphicsクラスのインスタンスへのポインタ
		static std::unique_ptr<mylib::Graphics> m_graphics;
		//デバイスリソース
		std::unique_ptr<DX::DeviceResources> m_deviceResources;
		//コモンステート
		std::unique_ptr<DirectX::CommonStates> m_commonStates;
		//スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		//スプライトフォント
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
		//ベーシックエフェクト	
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
		//プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
		//ラスタライザーステート
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterrizerState;
		//入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		//スクリーンサイズ
		int m_screenW, m_screenH;
		//ビュー行列
		DirectX::SimpleMath::Matrix m_view;
		//射影行列
		DirectX::SimpleMath::Matrix m_projection;
		//デバイス
		ID3D11Device* m_device;
		//デバイスコンテキスト
		ID3D11DeviceContext* m_context;
		
	};
}
