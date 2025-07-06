/*
	@file	FellowIdle.h
	@brief	仲間待機状態クラス
*/
#pragma once
#ifndef FELLOW_IDLE_DEFINED
#define FELLOW_IDLE_DEFINED

#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;
class Fellow;
class Map;

class FellowIdle : public IState
{
public:
   bool GetFellowMove() { return m_isMove; }
public:
	//コンストラクタ
	FellowIdle(Fellow* fellow, Map* map);
	//デストラクタ
	~FellowIdle();
	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime);
	//描画する
	void Render();
	//後処理する
	void Finalize();
	//動ける状態にあるか
	void MoveFellow();
private:
	//グラフィックス
	yamadalib::Graphics* m_graphics;
	// 共通リソース
	CommonResources* m_commonResources;
	//プレイヤー
	Fellow* m_fellow;
	Map* m_map;	//マップ


	bool m_isMove;	//敵が動くことができるか
};
#endif // FELLOW_IDLE_DEFINED
