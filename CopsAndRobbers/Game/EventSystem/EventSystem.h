/*
	@file	EventSystem.h
	@brief	イベントシステムクラス
*/
#pragma once
#ifndef EVENT_SYSTEM_DEFINED
#define EVENT_SYSTEM_DEFINED

#include "Game/Interface/IComponent.h"
#include "Game/Interface/ISubject.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <type_traits>

class EventSystem
{
public:
   // リスナーを登録する
   //引数が一個の場合テンプレートを使用したイベント登録(今後一つにまとめる)
   template <typename T1>
   static void RegisterListener(const std::function<void(T1)>& listener, T1 value1)
   {
	  listeners.push_back({ listener, value1 });
	  listener(value1);
   }
   //引数が２つ
   template <typename T1, typename T2>
   static void RegisterListener(const std::function<void(T1, T2)>& listener, T1 value1, T2 value2)
   {
	  listeners.push_back({ listener, value1, value2 });
	  listener(value1, value2);
   }
   //引数が３つ
   template <typename T1, typename T2, typename T3>
   static void RegisterListener(const std::function<void(T1, T2, T3)>& listener, T1 value1, T2 value2, T3 value3)
   {
	  listeners.push_back({ listener, value1, value2, value3 });
	  //listener(value);
   }

   // 登録されたリスナーをすべて実行
   //static void TriggerEvents()
   //{
	  //for (const auto& listener : listeners) {
		 //listener();
	  //}
   //}

private:
   //リスナーを格納するリスト
   static std::vector<std::function<void()>> listeners;
};
#endif // EVENT_SYSTEM_DEFINED
