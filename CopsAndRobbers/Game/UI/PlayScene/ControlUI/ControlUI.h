/*
	@file	ControlUI.h
	@brief	�R���g���[��UI�N���X
*/
#pragma once
#ifndef CONTROL_UI_DEFINED
#define CONTROL_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;

class ControlUI
{
public:
   static constexpr DirectX::SimpleMath::Vector2 CONTROLS_POSITION = DirectX::SimpleMath::Vector2(50, 650);						 //������@�̃e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 SPRINT_RENDER_SPACE = DirectX::SimpleMath::Vector2(350.0f, 0.0f);				 //�_�b�V���̕`��Ԋu
   static constexpr DirectX::SimpleMath::Vector2 MOUSE_RENDER_SPACE = DirectX::SimpleMath::Vector2(800.0f, 0.0f);				 //�}�E�X�̕`��Ԋu
   static constexpr RECT MOVE_RECT = { 0, 0 , 634, 80 };			 //����̃e�N�X�`���؂���
   static constexpr RECT SPRINT_RECT = { 0, 65 , 624, 200 };		 //�_�b�V���e�N�X�`���؂���
   static constexpr RECT MOUSE_RECT = { 0, 125 , 624, 200 };		 //�}�E�X����̃e�N�X�`���؂���
public:
    //�R���X�g���N�^
    ControlUI();
	//�f�X�g���N�^
	~ControlUI();

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

	//UI�w�i�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	//������@�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_controlsTextTexture;

	//������@�̕`����W
	DirectX::SimpleMath::Vector2 m_controlsPosition;
};
#endif // CONTROL_UI_DEFINED
