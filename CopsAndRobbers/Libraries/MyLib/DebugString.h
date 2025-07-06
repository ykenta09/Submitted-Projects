/*
	@file	DebugString.h
	@brief	�f�o�b�O������̕`��N���X
	
	@details	AddString(C����́uprintf�v�֐��Ɠ����t�H�[�}�b�g�w��)
*/
#pragma once
#include <SpriteFont.h>
#include <vector>
#include <string>

namespace mylib
{
	class DebugString
	{
	private:
		// �f�[�^�����o
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
		std::vector<std::string> m_strings;
		float m_fontHeight;
		DirectX::SimpleMath::Color m_color;


	public:
		// �R���X�g���N�^
		DebugString(
			ID3D11Device* device,
			ID3D11DeviceContext* context,
			const wchar_t* fontFilePath
		);

		// �f�X�g���N�^
		~DebugString() = default;

		// �`�悷�镶�����o�^����
		void AddString(const char* format, ...);

		// �f�o�b�O�������`�悷��
		void Render(DirectX::CommonStates* states);

		// �`�悷�镶����̐F���w�肷��
		void SetColor(DirectX::FXMVECTOR color = DirectX::Colors::White)
		{
			m_color = color;
		}
	};
}
