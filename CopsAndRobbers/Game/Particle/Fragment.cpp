//--------------------------------------------------------------------------------------
//	File: Particle.h
//
//	パーティクルクラス
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Game/Particle/Fragment.h"

#include "Game/Particle/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> Fragment::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

///	<summary>
///	コンストラクタ
///	</summary>
Fragment::Fragment()
   : m_pDR(nullptr)
   , time{ 0.0f },
   m_currentDistance{ 90.0f }
{
}

///	<summary>
///	デストラクタ
///	</summary>
Fragment::~Fragment()
{
}


///	<summary>
///	生成関数
///	</summary>
///	<param name="pDR">ユーザーリソース等から持ってくる</param>
void Fragment::Create(DX::DeviceResources* pDR)
{
   m_pDR = pDR;
   ID3D11Device1* device = pDR->GetD3DDevice();

   //	シェーダーの作成
   CreateShader();

   //	プリミティブバッチの作成
   m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

   m_states = std::make_unique<DirectX::CommonStates>(device);

}

///	<summary>
///	Shader作成部分だけ分離した関数
///	</summary>
void Fragment::CreateShader()
{
   ID3D11Device1* device = m_pDR->GetD3DDevice();

   //	コンパイルされたシェーダファイルを読み込み
   BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
   BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
   BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Fragment/FragmentPS.cso");

   //	インプットレイアウトの作成
   device->CreateInputLayout(&INPUT_LAYOUT[0],
	  static_cast<UINT>(INPUT_LAYOUT.size()),
	  VSData.GetData(), VSData.GetSize(),
	  m_inputLayout.GetAddressOf());

   //	頂点シェーダ作成
   if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
   {//	エラー
	  MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
	  return;
   }

   //	ジオメトリシェーダ作成
   if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
   {//	エラー
	  MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
	  return;
   }
   //	ピクセルシェーダ作成
   if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
   {//	エラー
	  MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
	  return;
   }

   //	シェーダーにデータを渡すためのコンスタントバッファ生成
   D3D11_BUFFER_DESC bd;
   ZeroMemory(&bd, sizeof(bd));
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof(ConstBuffer);
   bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bd.CPUAccessFlags = 0;
   device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

///	<summary>
///	描画関数
///	</summary>
///	<param name="view">ビュー行列</param>
///	<param name="proj">射影行列</param>
void Fragment::Render()
{
   time += 0.025f;

   ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();

   //	頂点情報(板ポリゴンの４頂点の座標情報）
   DirectX::VertexPositionColorTexture vertex[4] =
   {
	   DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f,  0.0f, 0.0f), DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
   };

   //	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
   ConstBuffer cbuff;
   cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
   cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);
   cbuff.time = DirectX::SimpleMath::Vector4(time, 0, 0, 0);
   cbuff.distance = DirectX::SimpleMath::Vector4(m_currentDistance, m_prevDistance, 0, 0);

   //	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
   context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

   //	シェーダーにバッファを渡す
   ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
   context->VSSetConstantBuffers(0, 1, cb);
   context->GSSetConstantBuffers(0, 1, cb);
   context->PSSetConstantBuffers(0, 1, cb);

   //	画像用サンプラーの登録
   ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
   context->PSSetSamplers(0, 1, sampler);

   //	半透明描画指定
   ID3D11BlendState* blendstate = m_states->NonPremultiplied();

   //	透明判定処理
   context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

   //	深度バッファに書き込み参照する
   context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

   //	カリングは左周り
   context->RSSetState(m_states->CullNone());

   //	シェーダをセットする
   context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
   context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
   context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

   //	インプットレイアウトの登録
   context->IASetInputLayout(m_inputLayout.Get());

   //	板ポリゴンを描画
   m_batch->Begin();
   m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);
   m_batch->End();

   //	シェーダの登録を解除しておく
   context->VSSetShader(nullptr, nullptr, 0);
   context->GSSetShader(nullptr, nullptr, 0);
   context->PSSetShader(nullptr, nullptr, 0);

   //今回使った距離を保存しておく
   m_prevDistance = m_currentDistance;
   m_currentDistance = 90.0f;
}

