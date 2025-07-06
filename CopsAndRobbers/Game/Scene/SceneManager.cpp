/*
	@file	SceneManager.cpp
	@brief	シーンマネージャクラス
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
/// コンストラクタ
/// </summary>
SceneManager::SceneManager()
	:
	m_currentScene{},
	m_commonResources{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneManager::~SceneManager()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
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
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void SceneManager::Update(float elapsedTime)
{
    //現在のシーンを更新する
	m_currentScene->Update(elapsedTime);

	// 説明用変数：次のシーン
	const IScene::SceneID nextSceneID = m_currentScene->GetNextSceneID();

	// シーンを変更しないとき
	if (nextSceneID == IScene::SceneID::NONE) return;

	// シーンを変更するとき
	ChangeScene(nextSceneID);
}

/// <summary>
/// 描画する
/// </summary>
void SceneManager::Render()
{
    //現在のシーンを描画する
	m_currentScene->Render();
}


/// <summary>
///  後始末する
/// </summary>
void SceneManager::Finalize()
{
    //シーンを削除する
	DeleteScene();
	//現在のシーンをリセットする
	m_currentScene.reset();
}


/// <summary>
/// シーンを変更する
/// </summary>
/// <param name="sceneID">変更するシーンのID</param>
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
    //シーンを削除する
	DeleteScene();
	//シーンを作成する
	CreateScene(sceneID);
}

/// <summary>
/// シーンを作成する
/// </summary>
/// <param name="sceneID">作成するシーンのID</param>
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
			assert(!"SceneManager::CreateScene::シーン名が存在しません！");
			// no break
	}

	assert(m_currentScene && "SceneManager::CreateScene::次のシーンが生成されませんでした！");
	//作成したシーンの初期化
	m_currentScene->Initialize(m_commonResources);
}

/// <summary>
/// シーンを削除する
/// </summary>
void SceneManager::DeleteScene()
{
	if (m_currentScene)
	{
		m_currentScene.reset();
	}
}
