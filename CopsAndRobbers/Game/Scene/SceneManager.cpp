/*
	@file	SceneManager.cpp
	@brief	�V�[���}�l�[�W���N���X
*/
#include "pch.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "Game/Scene/StageSelectScene.h"
#include "ResultScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Libraries/yamadaLib/Resources.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneManager::SceneManager()
	:
	m_currentScene{},
	m_commonResources{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneManager::~SceneManager()
{
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
void SceneManager::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

#ifdef _DEBUG
	//ChangeScene(IScene::SceneID::PLAY);
	ChangeScene(IScene::SceneID::TITLE);
	//ChangeScene(IScene::SceneID::STAGE_SELECT);
	//ChangeScene(IScene::SceneID::RESULT);
#endif // _DEBUG
#ifdef NDEBUG
	ChangeScene(IScene::SceneID::TITLE);
#endif // NDEBUG
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void SceneManager::Update(float elapsedTime)
{
    //���݂̃V�[�����X�V����
	m_currentScene->Update(elapsedTime);

	// �����p�ϐ��F���̃V�[��
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// �V�[����ύX���Ȃ��Ƃ�
	if (nextSceneID == IScene::SceneID::NONE) return;

	// �V�[����ύX����Ƃ�
	ChangeScene(nextSceneID);
}

/// <summary>
/// �`�悷��
/// </summary>
void SceneManager::Render()
{
    //���݂̃V�[����`�悷��
	m_currentScene->Render();
}


/// <summary>
///  ��n������
/// </summary>
void SceneManager::Finalize()
{
    //�V�[�����폜����
	DeleteScene();
	//���݂̃V�[�������Z�b�g����
	m_currentScene.reset();
}


/// <summary>
/// �V�[����ύX����
/// </summary>
/// <param name="sceneID">�ύX����V�[����ID</param>
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
    //�V�[�����폜����
	DeleteScene();
	//�V�[�����쐬����
	CreateScene(sceneID);
}

/// <summary>
/// �V�[�����쐬����
/// </summary>
/// <param name="sceneID">�쐬����V�[����ID</param>
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_currentScene == nullptr);

	switch (sceneID)
	{
		case IScene::SceneID::TITLE:
			m_currentScene = std::make_unique<TitleScene>();
			break;
		case IScene::SceneID::PLAY:
		   m_currentScene = std::make_unique<PlayScene>();
		   break;
		case IScene::SceneID::STAGE_SELECT:
		   m_currentScene = std::make_unique<StageSelectScene>();
		   break;
		case IScene::SceneID::RESULT:
			m_currentScene = std::make_unique<ResultScene>();
			break;
		default:
			assert(!"SceneManager::CreateScene::�V�[���������݂��܂���I");
			// no break
	}

	assert(m_currentScene && "SceneManager::CreateScene::���̃V�[������������܂���ł����I");
	//�쐬�����V�[���̏�����
	m_currentScene->Initialize(m_commonResources);
}

/// <summary>
/// �V�[�����폜����
/// </summary>
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		m_currentScene.reset();
	}
}
