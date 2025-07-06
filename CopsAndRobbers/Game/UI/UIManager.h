/*
	@file	UIManager.h
	@brief	UIマネージャークラス
*/
#pragma once
#ifndef UI_MANAGER_DEFINED
#define UI_MANAGER_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include "Game/Interface/IUIGroup.h"
#include "Game/UI/TitleScene/TitleSceneUI.h"
#include "Game/UI/PlayScene/PlaySceneUI.h"
#include "Game/UI/ResultScene/ResultSceneUI.h"

// 前方宣言
class CommonResources;
class Player;
class Fellow;
class Map;

class UIManager
{
public:
   enum SceneType
   {
	  TITLE,	  //タイトル
	  PLAY,		  //プレイ
	  RESULT	  //リザルト
   };
public:
   IUIGroup* GetCurrentUIGroup() { return m_currentUIGroup; }


   TitleSceneUI* GetTitleSceneUI() const { return m_titleScene.get(); }
   PlaySceneUI* GetPlaySceneUI() const { return m_playScene.get(); }
   ResultSceneUI* GetResultSceneUI() const { return m_resultScene.get(); }
public:
   //コンストラクタ
   UIManager(CommonResources* resoureces);
   UIManager(CommonResources* resoureces, Player* player, Fellow* fellow, Map* map);
   //デストラクタ
   ~UIManager();

   //初期化する
   void Initialize(SceneType scene);
   //更新する
   void Update(const float& elapsedTime, const float& playTime = 0);
   //描画する
   void Render();
   //後始末する
   void Finalize();

   //ステージを切り替える
   void ChangeScene(SceneType scene);

private:
   // 共通リソース
   CommonResources* m_commonResources;
   //グラフィックス
   yamadalib::Graphics* m_graphics;

   //シーン
   std::unique_ptr<TitleSceneUI> m_titleScene;
   std::unique_ptr<PlaySceneUI> m_playScene;
   std::unique_ptr<ResultSceneUI> m_resultScene;

   IUIGroup* m_currentUIGroup;
   Player* m_player;
   Fellow* m_fellow;
   Map* m_map;
};
#endif // UI_MANAGER_DEFINED
