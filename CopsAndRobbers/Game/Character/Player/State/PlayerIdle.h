/*
	@file	PlayerIdle.h
	@brief	プレイヤー待機状態クラス
*/
#pragma once
#ifndef PLAYER_IDLE_DEFINED
#define PLAYER_IDLE_DEFINED

#include "Game/Interface/IState.h"

// 前方宣言
class Player;

class PlayerIdle : public IState
{
public:
	//コンストラクタ
	PlayerIdle(Player* player);
	//デストラクタ
	~PlayerIdle();
	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render() ;
	//後処理する
	void Finalize();
private:
	//プレイヤー
	Player* m_player;
};
#endif // PLAYER_IDLE_DEFINED
