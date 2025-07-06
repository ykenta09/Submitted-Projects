#include "pch.h"
#include "Game/Observer/Messenger.h"
#include "Game/Interface/IObserver.h"

//キーボード観察者リストを初期化する
std::vector<std::pair<DirectX::Keyboard::Keys, IObserver*>> Messenger::s_observerList;
// キー範囲リスト(キー、開始インデックスと終了インデックス)
std::unordered_map<DirectX::Keyboard::Keys, std::vector<std::pair<int, int>>> Messenger::s_keysRangeList;


/// <summary>
/// 観察者をアタッチする
/// </summary>
/// <param name="key">登録キー</param>
/// <param name="observer">観測者</param>
void Messenger::Attach(const DirectX::Keyboard::Keys& key, IObserver* observer)
{
	//観察者をアタッチする
	s_observerList.emplace_back(std::make_pair(key, observer));
}

/// <summary>
/// 観察者をデタッチする
/// </summary>
/// <param name="key">登録解除するキー</param>
/// <param name="observer">観測者</param>
void Messenger::Detach(const DirectX::Keyboard::Keys& key, IObserver* observer)
{
	//観察者リストから観察者を検索する
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
/// 観察者に通知する
/// </summary>
/// <param name="keyboardState">キーボードの状態</param>
void Messenger::Notify(const DirectX::Keyboard::State& keyboardState)
{
	//観察者リストから観察者を取り出す
	for (const auto& observer : s_observerList)
	{
		//観察者が処理するキーか確認
		if (keyboardState.IsKeyDown(observer.first))
		{
			// 観察者のOnNotify通知関数を呼び出す
			observer.second->OnKeyPressed(observer.first);
		}
	}
}

/// <summary>
/// 観察者リストをリセットする
/// </summary>
void Messenger::ClearObserverList()
{
	if (s_observerList.size()) s_observerList.clear();
}


/// <summary>
///  観察者リストをソートする
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
/// キー範囲リストを生成する
/// </summary>
void Messenger::CreateKeyRangeList()
{
	// キー範囲をクリアする
	if (s_keysRangeList.size()) s_keysRangeList.clear();
	// 開始インデックスを設定する
	int startIndex = 0;
	// 観察者リストの開始キーを現在のキーにする
	DirectX::Keyboard::Keys currentKey = s_observerList[startIndex].first;
	for (int index = 1; index < s_observerList.size(); index++)
	{
		// 観察者リストの現在キーと現在のキーが等しくない場合
		if (s_observerList[index].first != currentKey)
		{
			// キー、開始インデックスと終了インデックスをキー範囲リストに追加する
			s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, static_cast<int>(index - 1)));
			// 現在のインデックスを開始インデックスに設定する
			startIndex = index;
			// 現在の観察者のキーを現在のキーに設定する
			currentKey = s_observerList[index].first;
		}
	}
	// キー、開始インデックスと最後のインデックスをキー範囲リストに追加する
	s_keysRangeList[currentKey].push_back(std::make_pair(startIndex, static_cast<int>(s_observerList.size() - 1)));
}
