/*
	@file	TitleSound.cpp
	@brief	タイトルシーンサウンドクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/TitleSceneSound.h"
//FMODのインクルード
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include "PlaySceneSound.h"


/// <summary>
/// コンストラクタ
/// </summary>
TitleSceneSound::TitleSceneSound()
	: 
	m_system(nullptr),
	m_soundSE(nullptr), 
	m_soundBGM(nullptr), 
	m_channelSE(nullptr), 
	m_channelBGM(nullptr),
    m_volumeBGM(0.3f),
    m_volumeSE(0.3f)
{
}


/// <summary>
/// デストラクタ
/// </summary>
TitleSceneSound::~TitleSceneSound()
{
}


/// <summary>
/// 初期化する
/// </summary>
void TitleSceneSound::Initialize()
{
	//サウンドを読み込む
	LoadSound();
}

/// <summary>
/// 更新する
/// </summary>
void TitleSceneSound::Update()
{
	FMOD_RESULT result;

	// 二重再生しない
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
		ApplyVolume();
	}

	// FMODのシステムを更新する
	result = m_system->update();
	assert(result == FMOD_OK);

}


/// <summary>
/// サウンドを読み込む
/// </summary>
void TitleSceneSound::LoadSound()
{
	// システムをインスタンス化する
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// システムを初期化する
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SEをロードする
	result = m_system->createSound("Resources/Sounds/Button.ogg", FMOD_DEFAULT, nullptr, &m_soundSE);
	assert(result == FMOD_OK);

	// BGMをロードする
	result = m_system->createSound("Resources/Sounds/TitleBGM.ogg", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);

}


/// <summary>
/// 選択音
/// </summary>
void TitleSceneSound::SelectionSE()
{
	FMOD_RESULT result;

	result = m_system->playSound(m_soundSE, nullptr, false, &m_channelSE);
	assert(result == FMOD_OK);
	ApplyVolume();
}

/// <summary>
/// SEが再生中かどうか
/// </summary>
/// <returns></returns>
bool TitleSceneSound::IsSEPlaying()
{
	bool isPlaying = false;
	if (m_channelSE != nullptr)
	{
		m_channelSE->isPlaying(&isPlaying);
	}
	return isPlaying;
}

/// <summary>
/// BGM音量設定
/// </summary>
/// <param name="volume">設定する音量</param>
void TitleSceneSound::SetVolumeBGM(float volume)
{
   m_volumeBGM = volume;
   //音量を適用する
   ApplyVolume();
}

/// <summary>
/// SE音量を設定する
/// </summary>
/// <param name="volume">設定する音量</param>
void TitleSceneSound::SetVolumeSE(float volume)
{
   m_volumeSE = volume;
   //音量を適用する
   ApplyVolume();
}


/// <summary>
/// 音量を適用する
/// </summary>
void TitleSceneSound::ApplyVolume()
{
   //BGMとSEそれぞれの音量を適用する
   if (m_channelBGM)
   {
	  FMOD_RESULT result = m_channelBGM->setVolume(m_volumeBGM);
	  assert(result == FMOD_OK);
   }
   if (m_channelSE)
   {
	  FMOD_RESULT result = m_channelSE->setVolume(m_volumeSE);
	  assert(result == FMOD_OK);
   }
}

/// <summary>
/// 後始末する
/// </summary>
void TitleSceneSound::Finalize()
{
	// Sound用のオブジェクトを解放する
	if(m_soundSE) m_soundSE->release();
	if (m_soundBGM) m_soundBGM->release();
	if (m_system) m_system->release();
}

