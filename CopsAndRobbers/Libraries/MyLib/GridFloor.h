/*
	@file	GridFloor.h
	@brief	�O���b�h����`�悷��N���X
*/
#pragma once

namespace mylib
{
	class GridFloor
	{
	private:
		// �f�[�^�����o
		DirectX::CommonStates* m_commonStates;
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		DirectX::SimpleMath::Color m_color;
		float m_size;
		size_t m_divs;


	public:
		// �R���X�g���N�^
		GridFloor(
			ID3D11Device* device,
			ID3D11DeviceContext* context,
			DirectX::CommonStates* states,
			const DirectX::FXMVECTOR& color = DirectX::Colors::LightGray,
			float size = 10.0f,	// ���̑傫��
			size_t divs = 10	// ������
		);

		// �f�X�g���N�^
		~GridFloor() = default;

		// ����`�悷��
		void Render(
			ID3D11DeviceContext* context,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& projection
		);

		// ���̐F���w�肷��
		void SetColor(const DirectX::FXMVECTOR& color)
		{
			m_color = color;
		}
	};
}
