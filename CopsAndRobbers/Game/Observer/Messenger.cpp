#include "pch.h"
#include "Game/Observer/Messenger.h"
#include "Game/Interface/IObserver.h"

//�L�[�{�[�h�ώ@�҃��X�g������������
std::vector<std::pair<DirectX::Keyboard::Keys, IObserver*>> Messenger::s_observerList;
// �L�[�͈̓��X�g(�L�[�A�J�n�C���f�b�N�X�ƏI���C���f�b�N�X)
std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> Messenger::s_keysRangeList;


/// <summary>
/// �ώ@�҂��A�^�b�`����
/// </summary>
/// <param name="key">�o�^�L�[</param>
/// <param name="observer">�ϑ���</param>
void Messenger::Attach(const DirectX::Keyboard::Keys& key, IObserver* observer)
{
	//�ώ@�҂��A�^�b�`����
	s_observerList.emplace_back(std::make_pair(key, observer));
}

/// <summary>
/// �ώ@�҂��f�^�b�`����
/// </summary>
/// <param name="key">�o�^��������L�[</param>
/// <param name="observer">�ϑ���</param>
void Messenger::Detach(const DirectX::Keyboard::Keys& key, IObserver* observer)
{
	//�ώ@�҃��X�g����ώ@�҂���������
	s_observerList.erase(
		std::remove_if(s_observerList.begin(), s_observerList.end(),
			[&key, observer](const std::pair<DirectX::Keyboard::Keys, IObserver*>& entry)
			{
				return entry.first == key && entry.second == observer;
			}),
		s_observerList.end()
	);
}

/// <summary>
/// �ώ@�҂ɒʒm����
/// </summary>
/// <param name="keyboardState">�L�[�{�[�h�̏��</param>
void Messenger::Notify(const DirectX::Keyboard::State& keyboardState)
{
	//�ώ@�҃��X�g����ώ@�҂����o��
	for (const auto& observer : s_observerList)
	{
		//�ώ@�҂���������L�[���m�F
		if (keyboardState.IsKeyDown(observer.first))
		{
			// �ώ@�҂�OnNotify�ʒm�֐����Ăяo��
			observer.second->OnKeyPressed(observer.first);
		}
	}
}

/// <summary>
/// �ώ@�҃��X�g�����Z�b�g����
/// </summary>
void Messenger::ClearObserverList()
{
	if (s_observerList.size()) s_observerList.clear();
}


/// <summary>
///  �ώ@�҃��X�g���\�[�g����
/// </summary>
void Messenger::SortObserverList()
{
	
	std::sort(
		s_observerList.begin(),
		s_observerList.end(),
		[](const std::pair<DirectX::Keyboard::Keys, IObserver*>& a,
			const std::pair<DirectX::Keyboard::Keys, IObserver*>& b)
		{
			return a.first < b.first;
		});
}

/// <summary>
/// �L�[�͈̓��X�g�𐶐�����
/// </summary>
void Messenger::CreateKeyRangeList()
{
	// �L�[�͈͂��N���A����
	if (s_keysRangeList.size()) s_keysRangeList.clear();
	// �J�n�C���f�b�N�X��ݒ肷��
	int startIndex = 0;
	// �ώ@�҃��X�g�̊J�n�L�[�����݂̃L�[�ɂ���
	DirectX::Keyboard::Keys currentKey = s_observerList[startIndex].first;
	for (int index = 1; index < s_observerList.size(); index++)
	{
		// �ώ@�҃��X�g�̌��݃L�[�ƌ��݂̃L�[���������Ȃ��ꍇ
		if (s_observerList[index].first != currentKey)
		{
			// �L�[�A�J�n�C���f�b�N�X�ƏI���C���f�b�N�X���L�[�͈̓��X�g�ɒǉ�����
			s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, static_cast<int>(index - 1)));
			// ���݂̃C���f�b�N�X���J�n�C���f�b�N�X�ɐݒ肷��
			startIndex = index;
			// ���݂̊ώ@�҂̃L�[�����݂̃L�[�ɐݒ肷��
			currentKey = s_observerList[index].first;
		}
	}
	// �L�[�A�J�n�C���f�b�N�X�ƍŌ�̃C���f�b�N�X���L�[�͈̓��X�g�ɒǉ�����
	s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, static_cast<int>(s_observerList.size() - 1)));
}
