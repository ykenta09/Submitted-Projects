#include "pch.h"
#include "Game/Render/CustomModelRenderer.h"
#include "Game/CommonResources.h"
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#include "Libraries/yamadaLib/Resources.h"   
#include "Libraries/yamadaLib/Graphics.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resources">共通リソース</param>
/// <param name="graphics">グラフィックス</param>
CustomModelRenderer::CustomModelRenderer(CommonResources* resources, yamadalib::Graphics* graphics)
{
   m_commonResources = resources;
   m_graphics = graphics;
   // シェーダーと入力レイアウトの生成
   LoadShaders(m_commonResources->GetDeviceResources()->GetD3DDevice());
   //ポイントライトをクリアする
   ClearPointLightsParameter();

   //視錐台を生成
   m_frustum = std::make_unique<FrustumCuller>();

   m_cubeMapShadow = std::make_unique<CubeMapShadow>(m_commonResources);
}

/// <summary>
/// デストラクタ
/// </summary>
CustomModelRenderer::~CustomModelRenderer()
{
   ClearPointLightsParameter();
}

/// <summary>
/// モデルを描画する
/// </summary>
/// <param name="model">モデル</param>
/// <param name="texture">モデルのテクスチャ</param>
/// <param name="world">行列</param>
void CustomModelRenderer::Draw(const DirectX::Model& model, ID3D11ShaderResourceView* texture, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale)
{
   //描画モデルの中心座標
   DirectX::SimpleMath::Vector3 modelCenter = world.Translation();
   //ポイントライトのソート
   SetClosestLights(modelCenter, MAX_POINT_LIGHT);

   // デバイスコンテキスト・デバイス・コモンステートを取得
   ID3D11DeviceContext* context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
   auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
   DirectX::CommonStates* states = yamadalib::Graphics::GetInstance()->GetCommonStates();

   // ブレンド・深度・ラスタライザステートをセット
   context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
   context->OMSetDepthStencilState(nullptr, 0);
   context->RSSetState(states->CullCounterClockwise());

   // 深度ステンシルステートを明示的に作成・セット（深度テスト有効・書き込み有効・比較関数LESS）
   D3D11_DEPTH_STENCIL_DESC depthDesc = {};
   depthDesc.DepthEnable = TRUE;
   depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
   depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
   Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
   device->CreateDepthStencilState(&depthDesc, &depthState);
   context->OMSetDepthStencilState(depthState.Get(), 0);

   // フォグ用定数バッファを作成・更新
   UpdateConstantBuffers(context, world, uvScale);

   // シェーダー・入力レイアウトをセット
   context->IASetInputLayout(m_inputLayout.Get());
   context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
   context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

   // テクスチャ（SRV）をピクセルシェーダーにセット
   assert(texture != nullptr);
   ID3D11ShaderResourceView* srvs[1] = {
      texture/*,
      m_cubeMapShadow->GetSRV()*/
   };
   context->PSSetShaderResources(0, 1, srvs);
   // サンプラーステートをピクセルシェーダーにセット
   ID3D11SamplerState* samplers[1] = {
      states->LinearWrap()/*,
      m_cubeMapShadow->GetShadowSampler(),
      m_cubeMapShadow->GetDebugSampler()*/
   };
   context->PSSetSamplers(0, 1, samplers);

   // 定数バッファをシェーダーにセット
   context->VSSetConstantBuffers(0, 1, m_matrixBuffer.GetAddressOf());
   context->PSSetConstantBuffers(1, 1, m_fogBuffer.GetAddressOf());
   context->PSSetConstantBuffers(2, 1, m_uvScaleBuffer.GetAddressOf());
   context->PSSetConstantBuffers(3, 1, m_lightBuffer.GetAddressOf());
   context->PSSetConstantBuffers(4, 1, m_pointLightBuffer.GetAddressOf());
   //ポリゴンを描画する
   context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   // モデルの各メッシュ・パートごとに頂点/インデックスバッファをセットし描画
   for (const auto& mesh : model.meshes)
   {
      for (const auto& part : mesh->meshParts)
      {
         UINT stride = part->vertexStride;
         UINT offset = 0;
         context->IASetVertexBuffers(0, 1, part->vertexBuffer.GetAddressOf(), &stride, &offset);
         context->IASetIndexBuffer(part->indexBuffer.Get(), part->indexFormat, 0);
         //描画
         context->DrawIndexed(part->indexCount, part->startIndex, part->vertexOffset);
      }
   }

   // テクスチャスロットをクリア（SRV競合防止）
   ID3D11ShaderResourceView* nullSRV[2] = { nullptr, nullptr };
   context->PSSetShaderResources(0, 2, nullSRV);
}

