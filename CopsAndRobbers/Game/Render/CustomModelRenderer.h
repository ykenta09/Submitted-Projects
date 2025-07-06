/*
	@file	CustomModelRenderer.h
	@brief	カスタムモデルレンダークラス
*/
#pragma once
#include <Game/Interface/ICamera.h>
#include "CubeMapShadow.h"
#include  "Game/FrustumCuller.h"

#ifndef CUSTOM_MODEL_RENDERER_DEFINED
#define CUSTOM_MODEL_RENDERER_DEFINED

class CommonResources;
class CameraManager;
namespace yamadalib { class Graphics; } // ★前方宣言

class CustomModelRenderer
{
public:
   static constexpr	 DirectX::SimpleMath::Vector4 FOG_COLOR = { 0.1f, 0.1f, 0.2f, 0.3f };	 //フォグの色
   static constexpr	 float FOG_START = { 5.0f };		 //フォグの開始距離
   static constexpr	 float FOG_END = { 25.0f };			 //フォグの終了距離
   static constexpr int MAX_POINT_LIGHT = { 5 };		 //シェーダーに送るライトの最大数
public:
   //マテリアルバッファ
   struct MatrixBuffer
   {
	  DirectX::SimpleMath::Matrix World;
	  DirectX::SimpleMath::Matrix View;
	  DirectX::SimpleMath::Matrix Projection;
   };

   //ライトバッファ
   struct LightBuffer
   {
	  DirectX::SimpleMath::Vector3 LightDirection; //ワールド空間ライト方向
	  float padding;
	  DirectX::SimpleMath::Vector4 LightDiffuseColor;	 //ライトのディフューズ色
	  DirectX::SimpleMath::Vector4 AmbientColor;		 //アンビエント色
   };

   //ポイントライトの構造体
   struct PointLight
   {
	  DirectX::SimpleMath::Vector3 position = { 0.0f, 0.0f, 0.0f};
	  float range = 0.0f;
	  DirectX::SimpleMath::Vector3 color = { 0.0f, 0.0f, 0.0f };
	  float intensity = 1.5f;
   };

   //ポイントライトバッファ
   struct PointLightBuffer
   {
	  PointLight pointLights[MAX_POINT_LIGHT];
	  int numLights;
	  float padding[3];
   };
   //フォグバッファ
   struct FogBuffer
   {
	  DirectX::SimpleMath::Vector4 FogColor;
	  float FogStart;
	  float FogEnd;
	  float padding[2];
	  DirectX::SimpleMath::Vector3 CameraPosition;
	  float padding2;
   };

   //UVバッファ
   struct UVScaleBuffeer
   {
	  DirectX::SimpleMath::Vector2 uvScale;
	  float padding[2];
   };

   //カメラを設定する
   void SetCamera(ICamera* camera) { m_camera = camera; }

   CubeMapShadow* GetCubeMap() { return m_cubeMapShadow.get(); }
public:
	//コンストラクタ
    CustomModelRenderer(CommonResources* resources, yamadalib::Graphics* graphics);
	 //デストラクタ
	~CustomModelRenderer();

	//モデル描画
	void Draw(const DirectX::Model& model, ID3D11ShaderResourceView* texture, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale = DirectX::SimpleMath::Vector2::One);

	//行列・バッファの更新
	void UpdateConstantBuffers(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale);

	//シェーダーを読み込み、生成
	void LoadShaders(ID3D11Device* device);

	//バッファを生成する
	void CreateBuffer(ID3D11Device* device);

	//ライトバッファのパラメータを設定する
	void SetLightParameters(const DirectX::SimpleMath::Vector3& direction, const DirectX::SimpleMath::Vector4& diffuse, const DirectX::SimpleMath::Vector4& ambient);

	//ポイントライトのパラメータを設定する
	void SetPointLightParameters(const PointLight& lights);

	//ポイントライトをクリアする
	void SetClosestLights(const DirectX::SimpleMath::Vector3& modelPos, int maxLights);

	//ポイントライトのパラメータをクリアする
	void ClearPointLightsParameter();

	//全てのポイントライトをクリアする
	void ClearAllPointLights();

	void UpdateShadowMap(const DirectX::Model& model);
private:
   // 共通リソース
   CommonResources* m_commonResources;
   //グラフィックス
   yamadalib::Graphics* m_graphics;
   //カメラ
   ICamera* m_camera;

   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;		 //頂点シェーダー
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;			 //ピクセルシェーダー
   Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;			 //入力レイアウト
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;				 //マテリアルバッファ
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;				 //ライトバッファ
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_pointLightBuffer;			 //ポイントライトバッファ
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_fogBuffer;				 //フォグバッファ
   Microsoft::WRL::ComPtr<ID3D11Buffer> m_uvScaleBuffer;			 //uvバッファ

   LightBuffer m_lightParams;				 // ライトパラメータ保持用
   PointLightBuffer m_pointLightParams;		 //ポイントライトパラメータ
   std::vector<PointLight> m_allPointLights; //全てのポイントライト配列

   std::unique_ptr<FrustumCuller> m_frustum;	//視錐台カリング


   std::unique_ptr<CubeMapShadow> m_cubeMapShadow;
};
#endif // CUSTOM_MODEL_RENDERER_DEFINED

