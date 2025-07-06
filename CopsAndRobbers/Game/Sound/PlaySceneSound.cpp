/*
	@file	PlaySceneSound.cpp
	@brief	�v���C�V�[���T�E���h�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/PlaySceneSound.h"
//FMOD�̃C���N���[�h
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include "PlaySceneSound.h"

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
PlaySceneSound::~PlaySceneSound()
{
}


/// <summary>
///  ����������
/// </summary>
void PlaySceneSound::Initialize()
{
	//�T�E���h��ǂݍ���
	LoadSound();
}


/// <summary>
/// �X�V����
/// </summary>
void PlaySceneSound::Update()
{
	FMOD_RESULT result;

	// ��d�Đ����Ȃ�
	if (m_channelBGM == nullptr)
	{
	    PlayBGM("Normal");
	}

	// FMOD�̃V�X�e�����X�V����
	result = m_system->update();
	assert(result == FMOD_OK);

}


/// <summary>
/// �T�E���h��ǂݍ���
/// </summary>
void PlaySceneSound::LoadSound()
{
	// �V�X�e�����C���X�^���X������
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// �V�X�e��������������
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SE�����[�h����
	LoadBGM("Resources/Sounds/HeartSlowBeat.ogg", "HeartSlow");

	// BGM�����[�h����
	LoadBGM("Resources/Sounds/PlaySceneBGM.ogg", "Normal");
}


/// <summary>
/// SE��ǂݍ���
/// </summary>
/// <param name="path">SE�̃t�@�C���p�X</param>
/// <param name="name">���O</param>
void PlaySceneSound::LoadSE(const std::string& path, const std::string& name)
{
   FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_soundSE[name]);
   assert(result == FMOD_OK);
}


/// <summary>
/// BGM��ǂݍ���	
/// </summary>
/// <param name="path">BGM�̃t�@�C���p�X</param>
/// <param name="name">���O</param>
void PlaySceneSound::LoadBGM(const std::string& path, const std::string& name)
{
   FMOD_RESULT result = m_system->createSound(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_soundBGM[name]);
   assert(result == FMOD_OK);
}

/// <summary>
/// SE���Đ�����
/// </summary>
/// <param name="name">�Đ�����SE�̖��O</param>
void PlaySceneSound::PlaySE(const std::string& name)
{

   FMOD_RESULT result = m_system->playSound(m_soundSE[name], nullptr, false, &m_channelSE[name]);
   assert(result == FMOD_OK);
   //���ʐݒ�
   result = m_channelSE[name]->setVolume(m_volumeSE);
   assert(result == FMOD_OK);
}

/// <summary>
/// BGM���Đ�����
/// </summary>
/// <param name="name">�Đ�����BGM�̖��O</param>
void PlaySceneSound::PlayBGM(const std::string& name)
{

   if (m_soundBGM.find(name) != m_soundBGM.end())
   {
	  FMOD_RESULT result = m_system->playSound(m_soundBGM[name], nullptr, false, &m_channelBGM);
	  assert(result == FMOD_OK);
	  //���ʐݒ�
	  result = m_channelBGM->setVolume(m_volumeBGM);
	  assert(result == FMOD_OK);
   }
}


/// <summary>
/// ��n������
/// </summary>
void PlaySceneSound::Finalize()
{
	// Sound�p�̃I�u�W�F�N�g���������
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
/// �S�����Đ�����
/// </summary>
void PlaySceneSound::PlayerHeartSlowBeat()
{
// �ʂ̃`�����l���ŉ����Đ�����
   FMOD::Channel* channel;
   FMOD_RESULT result = m_system->playSound(m_soundBGM["HeartSlow"], nullptr, false, &channel);
   assert(result == FMOD_OK);

   // ���ʐݒ�
   result = channel->setVolume(m_volumeBGM);
   assert(result == FMOD_OK);

}