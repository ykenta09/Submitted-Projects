#include "pch.h"
#include "Game/Shadow/Shadow.h"
#include "Libraries/yamadaLib/Resources.h"


/// <summary>
/// コンストラクタ
/// </summary>
Shadow::Shadow()
    : 
   m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// デストラクタ
/// </summary>
Shadow::~Shadow()
{
}

/// <summary>
/// 初期化する
/// </summary>
void Shadow::Initialize()
{
   //デバイスを取得する
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    // 影のテクスチャを読み込む
    m_texture = yamadalib::Resources::GetInstance()->GetTexture("Shadow");
    // ベーシックエフェクトを設定する
    m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
    m_basicEffect->SetVertexColorEnabled(false);
    m_basicEffect->SetTextureEnabled(true);
    m_basicEffect->SetLightingEnabled(false);

    // 入力レイアウトを設定する
    DX::ThrowIfFailed(
       DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionTexture>(
            device,
            m_basicEffect.get(),
            m_inputLayout.ReleaseAndGetAddressOf()
        )
    );

    // 深度ステンシルバッファを設定する
    D3D11_DEPTH_STENCIL_DESC desc{};
    // 影：描画先のステンシルバッファと参照値が同じとき影を描画する
    desc.DepthEnable = true;                                    // 深度テストを行う
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;            // 深度バッファを更新する
    desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;                // 深度値以下なら更新する

    desc.StencilEnable = true;                                    // ステンシルテストを行う
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;        // 0xff
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;    // 0xff

    // 表面
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;        // 参照値とステンシル値が同値なら
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;        // OK　何もしない
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;        // NG　何もしない
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;    // NG　何もしない

    // 裏面も同じ設定
    desc.BackFace = desc.FrontFace;
    // 深度ステンシルステートを作成する
    DX::ThrowIfFailed(
        device->CreateDepthStencilState(&desc, m_graphics->GetDepthStencilState().ReleaseAndGetAddressOf())
    );
}


/// <summary>
/// 描画する
/// </summary>
/// <param name="position">描画する座標</param>
/// <param name="radius">半径</param>
void Shadow::Render(const DirectX::SimpleMath::Vector3& position, float radius)
{
    // 影ポリゴンの頂点情報を定義する
    DirectX::VertexPositionTexture vertices[] = {
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(1.0f, 0.0f)),
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(0.0f, 1.0f)),
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(1.0f, 1.0f)),
    };

    // 影ポリゴンのインデックス情報を定義する：DrawIndexedで描画するときに必要
    uint16_t indexes[] = { 0, 1, 2, 1, 3, 2 };

    // 影ポリゴンの頂点情報を設定する
    vertices[0].position = DirectX::SimpleMath::Vector3(-radius, 0.05f, -radius) + position;
    vertices[1].position = DirectX::SimpleMath::Vector3(radius, 0.05f, -radius) + position;
    vertices[2].position = DirectX::SimpleMath::Vector3(-radius, 0.05f, radius) + position;
    vertices[3].position = DirectX::SimpleMath::Vector3(radius, 0.05f, radius) + position;

    // 呼び出し側での設定
    auto states = m_graphics->GetCommonStates();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    context->OMSetDepthStencilState(states->DepthRead(), 1); // DepthDefault でデプステストを有効化
    // 影ポリゴンを描画する
    m_graphics->DrawPrimitivesTexture(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(), m_texture,
        [this, indexes, vertices](DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>&)
        {
          m_graphics->GetPrimitiveBatchTexture()->DrawIndexed(
                D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
                indexes,
                _countof(indexes),
                vertices,
                _countof(vertices)
            );
        });
    // デフォルトのデプステンシルステートに戻す
    context->OMSetDepthStencilState(nullptr, 0);  // デプステンシルステートを解除して元に戻す
}


/// <summary>
/// 後処理をする
/// </summary>
void Shadow::Finalize()
{
}