/// <summary>
/// 定数バッファ（行列・フォグ）の更新
/// </summary>
/// <param name="context">コンテキスト</param>
/// <param name="world">行列</param>
void CustomModelRenderer::UpdateConstantBuffers(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Vector2& uvScale)
{
   // 行列バッファを更新（転置してシェーダーに渡す）
   MatrixBuffer mb = { DirectX::XMMatrixTranspose(world), DirectX::XMMatrixTranspose(m_graphics->GetViewMatrix()), DirectX::XMMatrixTranspose(m_graphics->GetProjectionMatrix())};
   D3D11_MAPPED_SUBRESOURCE mapped;
   context->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &mb, sizeof(mb));
   context->Unmap(m_matrixBuffer.Get(), 0);

   // ライトバッファを更新
   context->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &m_lightParams, sizeof(m_lightParams));
   context->Unmap(m_lightBuffer.Get(), 0);

   // ポイントライトバッファを更新
   context->Map(m_pointLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &m_pointLightParams, sizeof(m_pointLightParams));
   context->Unmap(m_pointLightBuffer.Get(), 0);

   // フォグバッファを更新
   FogBuffer fb = { FOG_COLOR, FOG_START, FOG_END, {0.0f, 0.0f}, m_camera->GetEyePosition() };
   context->Map(m_fogBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &fb, sizeof(fb));
   context->Unmap(m_fogBuffer.Get(), 0);

   //uvスケールバッファの更新
   UVScaleBuffeer uv = { uvScale };
   context->Map(m_uvScaleBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   memcpy(mapped.pData, &uv, sizeof(uv));
   context->Unmap(m_uvScaleBuffer.Get(), 0);
}

/// <summary>
/// シェーダーの読み込み・生成処理
/// </summary>
/// <param name="device">デバイス</param>
void CustomModelRenderer::LoadShaders(ID3D11Device* device)
{
   Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

   // 頂点シェーダーをコンパイル
   HRESULT hr = D3DCompileFromFile(
      L"Resources/Shaders/FogPointLight/FogPointLightVS.hlsl",
      nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
      "main",
      "vs_5_0",
      0, 0, &vsBlob, &errorBlob
   );
   if (FAILED(hr)) {
      if (errorBlob) {
         OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      }
      throw std::runtime_error("VS compile error");
   }

   // ピクセルシェーダーをコンパイル
   hr = D3DCompileFromFile(
      L"Resources/Shaders/FogPointLight/FogPointLightPS.hlsl",
      nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
      "main",
      "ps_5_0",
      0, 0, &psBlob, &errorBlob
   );
   if (FAILED(hr)) {
      if (errorBlob) {
         OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      }
      throw std::runtime_error("PS compile error");
   }

   // シェーダーオブジェクト生成
   device->CreateVertexShader(
      vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
   device->CreatePixelShader(
      psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

   // 入力レイアウト生成（POSITION/NORMAL/TEXCOORDの頂点構造に対応）
   D3D11_INPUT_ELEMENT_DESC layout[] = {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
   };
   device->CreateInputLayout(
      layout, 4,
      vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
      m_inputLayout.GetAddressOf());

   //バッファーを生成する
   CreateBuffer(device);
}

/// <summary>
/// バッファを生成する
/// </summary>
/// <param name="device">デバイス</param>
void CustomModelRenderer::CreateBuffer(ID3D11Device* device)
{
   // 定数バッファ生成（行列バッファ)
   D3D11_BUFFER_DESC mbDesc = {};
   mbDesc.ByteWidth = sizeof(MatrixBuffer);
   mbDesc.Usage = D3D11_USAGE_DYNAMIC;
   mbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   mbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&mbDesc, nullptr, m_matrixBuffer.GetAddressOf());

   //ライトバッファ
   D3D11_BUFFER_DESC lbDesc = {};
   lbDesc.ByteWidth = sizeof(LightBuffer);
   lbDesc.Usage = D3D11_USAGE_DYNAMIC;
   lbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   lbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&lbDesc, nullptr, m_lightBuffer.GetAddressOf());

   //ポイントライトバッファ
   D3D11_BUFFER_DESC plDesc = {};
   plDesc.ByteWidth = sizeof(PointLightBuffer);
   plDesc.Usage = D3D11_USAGE_DYNAMIC;
   plDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   plDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&plDesc, nullptr, m_pointLightBuffer.GetAddressOf());

   //フォグバッファ
   D3D11_BUFFER_DESC fbDesc = {};
   fbDesc.ByteWidth = sizeof(FogBuffer);
   fbDesc.Usage = D3D11_USAGE_DYNAMIC;
   fbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   fbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&fbDesc, nullptr, m_fogBuffer.GetAddressOf());

   //UVバッファ
   D3D11_BUFFER_DESC uvDesc = {};
   uvDesc.ByteWidth = sizeof(UVScaleBuffeer);
   uvDesc.Usage = D3D11_USAGE_DYNAMIC;
   uvDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   uvDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   device->CreateBuffer(&uvDesc, nullptr, m_uvScaleBuffer.GetAddressOf());
}

