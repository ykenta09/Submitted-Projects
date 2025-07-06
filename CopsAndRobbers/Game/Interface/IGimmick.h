/*
	@file	IGimmick.h
	@brief	ギミックのインターフェイスクラス
*/
#pragma once
#include <SimpleMath.h>
#ifndef IGIMMICK_DEFINED
#define IGIMMICK_DEFINED

class IGimmick
{
public:
   //アクティブにする
   virtual void Active() = 0;
   //非アクティブにする
   virtual void Deactive() = 0;
   //処理が終了したか
   virtual bool IsFinished() = 0;
   //処理終了フラグをリセットする
   virtual void Reset() = 0;
};
#endif // IGIMMICK_DEFINED
