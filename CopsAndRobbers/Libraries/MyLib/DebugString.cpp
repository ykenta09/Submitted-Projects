/*
	@file	DebugString.cpp
	@brief	�f�o�b�O������̕`��N���X
*/
#include "pch.h"
#include "DebugString.h"
#include <cstdarg>
#include <cassert>

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
mylib::DebugString::DebugString(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	const wchar_t* fontFilePath
)
	:
	m_strings{}
{
	assert(device);
	assert(context);

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, fontFilePath);

	m_fontHeight = m_spriteFont->GetLineSpacing();
	m_color = DirectX::Colors::White;
}

//---------------------------------------------------------
// �`�悷�镶�����o�^����
//---------------------------------------------------------
void mylib::DebugString::AddString(const char* format, ...)
{
	// �ϒ��������������߂̃f�[�^�^�Fva_list
	va_list args;

	// �o�^���镶����̒��������߂�
	va_start(args, format);
	int length = vsnprintf(nullptr, 0, format, args);
	va_end(args);

	// �o�^���镶����̑傫���𒲐�����
	std::string appendString;
	appendString.resize(length + 1);	// +1 for the '\0'

	// �o�^���镶������쐬����
	va_start(args, format);
	vsnprintf(&appendString[0], length + 1, format, args);
	va_end(args);

	// �쐬����������𕶎���z��ɓo�^����
	m_strings.push_back(appendString);
}

//---------------------------------------------------------
// �f�o�b�O�������`�悷��
//---------------------------------------------------------
void mylib::DebugString::Render(DirectX::CommonStates* states)
{
	UNREFERENCED_PARAMETER(states);	// Begin�̃p�����[�^�ݒ�p

	m_spriteBatch->Begin();

	// ������̕`��ʒu
	DirectX::SimpleMath::Vector2 pos{ DirectX::SimpleMath::Vector2::Zero };

	for (unsigned int i = 0; i < m_strings.size(); i++)
	{
		// �\������s�̍������v�Z����
		pos.y = m_fontHeight * i;

		// �`�悷��
		m_spriteFont->DrawString(
			m_spriteBatch.get(),
			m_strings[i].c_str(),
			pos,
			m_color
		);
	}

	m_spriteBatch->End();

	// �`���A������z����N���A����
	m_strings.clear();
}
