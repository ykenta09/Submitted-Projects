/*
	@file	PlayerFactory.h
	@brief	ファクトリークラス
*/
#pragma once
#ifndef MESSENGER_DEFINED
#define MESSENGER_DEFINED

#include "Game/Interface/IComponent.h"
#include "Game/Interface/INonObject.h"
#include "Game/Interface/ISubject.h"
#include <unordered_map>


class Messenger : public ISubject
{
public:
	// 観察者をアタッチする
    static void Attach(const DirectX::Keyboard::Keys& key, IObserver* observer);
    // 観察者をデタッチする
	static void Detach(const DirectX::Keyboard::Keys& key, IObserver* observer);
	// 通知する
	static void Notify(const DirectX::Keyboard::State& keyboardState);
	//観察者リストをクリアする
	static void ClearObserverList();
	// 観察者リストをソートする
	static void SortObserverList();
	//  キー範囲リストを生成する
	static void CreateKeyRangeList();
private:
	// キーボード観察者リスト
    static std::vector<std::pair<DirectX::Keyboard::Keys, IObserver*>> s_observerList;
   // キー範囲リスト(開始インデックスと終了インデックス)
	static std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> s_keysRangeList;

};
#endif // MESSENGER_DEFINED

