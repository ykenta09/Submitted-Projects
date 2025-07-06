/*
	@file	SoundManager.cpp
	@brief	サウンドマネージャークラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/SoundManager.h"
//各シーンのサウンド
#include "Game/Sound/TitleSceneSound.h"
#include "Game/Sound/PlaySceneSound.h"
#include "Game/Sound/ResultSceneSound.h"
//FMODのインクルード
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"


/// <summary>
/// コンストラクタ
/// </summary>
SoundManager::SoundManager()
	:
	m_soundId{}
{
}


/// <summary>
/// デストラクタ
/// </summary>
SoundManager::~SoundManager()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
/// <param name="sceneid">シーンのID</param>
void SoundManager::Initialize(SoundSceneID sceneid)
{
    //現在のシーンを設定する
	m_soundId = sceneid;
	//現在のシーンの処理する
	switch (m_soundId)
	{
	case SoundManager::TITLE:
		m_titleSound = std::make_unique<TitleSceneSound>();
		m_titleSound->LoadSound();
		break;
	case SoundManager::PLAY:
		m_playSound = std::make_unique<PlaySceneSound>();
		m_playSound->LoadSound();
		break;
	case SoundManager::RESULT:
		m_resultSound = std::make_unique<ResultSceneSound>();
		m_resultSound->LoadSound();
		break;
	default:
		break;
	}
}


/// <summary>
/// 更新する
/// </summary>
void SoundManager::Update()
{
   //現在のシーンの処理する
	switch (m_soundId)
	{
	case SoundManager::TITLE:
		m_titleSound->Update();
		break;
	case SoundManager::PLAY:
		m_playSound->Update();
		break;
	case SoundManager::RESULT:
		m_resultSound->Update();
		break;
	default:
		break;
	}
}

/// <summary>
/// 後始末する
/// </summary>
void SoundManager::Finalize()
{
    //現在のシーンの処理する
	switch (m_soundId)
	{
	case SoundManager::TITLE:
		m_titleSound->Finalize();
		break;
	case SoundManager::PLAY:
		m_playSound->Finalize();
		break;
	case SoundManager::RESULT:
		m_resultSound->Finalize();
		break;
	default:
		break;
	}
}


/// <summary>
/// 敵が接近したことを通知する
/// </summary>
void SoundManager::NotifyEnemyApproach()
{
   m_playSound->PlayerHeartSlowBeat();
}
