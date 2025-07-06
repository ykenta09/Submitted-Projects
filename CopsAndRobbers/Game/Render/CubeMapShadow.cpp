#include "pch.h"
#include "CubeMapShadow.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/InputManager.h"
#include <Libraries/Microsoft/ReadData.h>
#include <d3dcompiler.h>

//仮ヘッダー
#include "Libraries/yamadaLib/Resources.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="size">キューブマップのサイズ</param>
CubeMapShadow::CubeMapShadow(CommonResources* resources)
   :
   m_commonResources(resources)
{
   InitializeResources();
}

CubeMapShadow::~CubeMapShadow()
{
   Release();
}

/// <summary>
/// リソースの初期化
/// </summary>
void CubeMapShadow::InitializeResources()
{
   //ライトの位置（仮）
   m_lightPosition = DirectX::SimpleMath::Vector3(5.0f, 5.0f, 0.0f);
   //ライトの回転
   m_lightQuaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
      DirectX::XMConvertToRadians(-90.0f), DirectX::XMConvertToRadians(45.0f), 0.0f);

   //ライトの角度
   m_lightTheta = 90.0f;

   m_wallPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

   auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
   LoadShader(device);
   CreateConstanBuffer(device);
   CreateShadowMapObject(device);
}

void CubeMapShadow::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
   auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
   auto states = m_commonResources->GetCommonStates();

   // ----------------------------------------------------------- //
   // ライト空間のビュー行列と射影行列を作成する
   // ----------------------------------------------------------- //

   /* 手順４ */

   //ライトの向いている方向を調整する
   const DirectX::SimpleMath::Vector3 lightDir = DirectX::SimpleMath::Vector3::Transform(
	  DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightQuaternion);

   //ライト空間のビュー行列を計算する
   const DirectX::SimpleMath::Matrix lightView = DirectX::SimpleMath::Matrix::CreateLookAt(
	  m_lightPosition,
	  m_lightPosition + lightDir,
	  DirectX::SimpleMath::Vector3::UnitY
   );

   //ライト空間の射影行列を計算する
   const DirectX::SimpleMath::Matrix lightProjection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
	  DirectX::XMConvertToRadians(m_lightTheta),
	  1.0f,
	  0.1f,
	  100.0f
   );

   // ----------------------------------------------------------- //
   // 定数バッファを更新する
   // ----------------------------------------------------------- //

   /* 手順５ */

   //GPUが使用するリソースのメモリにCPUからアクセスする際に利用する構造体
   D3D11_MAPPED_SUBRESOURCE mappedResource;

   //定数バッファをマップする
   context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

   //定数バッファに送るデータを準備する
   cbLight* cb = static_cast<cbLight*>(mappedResource.pData);
   const DirectX::SimpleMath::Matrix viewProj = lightView * lightProjection;
   cb->lightViewProjection = viewProj;
   cb->lightPosition = m_lightPosition;
   cb->lightDirection = lightDir;
   cb->lightAmbient = DirectX::SimpleMath::Color(0.2f, 0.2f, 0.2f);

   //マップを解除する
   context->Unmap(m_constantBuffer.Get(), 0);

   // ----------------------------------------------------------- //
   // Pass1 描画ターゲットを切り替えて、シャドウマップを作成する
   // ----------------------------------------------------------- //

   /* 手順６ */

   //必要な○○viewを取得する
   auto rtv = m_shadowMapRT->GetRenderTargetView();
   //auto srv = m_shadowMapRT->GetShaderResourceView();
   auto dsv = m_shadowMapDS->GetDepthStencilView();

   //シャドウマップ用のRTVとDSVをクリアする
   context->ClearRenderTargetView(rtv, DirectX::Colors::White);
   context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

   //RTVとDSVをシャドウマップ用に変更する
   context->OMSetRenderTargets(1, &rtv, dsv);

   //ビューポートをシャドウマップ用に変更する
   const D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
   context->RSSetViewports(1, &vp);

   // ----------------------------------------------------------- //
   // 影になるモデルを描画する
   // ----------------------------------------------------------- //

   /* 手順７ */
   DirectX::SimpleMath::Matrix world;

   //動くトーラス
   world = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(7.0f, 10.0f, 1.0f)) *
	  DirectX::SimpleMath::Matrix::CreateTranslation(m_wallPosition);
   m_wallModel->Draw(context, *states, world, lightView, lightProjection, false, [&]
	  {
		 context->VSSetShader(m_vsShadowMap.Get(), nullptr, 0);
		 context->PSSetShader(m_psShadowMap.Get(), nullptr, 0);
	  });

   //レンダーターゲットとビューポートを元に戻す
   auto renderTarget = m_commonResources->GetDeviceResources()->GetRenderTargetView();
   auto depthStencil = m_commonResources->GetDeviceResources()->GetDepthStencilView();

   context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
   context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
   context->OMSetRenderTargets(1, &renderTarget, depthStencil);
   auto const viewport = m_commonResources->GetDeviceResources()->GetScreenViewport();
   context->RSSetViewports(1, &viewport);

   // === 通常描画 ===
   m_wallModel->Draw(context, *states, world, view, proj, false, [&]()
	  {

	  });

}

