/*
	@file	Fragment.h
	@brief	リザルトシーンクラス
*/
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

class Fragment
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
	  DirectX::SimpleMath::Vector4		distance;
   };


public:
   //	関数
   static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:
   void SetPosition(DirectX::SimpleMath::Vector3 player, DirectX::SimpleMath::Vector3 enemy)
   {
	  float distance = (player - enemy).Length();
	  if (m_currentDistance > distance)
	  {
		 m_currentDistance = distance;
	  }
   }
public:
   Fragment();
   ~Fragment();

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
   //	頂点シェーダ
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
   //	ピクセルシェーダ
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
   //	ジオメトリシェーダ
   Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

   DirectX::SimpleMath::Matrix m_world;
   DirectX::SimpleMath::Matrix m_view;
   DirectX::SimpleMath::Matrix m_proj;

   //現在の距離と前回の距離
   float m_currentDistance;
   float m_prevDistance;

   float time;
};
