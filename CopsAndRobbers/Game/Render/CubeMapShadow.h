/*
	@file	CustomModelRenderer.h
	@brief	カスタムモデルレンダークラス
*/
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Libraries/MyLib/RenderTexture.h"
#include "Libraries/MyLib/DepthStencil.h"

#ifndef CUBE_MAP_SHADOW_DEFINED
#define CUBE_MAP_SHADOW_DEFINED

class CommonResources;
namespace yamadalib { class Graphics; } // ★前方宣言

class CubeMapShadow
{
public:
   // シャドウマップのサイズ
   static const int SHADOWMAP_SIZE = 512;

   // ライトの定数バッファ用構造体
   struct cbLight
   {
	  DirectX::XMMATRIX lightViewProjection;	// ライトの投影空間へ座標変換する行列
	  DirectX::XMVECTOR lightPosition;		// ライトの位置
	  DirectX::XMVECTOR lightDirection;		// ライトの方向
	  DirectX::XMVECTOR lightAmbient;			// ライトの環境光
   };

public:
	//コンストラクタ
    CubeMapShadow(CommonResources* resources);
   //デストラクタ
	~CubeMapShadow();


	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// キューブマップを更新（光源位置から6方向に描画）
	void UpdateCubeMap(
	   ID3D11DeviceContext* context,
	   const DirectX::XMFLOAT3& lightPos,
	   float lightRadius,
	   const DirectX::Model& model);

	void LoadShader(ID3D11Device* device);

	void CreateConstanBuffer(ID3D11Device* device);

	void CreateShadowMapObject(ID3D11Device* device);

	void Release();

	void LoadResources();

private:
   void InitializeResources();


private:
   // 共通リソース
   CommonResources* m_commonResources;
   //グラフィックス
   yamadalib::Graphics* m_graphics;

   // 定数バッファへのポインタ
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

   // 頂点シェーダ
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;

   // ピクセルシェーダ
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;

   // シャドウマップ用（レンダーテクスチャ）
   std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

   // シャドウマップ用（デプスステンシル）
   std::unique_ptr<DepthStencil> m_shadowMapDS;

   // 頂点シェーダ(シャドウマップ用）
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vsShadowMap;

   // ピクセルシェーダ（シャドウマップ用）
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psShadowMap;

   // サンプラ
   Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;


   // ライトの位置
   DirectX::SimpleMath::Vector3 m_lightPosition;

   // ライトの回転
   DirectX::SimpleMath::Quaternion m_lightQuaternion;

   //ID3D11Device* m_device;
   //ID3D11DeviceContext* m_context;


   // ビュー行列
   DirectX::SimpleMath::Matrix m_view;

   // 射影行列
   DirectX::SimpleMath::Matrix m_proj;


   //仮変数
   DirectX::Model* m_wallModel;

   // ライトの画角：ライトから見た風景を映すカメラの画角
   float m_lightTheta;


   DirectX::SimpleMath::Vector3 m_wallPosition;

};
#endif // CUBE_MAP_SHADOW_DEFINED

