/*
	@file	ResultSceneUI.h
	@brief	���U���g�V�[��UI�N���X
*/
#pragma once
#ifndef RESULT_SCENE_UI_DEFINED
#define RESULT_SCENE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Game/Interface/IUIGroup.h"


// �O���錾
class CommonResources;
class Stamina;

class ResultSceneUI : public IUIGroup
{
public:
public:
	//���Ԃ��擾����
	float GetTimer() { return m_timer; }

public:
    ResultSceneUI(CommonResources* resoureces);
	~ResultSceneUI();

	void Initialize() override;
	void Update(const float& elapsedTime, const float& playTime = 0) override;
	void Render() override;
	void Finalize();

	void LoadResources();

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;


	//����
	float m_timer;

};
#endif // RESULT_SCENE_UI_DEFINED