/// <summary>
/// シェーダをロードする
/// </summary>
/// <param name="device">デバイス</param>
void CubeMapShadow::LoadShader(ID3D11Device* device)
{
   // シェーダを読み込むためのblob
   std::vector<uint8_t> blob;

   // シャドウマップ用頂点シェーダをロードする
   blob = DX::ReadData(L"Resources/Shaders/ShadowMap/ShadowMapDepthVS.cso");
   DX::ThrowIfFailed(
      device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_vsShadowMap.ReleaseAndGetAddressOf())
   );

   // シャドウマップ用ピクセルシェーダをロードする
   blob = DX::ReadData(L"Resources/Shaders/ShadowMap/ShadowMapDepthPS.cso");
   DX::ThrowIfFailed(
      device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_psShadowMap.ReleaseAndGetAddressOf())
   );

}

/// <summary>
/// 定数バッファを作る
/// </summary>
/// <param name="device">デバイス</param>
void CubeMapShadow::CreateConstanBuffer(ID3D11Device* device)
{
   //定数バッファ用のバッファオブジェクトを作る
   D3D11_BUFFER_DESC bufferDesc{};
   bufferDesc.ByteWidth = static_cast<UINT>(sizeof(cbLight));
   bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
   bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   DX::ThrowIfFailed(
      device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf())
   );
}

/// <summary>
/// シャドウマップ用の各種オブジェクトを作成する
/// </summary>
/// <param name="device">デバイス</param>
void CubeMapShadow::CreateShadowMapObject(ID3D11Device* device)
{
   //シャドウマップの大きさを指定する
   const RECT rectShadow = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

   //レンダーテクスチャを作成する
   m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
   m_shadowMapRT->SetDevice(device);
   m_shadowMapRT->SetWindow(rectShadow);

   //深度ステンシルを作成する
   m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
   m_shadowMapDS->SetDevice(device);
   m_shadowMapDS->SetWindow(rectShadow);

   //シャドウマップ用のサンプラーを作成する
   D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
   samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
   samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
   DX::ThrowIfFailed(
      device->CreateSamplerState(&samplerDesc, m_shadowMapSampler.ReleaseAndGetAddressOf())
   );
}

void CubeMapShadow::Release()
{
}

void CubeMapShadow::LoadResources()
{
   m_wallModel = yamadalib::Resources::GetInstance()->GetModel("Wall_01");
}

/// <summary>
/// キューブマップを更新(6面分レンダリング)
/// </summary>
/// <param name="context"></param>
/// <param name="lightPos"></param>
/// <param name="lightRadius"></param>
/// <param name="model"></param>
//void CubeMapShadow::UpdateCubeMap(
//   ID3D11DeviceContext* context,
//   const DirectX::XMFLOAT3& lightPos,
//   float lightRadius,
//   const DirectX::Model& model)
//{
//   //シャドウマップ用のDSC(深度ステンシルビュー)を別途用意
//   auto dsv = m_shadowMapDS->GetDepthStencilView();
//   auto rtv = m_shadowMapRT->GetRenderTargetView();
//
//   //シャドウマップ描画パス
//   context->ClearRenderTargetView(rtv, DirectX::Colors::White);
//   context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
//   context->OMSetRenderTargets(1, &rtv, dsv);
//
//   D3D11_VIEWPORT vp = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
//   context->RSSetViewports(1, &vp);
//
//   context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
//   context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);)
//
//   // 6方向のビュー・プロジェクション行列を作成
//
//   // モデル描画（メッシュ毎）
//      for (const auto& mesh : model.meshes)
//      {
//         for (const auto& part : mesh->meshParts)
//         {
//            UINT stride = part->vertexStride;
//            UINT offset = 0;
//            context->IASetVertexBuffers(0, 1, part->vertexBuffer.GetAddressOf(), &stride, &offset);
//            context->IASetIndexBuffer(part->indexBuffer.Get(), part->indexFormat, 0);
//            context->DrawIndexed(part->indexCount, part->startIndex, part->vertexOffset);
//         }
//      }
//
//
//   //// 元のDSVに戻す
//   //context->OMSetRenderTargets(0, nullptr, oldDSV);
//   //if (oldDSV) oldDSV->Release();
//}
