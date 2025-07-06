#include "pch.h"
#include "Game/Shadow/Shadow.h"
#include "Libraries/yamadaLib/Resources.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Shadow::Shadow()
    : 
   m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Shadow::~Shadow()
{
}

/// <summary>
/// ����������
/// </summary>
void Shadow::Initialize()
{
   //�f�o�C�X���擾����
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    // �e�̃e�N�X�`����ǂݍ���
    m_texture = yamadalib::Resources::GetInstance()->GetTexture("Shadow");
    // �x�[�V�b�N�G�t�F�N�g��ݒ肷��
    m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
    m_basicEffect->SetVertexColorEnabled(false);
    m_basicEffect->SetTextureEnabled(true);
    m_basicEffect->SetLightingEnabled(false);

    // ���̓��C�A�E�g��ݒ肷��
    DX::ThrowIfFailed(
       DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionTexture>(
            device,
            m_basicEffect.get(),
            m_inputLayout.ReleaseAndGetAddressOf()
        )
    );

    // �[�x�X�e���V���o�b�t�@��ݒ肷��
    D3D11_DEPTH_STENCIL_DESC desc{};
    // �e�F�`���̃X�e���V���o�b�t�@�ƎQ�ƒl�������Ƃ��e��`�悷��
    desc.DepthEnable = true;                                    // �[�x�e�X�g���s��
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;            // �[�x�o�b�t�@���X�V����
    desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;                // �[�x�l�ȉ��Ȃ�X�V����

    desc.StencilEnable = true;                                    // �X�e���V���e�X�g���s��
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;        // 0xff
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;    // 0xff

    // �\��
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;        // �Q�ƒl�ƃX�e���V���l�����l�Ȃ�
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;        // OK�@�������Ȃ�
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;        // NG�@�������Ȃ�
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;    // NG�@�������Ȃ�

    // ���ʂ������ݒ�
    desc.BackFace = desc.FrontFace;
    // �[�x�X�e���V���X�e�[�g���쐬����
    DX::ThrowIfFailed(
        device->CreateDepthStencilState(&desc, m_graphics->GetDepthStencilState().ReleaseAndGetAddressOf())
    );
}


/// <summary>
/// �`�悷��
/// </summary>
/// <param name="position">�`�悷����W</param>
/// <param name="radius">���a</param>
void Shadow::Render(const DirectX::SimpleMath::Vector3& position, float radius)
{
    // �e�|���S���̒��_�����`����
    DirectX::VertexPositionTexture vertices[] = {
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(1.0f, 0.0f)),
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(0.0f, 1.0f)),
        DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector2(1.0f, 1.0f)),
    };

    // �e�|���S���̃C���f�b�N�X�����`����FDrawIndexed�ŕ`�悷��Ƃ��ɕK�v
    uint16_t indexes[] = { 0, 1, 2, 1, 3, 2 };

    // �e�|���S���̒��_����ݒ肷��
    vertices[0].position = DirectX::SimpleMath::Vector3(-radius, 0.05f, -radius) + position;
    vertices[1].position = DirectX::SimpleMath::Vector3(radius, 0.05f, -radius) + position;
    vertices[2].position = DirectX::SimpleMath::Vector3(-radius, 0.05f, radius) + position;
    vertices[3].position = DirectX::SimpleMath::Vector3(radius, 0.05f, radius) + position;

    // �Ăяo�����ł̐ݒ�
    auto states = m_graphics->GetCommonStates();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    context->OMSetDepthStencilState(states->DepthRead(), 1); // DepthDefault �Ńf�v�X�e�X�g��L����
    // �e�|���S����`�悷��
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
    // �f�t�H���g�̃f�v�X�e���V���X�e�[�g�ɖ߂�
    context->OMSetDepthStencilState(nullptr, 0);  // �f�v�X�e���V���X�e�[�g���������Č��ɖ߂�
}


/// <summary>
/// �㏈��������
/// </summary>
void Shadow::Finalize()
{
}
