/*
	@file	Graphics.h
	@brief  描画関係のクラス
*/	
#pragma once
#ifndef GRAPHIS_DEFINED
#define GRAPHIS_DEFINED

#include "DeviceResources.h"
#include "Game/Render/CustomModelRenderer.h"

class CommonResources;
//class CustomModelRenderer;

namespace yamadalib
{
	class Graphics final
	{
	public:
		//Graphicsクラスのインスタンスを取得する
		static yamadalib::Graphics* const GetInstance();
		//プリミティブバッチを取得する(VertexPositionColor)
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatch() const { return m_primitiveBatchColor.get(); }
		//VertexPositionColorのプリミティブ描画
		void DrawPrimitivesColor(
			DirectX::CommonStates& states,
			DirectX::SimpleMath::Matrix view,
			DirectX::SimpleMath::Matrix projection,
			std::function<void(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)> drawFunction);
		//プリミティブバッチを取得する(VertexPositionTexture)
		DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* GetPrimitiveBatchTexture() const { return m_primitiveBatchTexture.get(); }
		//VertexPositionTextureのプリミティブ描画
		void DrawPrimitivesTexture(
			DirectX::CommonStates& states,
			DirectX::SimpleMath::Matrix view,
			DirectX::SimpleMath::Matrix projection,
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture,
			std::function<void(DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>&)> drawFunction);

		//デバイスリソースを取得する
		DX::DeviceResources* GetDeviceResources() const { return m_deviceResources.get(); }
		//エフェクトファクトリを取得する
		DirectX::EffectFactory* GetFX() const { return m_effectFactory.get(); }
		//コモンステートを取得する
		DirectX::CommonStates* GetCommonStates() const  { return m_commonStates.get(); }
		//ビュー行列を取得する
		const DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_view; }
		//ビュー行列を設定する
		void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
		//射影行列を取得する
		const DirectX::SimpleMath::Matrix& GetProjectionMatrix() { return m_projection; }
		//射影行列を設定する
		void SetProjectiionMatrix(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }

		//深度ステンシルバッファを取得する
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return m_depthStencilState; }

		Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayoutTexture() { return m_layoutTexture; }
		
		// SpriteBatchクラスのインスタンスを取得する
		DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
		// BasicEffectクラスのインスタンス取得する
		DirectX::BasicEffect* GetBasicEffect() { return m_basicEffectColor.get(); }
		DirectX::BasicEffect* GetBasicEffectTexture() { return m_basicEffectTexture.get(); }
		//SpriteFontクラスのインスタンスを取得する
		DirectX::SpriteFont* GetFont() { return m_spriteFont.get(); }

		//カスタムレンダーを取得する
		CustomModelRenderer* GetCustomModelRenderer() { return m_customRender.get(); };

	public:
		//コンストラクタ
		Graphics();
		//デストラクタ
		~Graphics();
		//初期化
		void Initialize(CommonResources* resources);
		//モデル描画
		void DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world);
		//境界ボックスを描画
		void DrawBoundingBox(const DirectX::BoundingBox& box);
		//境界球を描画
		void DrawBoundingSphere(const DirectX::BoundingSphere& sphere);
		// 線分を描画する(XZ平面)
		void DrawLine(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color = DirectX::Colors::White);
		//線分を描画する(2D)
		void DrawLine2D(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color = DirectX::Colors::White);
		// 線分を描画する
		void DrawLine(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& vector, const DirectX::FXMVECTOR& color = DirectX::Colors::White);
		// 円を描画する(XZ平面)
		void DrawCircle(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& color = DirectX::Colors::White, const int& split = 32);
		//2Dで円を描画する
		void DrawCircle2D(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& color = DirectX::Colors::White, const int& split = 32);
		// 円を描画する
		void DrawCircle(const DirectX::SimpleMath::Vector3& center, const float& radius, const DirectX::FXMVECTOR& color = DirectX::Colors::White, const int& split = 32);
		//テクスチャを描画する
		void DrawTexture(
			const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture,
			const DirectX::SimpleMath::Vector2& position,
			const RECT* srcRect, 
			const DirectX::XMVECTORF32& color = DirectX::Colors::White,
		    const float rotation = 0.0f,
		    const DirectX::SimpleMath::Vector2& origin = DirectX::SimpleMath::Vector2::Zero,
		    const DirectX::SimpleMath::Vector2& scale = DirectX::SimpleMath::Vector2::One);

		//テクスチャのサイズを取得する
		RECT GetTextureRect(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

		//扇型を描画する
		void DrawFilledSector(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>& batch, const DirectX::SimpleMath::Vector3& center, const DirectX::XMVECTOR& color, const DirectX::SimpleMath::Vector2& point2, const DirectX::SimpleMath::Vector2& point3);

		//ビルボード描画
		void DrawTextureBillboard(ID3D11ShaderResourceView* texture, const DirectX::SimpleMath::Matrix& world, bool Depthflag = true);

		//クリア関数
		void Clear();
	private:
		//Graphisクラスのインスタンスへのポインタ
		static std::unique_ptr<yamadalib::Graphics> m_graphics;
		// 共通リソース
		CommonResources* m_commonResources;

		//カスタムレンダー
		std::unique_ptr<CustomModelRenderer> m_customRender;
		//デバイスリソース
		std::unique_ptr<DX::DeviceResources> m_deviceResources;
		//コモンステート
		std::unique_ptr<DirectX::CommonStates> m_commonStates;
 		//ビュー行列
		DirectX::SimpleMath::Matrix m_view;
		//射影行列（プロジェクション行列）
		DirectX::SimpleMath::Matrix m_projection;
		//デバイス
		ID3D11Device* m_device;
		//デバイスコンテキスト
		ID3D11DeviceContext* m_context;
		//プリミティブバッチ(VertexPositionColor)
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatchColor;
		//プリミティブバッチ(VertexPositionTexture)
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatchTexture;

		// スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		//スプライトフォント
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
		//ベーシックエフェクト
		std::unique_ptr<DirectX::BasicEffect> m_basicEffectColor;
		std::unique_ptr<DirectX::BasicEffect> m_basicEffectTexture;

		//エフェクトファクトリー
		std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
		//インプットレイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layoutColor;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layoutTexture;

		// 深度ステンシルステート
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_stencilState;
		D3D11_DEPTH_STENCIL_DESC m_stencilDesc;
		// 四角形で使用する 頂点情報
		DirectX::VertexPositionTexture	m_vertices[4];
	};	
}

#endif // GRAPHIS_DEFINED
