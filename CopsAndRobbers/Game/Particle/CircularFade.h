//--------------------------------------------------------------------------------------
//	File: CircularFade.h
//
//	パーティクルクラス
//	未拡張の場合はPosition情報のみ
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class CircularFade
{
public:
   //	データ受け渡し用コンスタントバッファ(送信側)
   struct ConstBuffer
   {
	  DirectX::SimpleMath::Matrix		matWorld;
	  DirectX::SimpleMath::Matrix		matView;
	  DirectX::SimpleMath::Matrix		matProj;
	  DirectX::SimpleMath::Vector4		Diffuse;
	  DirectX::SimpleMath::Vector4		time;
   };

public:
   //	関数
   static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
   //フェードが終わったかを取得する
   bool GetFade() { return m_isFadeEnd; }
   //フェードの設定をする
   void SetFadeIn(bool flag) { m_isFadeIn = flag; }
   void SetFadeOut(bool flag) { m_isFadeOut = flag; }
   //時間を設定する
   void SetTime(const float& time) { m_time = time; }
public:
   CircularFade();
   ~CircularFade();

   void LoadTexture(const wchar_t* path);

   void Create(DX::DeviceResources* pDR);

   void Render();
private:

   void CreateShader();
private:
   //	変数
   DX::DeviceResources* m_pDR;

   Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

   DX::StepTimer                           m_timer;
   //	入力レイアウト
   Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

   //	プリミティブバッチ
   std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
   //	コモンステート
   std::unique_ptr<DirectX::CommonStates> m_states;
   //	テクスチャハンドル
   std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
   //	テクスチャハンドル
   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
   //	頂点シェーダ
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
   //	ピクセルシェーダ
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
   //	ジオメトリシェーダ
   Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

   DirectX::SimpleMath::Matrix m_world;
   DirectX::SimpleMath::Matrix m_view;
   DirectX::SimpleMath::Matrix m_proj;

   float m_time;

   //フェードインが終わった
   bool m_isFadeIn;
   bool m_isFadeOut;
   bool m_isFadeEnd;


};