/// <summary>
/// ライトパラメータを設定する
/// </summary>
/// <param name="direction">ライトの方向</param>
/// <param name="diffuse">ライトのディフューズ</param>
/// <param name="ambient">アンビエントカラー</param>
void CustomModelRenderer::SetLightParameters(
   const DirectX::SimpleMath::Vector3& direction,
   const DirectX::SimpleMath::Vector4& diffuse,
   const DirectX::SimpleMath::Vector4& ambient)
{
   m_lightParams.LightDirection = direction;
   m_lightParams.LightDiffuseColor = diffuse;
   m_lightParams.AmbientColor = ambient;
}


/// <summary>
/// モデルの中心位置から近いライトを選ぶ
/// </summary>
/// <param name="modelPos">モデルの中心座標</param>
/// <param name="maxLights">シェーダーに渡すライトの最大数</param>
void CustomModelRenderer::SetClosestLights(const DirectX::SimpleMath::Vector3& modelPos, int maxLights)
{
   //ポイントライトのパラメータをクリアする
   ClearPointLightsParameter();

   // 距離でソートするための一時配列
   struct LightInfo
   {
      PointLight light;
      float distance;
   };
   std::vector<LightInfo> visibleLights;

   //視錐台を作成する
   DirectX::BoundingFrustum frustum = m_frustum->CreateViewFrustum(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

   //視錐台カリングと距離計算
   for (const auto& light : m_allPointLights)
   {
      //ライトの影響範囲を考慮したバウンディングスフィア
      DirectX::BoundingSphere sphere;
      sphere.Center = light.position;
      sphere.Radius = light.range;

      //視錐台との交差判定
      if (m_frustum->IsVisible(frustum, sphere))
      {
         float distance = (modelPos - light.position).Length();
         visibleLights.push_back({ light, distance });
      }
   }

   //距離でソート(昇順)
   std::sort(visibleLights.begin(), visibleLights.end(), []
   (const LightInfo& a, const LightInfo& b) {
         return a.distance < b.distance;
      });

   //渡すポイントライトの数を制限
   int count = std::min(maxLights, (int)visibleLights.size());
   m_pointLightParams.numLights = count;
   for (int i = 0; i < count; i++)
   {
         m_pointLightParams.pointLights[i] = visibleLights[i].light;
   }
}

/// <summary>
/// ポイントライトのパラメータを設定する
/// </summary>
/// <param name="lights">設定するポイントライト</param>
void CustomModelRenderer::SetPointLightParameters(const PointLight& light)
{
   m_allPointLights.push_back(light);
}

/// <summary>
/// ポイントライトのパラメータをクリアする
/// </summary>
void CustomModelRenderer::ClearPointLightsParameter()
{
   for (int i = 0; i < MAX_POINT_LIGHT; i++)
   {
      m_pointLightParams.pointLights[i] = PointLight();
   }
   m_pointLightParams.numLights = 0;
}

/// <summary>
/// 全てのポイントライトをクリアする
/// </summary>
void CustomModelRenderer::ClearAllPointLights()
{
   m_allPointLights.clear();
}

void CustomModelRenderer::UpdateShadowMap(const DirectX::Model& model)
{
   UNREFERENCED_PARAMETER(model);
   //auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

   //// ポイントライトごとにキューブマップを更新
   //for (int i = 0; i < m_pointLightParams.numLights; ++i)
   //{
   //   //ライトを取得する
   //   const auto& light = m_pointLightParams.pointLights[i];
   //   //m_cubeMapShadow->UpdateCubeMap(
   //   //   context,
   //   //   light.position,
   //   //   light.range,
   //   //   model
   //   //);
   //}
}