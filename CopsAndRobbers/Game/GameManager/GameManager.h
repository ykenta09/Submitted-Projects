/*
	@file	GameManager.h
	@brief	ゲームマネージャークラス
*/
#pragma once
#ifndef GAMEMANAGER_DEFINED
#define GAMEMANAGER_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Camera/FixedCamera.h"   
#include "Game/Interface/INonObject.h"

// 前方宣言
class CommonResources;
class AStarGraph;
class Map;

class GameManager : public INonObject
{
public:
    static constexpr float DELAY_TIME = { 5.0f };		 //ゲーム開始時のディレイタイム

	enum class GameState
	{
	   GameStart,
	   GamePlay,
	   GameEnd
	};
public:
   //Graphicsクラスのインスタンスを取得する
   static GameManager* const GetInstance(CommonResources* resources = nullptr, Map* map = nullptr);
public:
	float GetTimer() { return m_timer; }
	float GetDelayTimer() { return m_delayTimer; }
	//ゴールをしたかどうか
	void SetIsGoal(bool flag) { m_isGoal = flag; }
	//ゲームの状態を取得する
	GameState GetGameState() const { return m_gameState; }

	bool GetGameStart() const { return m_gameStart; }
	bool GetGamePlay() const { return m_gamePlay; }
	bool GetGameClear() const { return m_gameClear; }
	bool GetGameOver() const { return m_gameOver; }

	//共通リソースを取得する
	CommonResources* GetCommonResources() { return m_commonResources; }

	//経路探索グラフを取得する
	AStarGraph* GetGraph() { return m_astarGraph.get(); }
	//マップを取得する
	Map* GetMap() { return m_map; }

	//現在のいるステージ番号を取得する
	int GetCurrentStageNumber() const { return m_currentStageNumber; }
	void SetCurrentStageNumber(const int& stageNumber) { m_currentStageNumber = stageNumber; }

public:
	GameManager(CommonResources* resources, Map* map);
	~GameManager();

	void Initialize() override;
	void Update(const float& elapsedTime) override;
	void Render() override;
	void Finalize() override;

	void GoalEvent();

	void OnKeyPressed(const DirectX::Keyboard::Keys& keys) override;
	//インスタンスを削除する
	void DestroyInstance();

	//ゲームスタート関数
	void GamePlay();
	//ゲーム終了関数
	void GameClear();
	void GameOver();
	//ゲームの状態をリセット
	void Reset();
private:
    //GameManagerクラスのインスタンスへのポインタ
    static std::unique_ptr<GameManager> m_gameManager;

    std::unique_ptr<AStarGraph> m_astarGraph;		  //AStarグラフ

	CommonResources* m_commonResources;	 // 共通リソース
	yamadalib::Graphics* m_graphics;	 //グラフィックス
	Map* m_map;

	DirectX::Keyboard::State m_keyboardState;						 //キーボードステート
	DirectX::Keyboard::KeyboardStateTracker m_keyboradStateTracker;	 //キーボードステートトラッカー

	float m_delayTimer;	//開始ディレイ時間

	float m_timer;	//プレイ時間

	bool m_isGoal;		//ゴール下かどうか(削除予定の変数)

	GameState m_gameState; //ゲームの状態
	bool m_gameStart;	   //ゲームスタートフラグ
	bool m_gamePlay;	   //ゲームプレイ中
	bool m_gameClear;	   //ゲームクリアフラグ
	bool m_gameOver;	   //ゲームオーバーフラグ

	int m_currentStageNumber;	 //現在のいるステージの番号

};
#endif // GAMEMANAGER_DEFINED
