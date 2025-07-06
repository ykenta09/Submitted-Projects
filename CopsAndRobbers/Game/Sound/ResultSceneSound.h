/*
	@file	ResultSceneSound.h
	@brief	リザルトサウンドクラス
*/
#ifndef RESULT_SOUND_DEFINED
#define RESULT_SOUND_DEFINED
#pragma once

// FMOD用の前方宣言
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class ResultSceneSound
{
public:
	ResultSceneSound();
	//デストラクタ
	~ResultSceneSound();
	//初期化する
	void Initialize();
	//更新する
	void Update();
	void Finalize();
public:
	void LoadSound();	
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
#endif // RESULT_SOUND_DEFINED
