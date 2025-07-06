/*
	@file	EventSystem.h
	@brief	�C�x���g�V�X�e���N���X
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
   // ���X�i�[��o�^����
   //��������̏ꍇ�e���v���[�g���g�p�����C�x���g�o�^(�����ɂ܂Ƃ߂�)
   template <typename T1>
   static void RegisterListener(const std::function<void(T1)>& listener, T1 value1)
   {
	  listeners.push_back({ listener, value1 });
	  listener(value1);
   }
   //�������Q��
   template <typename T1, typename T2>
   static void RegisterListener(const std::function<void(T1, T2)>& listener, T1 value1, T2 value2)
   {
	  listeners.push_back({ listener, value1, value2 });
	  listener(value1, value2);
   }
   //�������R��
   template <typename T1, typename T2, typename T3>
   static void RegisterListener(const std::function<void(T1, T2, T3)>& listener, T1 value1, T2 value2, T3 value3)
   {
	  listeners.push_back({ listener, value1, value2, value3 });
	  //listener(value);
   }

   // �o�^���ꂽ���X�i�[�����ׂĎ��s
   //static void TriggerEvents()
   //{
	  //for (const auto& listener : listeners) {
		 //listener();
	  //}
   //}

private:
   //���X�i�[���i�[���郊�X�g
   static std::vector<std::function<void()>> listeners;
};
#endif // EVENT_SYSTEM_DEFINED
