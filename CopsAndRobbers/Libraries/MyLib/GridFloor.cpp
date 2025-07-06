/*
	@file	GridFloor.cpp
	@brief	�O���b�h����`�悷��N���X
*/
#include "pch.h"
#include "GridFloor.h"
#include "Libraries/Microsoft/DebugDraw.h"
#include <cassert>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
mylib::GridFloor::GridFloor(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	const DirectX::FXMVECTOR& color,
	float size,
	size_t divs
)
	:
	m_commonStates(states),
	m_color{color},
	m_size{size},
	m_divs{divs}
{
	assert(device);
	assert(context);
	assert(states);

	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetTextureEnabled(false);

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);
}

//---------------------------------------------------------
// �O���b�h����`�悷��
//---------------------------------------------------------
void mylib::GridFloor::Render(
	ID3D11DeviceContext* context,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& projection
)
{
	assert(context);

	// �u�����h�X�e�[�g�A�f�v�X�X�e���V���X�e�[�g�A�J�����O���w�肷��
	context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_commonStates->DepthDefault(), 0);
	context->RSSetState(m_commonStates->CullNone());

	// �ϊ��s���ݒ肷��
	Matrix world{ Matrix::Identity };
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);

	// ���̓��C�A�E�g���w�肷��
	context->IASetInputLayout(m_inputLayout.Get());

	// �O���b�h����`�悷��
	m_primitiveBatch->Begin();
	DX::DrawGrid(
		m_primitiveBatch.get(),
		Vector3(m_size / 2.0f, 0.0f, 0.0f),	// ����
		Vector3(0.0f, 0.0f, m_size / 2.0f),	// �c��
		Vector3::Zero,						// origin
		m_divs,								// xdivs
		m_divs,								// ydivs
		m_color								// color
	);
	m_primitiveBatch->End();
}
