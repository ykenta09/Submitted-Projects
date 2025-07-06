/*
	@file		BlackBoard.h
	@brief		黒板クラス：データの受け渡し用クラス
*/
#pragma once

namespace mylib
{
	class BlackBoard
	{
	public:
	   // 時間の加算
	   void AddTime(float time) { m_time = time; }
	   // 時間の取得
	   float GetTime() const { return m_time; }
	   //ステージ番号を設定する
	   void SetStageNumber(int number) { m_stageNumber = number; }
	   //ステージ番号の取得
	   int GetStageNumber() const { return m_stageNumber; }
	   //所持している鍵の数を増やす
	   void SetHasKeyNum(int num) { m_hasKeyNum = num; }
	   //所持している鍵の数を取得する
	   int GetHasKeyNum() const { return m_hasKeyNum; }
	public:
		BlackBoard() 
		   :
		   m_time{},
		   m_stageNumber{ 0 },
		   m_hasKeyNum{ 0 }
		{}
		~BlackBoard() = default;

	private:
	   float m_time;		  // 時間
	   int m_stageNumber;	  //ステージ番号
	   int m_hasKeyNum;		  //所持している鍵の数
	};
}