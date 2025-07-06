/*
	@file	SceneManager.h
	@brief	シーンマネージャクラス
*/
#pragma once
#ifndef SCENE_MANAGER_DEFINED
#define SCENE_MANAGER_DEFINED

#include "Game/Interface/IScene.h"

// 前方宣言
class CommonResources;

class SceneManager
{

public:
    //コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();
	//初期化する
	void Initialize(CommonResources* resources);
	//更新する
	void Update(float elapsedTime);
	//描画する
	void Render();
	//後始末する
	void Finalize();


private:
    //シーンを変更する
	void ChangeScene(IScene::SceneID sceneID);
	//シーンを作成する
	void CreateScene(IScene::SceneID sceneID);
	//シーンを削除する
	void DeleteScene();
private:
   CommonResources* m_commonResources;		 //共通リソース
	std::unique_ptr<IScene> m_currentScene;	//シーンのポインタ
};
#endif // SCENE_MANAGER_DEFINED
