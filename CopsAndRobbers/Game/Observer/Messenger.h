/*
	@file	PlayerFactory.h
	@brief	�t�@�N�g���[�N���X
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
	// �ώ@�҂��A�^�b�`����
    static void Attach(const DirectX::Keyboard::Keys& key, IObserver* observer);
    // �ώ@�҂��f�^�b�`����
	static void Detach(const DirectX::Keyboard::Keys& key, IObserver* observer);
	// �ʒm����
	static void Notify(const DirectX::Keyboard::State& keyboardState);
	//�ώ@�҃��X�g���N���A����
	static void ClearObserverList();
	// �ώ@�҃��X�g���\�[�g����
	static void SortObserverList();
	//  �L�[�͈̓��X�g�𐶐�����
	static void CreateKeyRangeList();
private:
	// �L�[�{�[�h�ώ@�҃��X�g
    static std::vector<std::pair<DirectX::Keyboard::Keys, IObserver*>> s_observerList;
   // �L�[�͈̓��X�g(�J�n�C���f�b�N�X�ƏI���C���f�b�N�X)
	static std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> s_keysRangeList;

};
#endif // MESSENGER_DEFINED

