/*
	@file	PlaySceneSound.cpp
	@brief	プレイシーンサウンドクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/PlaySceneSound.h"
//FMODのインクルード
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include "PlaySceneSound.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlaySceneSound::PlaySceneSound()
   :
   m_system(nullptr),
   m_soundBGM(),
   m_soundSE(),
   m_channelSE(),
   m_channelBGM(nullptr),
   m_volumeBGM(0.5f),
   m_volumeSE(0.5f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlaySceneSound::~PlaySceneSound()
{
}


/// <summary>
///  初期化する
/// </summary>
void PlaySceneSound::Initialize()
{
	//サウンドを読み込む
	LoadSound();
}


/// <summary>
/// 更新する
/// </summary>
void PlaySceneSound::Update()
{
	FMOD_RESULT result;

	// 二重再生しない
	if (m_channelBGM == nullptr)
	{
	    PlayBGM("Normal");
	}

	// FMODのシステムを更新する
	result = m_system->update();
	assert(result == FMOD_OK);

}


/// <summary>
/// サウンドを読み込む
/// </summary>
void PlaySceneSound::LoadSound()
{
	// システムをインスタンス化する
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// システムを初期化する
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SEをロードする
	LoadBGM("Resources/Sounds/HeartSlowBeat.ogg", "HeartSlow");

	// BGMをロードする
	LoadBGM("Resources/Sounds/PlaySceneBGM.ogg", "Normal");
}


/// <summary>
/// SEを読み込む
/// </summary>
/// <param name="path">SEのファイルパス</param>
/// <param name="name">名前</param>
void PlaySceneSound::LoadSE(const std::string& path, const std::string& name)
{
   FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_soundSE[name]);
   assert(result == FMOD_OK);
}


/// <summary>
/// BGMを読み込む	
/// </summary>
/// <param name="path">BGMのファイルパス</param>
/// <param name="name">名前</param>
void PlaySceneSound::LoadBGM(const std::string& path, const std::string& name)
{
   FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_soundBGM[name]);
   assert(result == FMOD_OK);
}

/// <summary>
/// SEを再生する
/// </summary>
/// <param name="name">再生するSEの名前</param>
void PlaySceneSound::PlaySE(const std::string& name)
{

   FMOD_RESULT result = m_system->playSound(m_soundSE[name], nullptr, false, &m_channelSE[name]);
   assert(result == FMOD_OK);
   //音量設定
   result = m_channelSE[name]->setVolume(m_volumeSE);
   assert(result == FMOD_OK);
}

/// <summary>
/// BGMを再生する
/// </summary>
/// <param name="name">再生するBGMの名前</param>
void PlaySceneSound::PlayBGM(const std::string& name)
{

   if (m_soundBGM.find(name) != m_soundBGM.end())
   {
	  FMOD_RESULT result = m_system->playSound(m_soundBGM[name], nullptr, false, &m_channelBGM);
	  assert(result == FMOD_OK);
	  //音量設定
	  result = m_channelBGM->setVolume(m_volumeBGM);
	  assert(result == FMOD_OK);
   }
}


/// <summary>
/// 後始末する
/// </summary>
void PlaySceneSound::Finalize()
{
	// Sound用のオブジェクトを解放する
   for (const auto& sound : m_soundSE)
   {
	  sound.second->release();
   }
   for (const auto& sound : m_soundBGM)
   {
	  sound.second->release();
   }
	m_system->release();
}


/// <summary>
/// 心音を再生する
/// </summary>
void PlaySceneSound::PlayerHeartSlowBeat()
{
// 別のチャンネルで音を再生する
   FMOD::Channel* channel;
   FMOD_RESULT result = m_system->playSound(m_soundBGM["HeartSlow"], nullptr, false, &channel);
   assert(result == FMOD_OK);

   // 音量設定
   result = channel->setVolume(m_volumeBGM);
   assert(result == FMOD_OK);

}