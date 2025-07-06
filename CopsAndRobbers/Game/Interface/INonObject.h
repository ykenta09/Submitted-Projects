/*
	@file	INonObject.h
	@brief	非オブジェクトインターフェイスクラス
*/
#ifndef INON_OBJECT_DEFINED
#define INON_OBJECT_DEFINED
#pragma once
#include "pch.h"
#include "Game/Interface/IObserver.h"

class INonObject : public IObserver
{
public:
   virtual ~INonObject() = default;
   virtual void Initialize() = 0;						 //初期化処理
   virtual void Update(const float& elapsedTime) = 0;	 //更新処理
   virtual void Render() = 0;							 //描画
   virtual void Finalize() = 0;						    //後処理

   //キーボードが押されたときに呼び出される
   virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};
#endif // INON_OBJECT_DEFINED
