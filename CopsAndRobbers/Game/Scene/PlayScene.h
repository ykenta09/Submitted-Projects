/*
	@file	PlayScene.h
	@brief	プレイシーンクラス
*/
#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED

#include "Game/Interface/IScene.h"
#include "Game/Interface/IComponent.h"
#include "Game/Interface/IComposite.h"
#include "Game/Interface/INonObject.h"
#include "Game/Map/Goal/Goal.h"

#include "StepTimer.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/yamadaLib/LoadJson.h"
#include "Game/Particle/Fade.h"
#include "Game/Particle/CircularFade.h"
#include "Game/Particle/Partcle.h"
#include "Game/FrustumCuller.h"

#include "Game/Particle/Fragment.h"

// 前方宣言
class CommonResources;
class SoundManager;
class Map;
class Enemy;
class GameManager;
class CameraManager;
class UIManager;
class Fellow;
class GraphScene;

namespace mylib
{
	class GridFloor;
	class TPSCamera;
}

class PlayScene final : public IScene
{
public:
   static constexpr float CAMERA_ROTATION_ANGLE = { DirectX::XMConvertToRadians(40.0f) };				//カメラの回転量

public:
    //コンストラクタ
	PlayScene();
	//デストラクタ
	~PlayScene() override;

	//初期化する
	void Initialize(CommonResources* resources) override;
	//更新する
	void Update(float elapsedTime)override;
	//描画する
	void Render() override;
	//後始末をする
	void Finalize() override;

	//次のシーンIDを取得する
	SceneID GetNextSceneID() const;

	//ステージをロードする
	void LoadStage(int stageNumber);

	//ゴール条件を満たしたか
	bool IsGoal();
	//スタート処理
	void StartEvent(float elapsedTime, Player* player);
	//終了処理
	void EndProcessing(float elapsedTime);
private:
	CommonResources* m_commonResources;	  	// 共通リソース
	yamadalib::Graphics* m_graphics;	    //グラフィックス
	CollisionManager* m_collision;		    //衝突判定
	GameManager* m_gameManager;		  //ゲームマネージャー

	std::unique_ptr<SoundManager> m_soundManager;	  //サウンド
	std::unique_ptr<CameraManager> m_cameraManager;	  //カメラマネージャー
	std::unique_ptr<UIManager> m_uiManager;			  //UIマネージャー
	std::unique_ptr<LoadJson> m_loadJson;			  //jsonファイル

	DirectX::Keyboard::State m_keyboardState;						 //キーボードステート
	DirectX::Keyboard::KeyboardStateTracker m_keyboradStateTracker;	 //キーボードステートトラッカー
	DirectX::Mouse::State m_mouseState;								 //マウスのステート

	DirectX::SimpleMath::Matrix m_view;			// ビュー行列
	DirectX::SimpleMath::Matrix m_projection;	// 射影行列

	std::unique_ptr<IComposite> m_root;	//シーングラフルート
	std::vector<std::unique_ptr<INonObject>> m_nonObjects; 	   //非オブジェクトのインスタンス配列
	std::unique_ptr<Map> m_map;	//マップ
	std::unique_ptr<Enemy> m_enemy;	//敵
	std::unique_ptr<Fellow> m_fellow;	//仲間
	std::unique_ptr<GraphScene> m_graphScene;	//グラフシーン
	std::unique_ptr<Fade> m_fade;				//パーティクル
	std::unique_ptr<Fragment> m_fragment;		//警告エフェクト

	bool m_isChangeScene;			// シーンチェンジフラグ
	float m_cameraAngle;			//スタートイベント用
	float m_endEventCameraAngle;
	float m_timer;					//フレームをカウントする
	int m_enemyMaxIndex;			//ステージの敵の総数
	std::vector<Enemy*> m_enemies;	//敵の配列	 
};
#endif // PLAY_SCENE_DEFINED
