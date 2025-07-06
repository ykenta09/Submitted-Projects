/*
	@file	PlaySceneSound.h
	@brief	サウンドマネージャークラス
*/
#ifndef PLAY_SOUND_DEFINED
#define PLAY_SOUND_DEFINED
#pragma once
#include <unordered_map>
#include <string>

// FMOD用の前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class PlaySceneSound
{
public:
	PlaySceneSound();
	//デストラクタ
	~PlaySceneSound();
	//初期化する
	void Initialize();
	//更新する
	void Update();
	void Finalize();
	void PlayerHeartSlowBeat();
public:
	void LoadSound();		//サウンドをロードする
	void SetVolumeBGM(float volume);
	void SetVolumeSE(float volume);
	void LoadSE(const std::string& path, const std::string& name);
	void LoadBGM(const std::string& path, const std::string& name);
	void PlaySE(const std::string& name);
	void PlayBGM(const std::string& name);
	float GetVolumeBGM() const { return m_volumeBGM; }
	float GetVolumeSE() const { return m_volumeSE; }
private:
	// 共通リソース
	CommonResources* m_commonResources;

	//FMOD
	FMOD::System* m_system;	// FMODのシステム
	std::unordered_map<std::string, FMOD::Sound*> m_soundSE;
	std::unordered_map<std::string, FMOD::Sound*> m_soundBGM;
	//FMOD::Sound* m_soundSE;	// SE用の音声データ
	//FMOD::Sound* m_soundBGM;	// BGM用の音声データ
	std::unordered_map<std::string, FMOD::Channel*> m_channelSE;
	FMOD::Channel* m_channelBGM;
	//FMOD::Channel* m_channelSE;	// SEを再生するチャンネル
	//FMOD::Channel* m_channelBGM;// BGMを再生するチャンネル
	float m_volumeBGM;
	float m_volumeSE;

};
#endif // PLAY_SOUND_DEFINED
