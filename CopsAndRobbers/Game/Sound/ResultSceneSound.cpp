/*
	@file	ResultSound.cpp
	@brief	リザルトサウンドシーンクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/ResultSceneSound.h"
//FMODのインクルード
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include "PlaySceneSound.h"


/// <summary>
/// コンストラクタ
/// </summary>
ResultSceneSound::ResultSceneSound()
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
ResultSceneSound::~ResultSceneSound()
{
}


/// <summary>
///  初期化する
/// </summary>
void ResultSceneSound::Initialize()
{
	//サウンドを読み込む
	LoadSound();
}


/// <summary>
/// 更新する
/// </summary>
void ResultSceneSound::Update()
{
	FMOD_RESULT result;

	// 二重再生しない
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
	}

	// FMODのシステムを更新する
	result = m_system->update();
	assert(result == FMOD_OK);

}


/// <summary>
/// サウンドを読み込む
/// </summary>
void ResultSceneSound::LoadSound()
{
	// システムをインスタンス化する
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// システムを初期化する
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SEをロードする
	result = m_system->createSound("Resources/Sounds/Blast.ogg", FMOD_DEFAULT, nullptr, &m_soundSE);
	assert(result == FMOD_OK);

	// BGMをロードする
	result = m_system->createSound("Resources/Sounds/PlaySceneBGM.ogg", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);

}


/// <summary>
/// BGMの音量を設定する
/// </summary>
/// <param name="volume">設定する音量</param>
void ResultSceneSound::SetVolumeBGM(float volume)
{
   m_volumeBGM = volume;
   //音量を適用する
   ApplyVolume();
}


/// <summary>
/// SEの音量を設定する
/// </summary>
/// <param name="volume">設定する音量</param>
void ResultSceneSound::SetVolumeSE(float volume)
{
   m_volumeSE = volume;
   //音量を適用する
   ApplyVolume();
}

/// <summary>
/// 音量を適用する
/// </summary>
void ResultSceneSound::ApplyVolume()
{
   //BGMとSEそれぞれ音量を適用する
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
void ResultSceneSound::Finalize()
{
	// Sound用のオブジェクトを解放する
	m_soundSE->release();
	m_soundBGM->release();
	m_system->release();
}

