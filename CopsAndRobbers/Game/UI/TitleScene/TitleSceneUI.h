/*
	@file	TitleSceneUI.h
	@brief	�^�C�g���V�[��UI�N���X
*/
#pragma once
#ifndef TITLE_SCENE_UI_DEFINED
#define TITLE_SCENE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Game/Interface/IUIGroup.h"


// �O���錾
class CommonResources;
class Stamina;

class TitleSceneUI : public IUIGroup
{
public:
public:
	//���Ԃ��擾����
	float GetTimer() { return m_timer; }

public:
    //�R���X�g���N�^
    TitleSceneUI(CommonResources* resoureces);
	//�f�X�g���N�^
	~TitleSceneUI();

	//����������
	void Initialize() override;
	//�X�V����
	void Update(const float& elapsedTime, const float& playTime = 0) override;
	//�`�悷��
	void Render() override;
	//��n������
	void Finalize();
	//���\�[�X��ǂݍ���
	void LoadResources();

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;

	//����
	float m_timer;
	//���Ԃ̕`����W
	DirectX::SimpleMath::Vector2 m_timerPosition;
	//�X�^�~�i�Q�[�W�`����W
	DirectX::SimpleMath::Vector2 m_staminaPosition;
	//������@�̕`����W
	DirectX::SimpleMath::Vector2 m_controlsPosition;
};
#endif // TITLE_SCENE_UI_DEFINED
