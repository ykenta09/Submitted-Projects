/*
	@file	IObserver.h
	@brief	オブザーバーのインターフェイスクラス
*/
#pragma once
#ifndef IOBSERVER_DEFINED
#define IOBSERVER_DEFINED

class ISubject;

class IObserver
{
public:
	//キーボードが押されたときに呼び出される
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};
#endif // IOBSERVER_DEFINED
