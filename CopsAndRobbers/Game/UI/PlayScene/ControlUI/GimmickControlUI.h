/*
	@file	GimmickControlUI.h
	@brief	�R���g���[��UI�N���X
*/
#pragma once
#ifndef GIMMICK_CONTROL_UI_DEFINED
#define GIMMICK_CONTROL_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;

class GimmickControlUI
{
public:
   static constexpr DirectX::SimpleMath::Vector3 CONTROLS_OFFSET = DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f);						 //������@�̃e�N�X�`�����W
public:
    //�R���X�g���N�^
    GimmickControlUI();
	//�f�X�g���N�^
	~GimmickControlUI();

	//����������
	void Initialize();
	//�X�V����
	void Update();
	//�`�悷��
	void Render();
	//��n������
	void Finalize();

private:
	//������@��UI
	void ControlsUI();
	//���\�[�X��ǂݍ���
	void LoadResources();

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
	//������@�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_controlTexture;

	//������@�̕`����W
	DirectX::SimpleMath::Vector3 m_controlsPosition;

	//UI�\���t���O
	bool m_isVisible;
};
#endif // GIMMICK_CONTROL_UI_DEFINED
