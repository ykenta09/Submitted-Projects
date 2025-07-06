/*
	@file	Shadow.h
	@brief	�e�N���X
*/
#pragma once
#ifndef SHADOW_DEFINED
#define SHADOW_DEFINED

#include "Libraries/yamadaLib/Graphics.h"

//�e�N���X
class Shadow
{	
public:
	//�R���X�g���N�^
	Shadow();
	//�f�X�g���N�^
	~Shadow();

	//����������
	void Initialize();
	//�`�悷��
	void Render(
	const DirectX::SimpleMath::Vector3& position,
	float radius = 0.9f);
	//�㏈��������
	void Finalize();
private:
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
};
#endif // SHADOW_DEFINED

