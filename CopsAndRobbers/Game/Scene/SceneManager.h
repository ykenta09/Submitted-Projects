/*
	@file	SceneManager.h
	@brief	�V�[���}�l�[�W���N���X
*/
#pragma once
#ifndef SCENE_MANAGER_DEFINED
#define SCENE_MANAGER_DEFINED

#include "Game/Interface/IScene.h"

// �O���錾
class CommonResources;

class SceneManager
{

public:
    //�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();
	//����������
	void Initialize(CommonResources* resources);
	//�X�V����
	void Update(float elapsedTime);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();


private:
    //�V�[����ύX����
	void ChangeScene(IScene::SceneID sceneID);
	//�V�[�����쐬����
	void CreateScene(IScene::SceneID sceneID);
	//�V�[�����폜����
	void DeleteScene();
private:
   CommonResources* m_commonResources;		 //���ʃ��\�[�X
	std::unique_ptr<IScene> m_currentScene;	//�V�[���̃|�C���^
};
#endif // SCENE_MANAGER_DEFINED
