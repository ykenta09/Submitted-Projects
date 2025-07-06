/*
	@file	SoundManager.h
	@brief	サウンドマネージャークラス
*/
#ifndef SOUND_MANAGER_DEFINED
#define SOUND_MANAGER_DEFINED
#pragma once

// FMOD用の前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

// 前方宣言
class TitleSceneSound;
class PlaySceneSound;
class ResultSceneSound;;

class SoundManager
{
public:
	enum SoundSceneID : int
	{
		TITLE,
		PLAY,
		RESULT
	};
public:
	//タイトルのサウンドを取得する
	TitleSceneSound* GetTitle() { return m_titleSound.get(); }
	PlaySceneSound* GetPlay() { return m_playSound.get(); }
	ResultSceneSound* GetResult() { return m_resultSound.get(); }
public:
	SoundManager();
	//デストラクタ
	~SoundManager();
	//初期化する
	void Initialize(SoundSceneID sceneid);
	//更新する
	void Update();
	void Finalize();

	void NotifyEnemyApproach();

private:
	//SceneSound
	SoundSceneID m_soundId;

	//シーンのサウンド
	std::unique_ptr<PlaySceneSound> m_playSound;
	std::unique_ptr<TitleSceneSound> m_titleSound;
	std::unique_ptr<ResultSceneSound> m_resultSound;

};
#endif // SOUND_MANAGER_DEFINED
