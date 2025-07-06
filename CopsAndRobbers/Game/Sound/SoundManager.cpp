/*
	@file	SoundManager.cpp
	@brief	�T�E���h�}�l�[�W���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/SoundManager.h"
//�e�V�[���̃T�E���h
#include "Game/Sound/TitleSceneSound.h"
#include "Game/Sound/PlaySceneSound.h"
#include "Game/Sound/ResultSceneSound.h"
//FMOD�̃C���N���[�h
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::SoundManager()
	:
	m_soundId{}
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
SoundManager::~SoundManager()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
/// <param name="sceneid">�V�[����ID</param>
void SoundManager::Initialize(SoundSceneID sceneid)
{
    //���݂̃V�[����ݒ肷��
	m_soundId = sceneid;
	//���݂̃V�[���̏�������
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
/// �X�V����
/// </summary>
void SoundManager::Update()
{
   //���݂̃V�[���̏�������
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
/// ��n������
/// </summary>
void SoundManager::Finalize()
{
    //���݂̃V�[���̏�������
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
/// �G���ڋ߂������Ƃ�ʒm����
/// </summary>
void SoundManager::NotifyEnemyApproach()
{
   m_playSound->PlayerHeartSlowBeat();
}
