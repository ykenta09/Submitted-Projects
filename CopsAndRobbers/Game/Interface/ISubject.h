/*
	@file	ISubject.h
	@brief	オブザーバーのインターフェイスクラス
*/
#pragma once
#ifndef ISUBJECT_DEFINED
#define ISUBJECT_DEFINED

#include "Game/Interface/IComponent.h"

// 前方宣言
class IComponent;

class ISubject
{
public:
	// デストラクタ
	virtual ~ISubject() = default;
	// 観察者をアタッチする
	virtual void Attach(const DirectX::Keyboard::Keys& key, IComponent* observer) = 0;
	// 観察者をデタッチする
	virtual void Detach(const DirectX::Keyboard::Keys& key, IComponent* observer) = 0;
	// 通知する
	virtual void Notify(const DirectX::Keyboard::State& keyboardState,
		const std::vector<int>& playerNodeNumbers) = 0;
};
#endif // ISUBJECT_DEFINED

