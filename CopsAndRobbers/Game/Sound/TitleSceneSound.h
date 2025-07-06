/*
	@file	TitleSceneSound.h
	@brief	サウンドマネージャークラス
*/
#ifndef TITLE_SOUND_DEFINED
#define TITLE_SOUND_DEFINED
#pragma once

// FMOD用の前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class TitleSceneSound
{
public:
	TitleSceneSound();
	//デストラクタ
	~TitleSceneSound();
	//初期化する
	void Initialize();
	//更新する
	void Update();
	void Finalize();
public:
	//音源を読み込む
	void LoadSound();
	//SEを呼び出す
	void SelectionSE();
	//SEが再生中かどうか確認する
	bool IsSEPlaying();
	//音量設定
	void SetVolumeBGM(float volume);
	void SetVolumeSE(float volume);
	//音量取得
	float GetVolumeBGM() const { return m_volumeBGM; }
	float GetVolumeSE() const { return m_volumeSE; }
    //音量を適応する
	void ApplyVolume();
private:
	// 共通リソース
	CommonResources* m_commonResources;

	//FMOD
	FMOD::System* m_system;	// FMODのシステム
	FMOD::Sound* m_soundSE;	// SE用の音声データ
	FMOD::Sound* m_soundBGM;	// BGM用の音声データ
	FMOD::Channel* m_channelSE;	// SEを再生するチャンネル
	FMOD::Channel* m_channelBGM;// BGMを再生するチャンネル
	float m_volumeBGM;			 //BGMの音量
	float m_volumeSE;			 //SEの音量

};
#endif // TITLE_SOUND_DEFINED
