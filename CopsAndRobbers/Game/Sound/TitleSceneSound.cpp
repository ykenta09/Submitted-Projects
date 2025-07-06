/*
	@file	TitleSound.cpp
	@brief	�^�C�g���V�[���T�E���h�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/TitleSceneSound.h"
//FMOD�̃C���N���[�h
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include "PlaySceneSound.h"


/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
TitleSceneSound::~TitleSceneSound()
{
}


/// <summary>
/// ����������
/// </summary>
void TitleSceneSound::Initialize()
{
	//�T�E���h��ǂݍ���
	LoadSound();
}

/// <summary>
/// �X�V����
/// </summary>
void TitleSceneSound::Update()
{
	FMOD_RESULT result;

	// ��d�Đ����Ȃ�
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
		ApplyVolume();
	}

	// FMOD�̃V�X�e�����X�V����
	result = m_system->update();
	assert(result == FMOD_OK);

}


/// <summary>
/// �T�E���h��ǂݍ���
/// </summary>
void TitleSceneSound::LoadSound()
{
	// �V�X�e�����C���X�^���X������
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// �V�X�e��������������
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SE�����[�h����
	result = m_system->createSound("Resources/Sounds/Button.ogg", FMOD_DEFAULT, nullptr, &m_soundSE);
	assert(result == FMOD_OK);

	// BGM�����[�h����
	result = m_system->createSound("Resources/Sounds/TitleBGM.ogg", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);

}


/// <summary>
/// �I����
/// </summary>
void TitleSceneSound::SelectionSE()
{
	FMOD_RESULT result;

	result = m_system->playSound(m_soundSE, nullptr, false, &m_channelSE);
	assert(result == FMOD_OK);
	ApplyVolume();
}

/// <summary>
/// SE���Đ������ǂ���
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
/// BGM���ʐݒ�
/// </summary>
/// <param name="volume">�ݒ肷�鉹��</param>
void TitleSceneSound::SetVolumeBGM(float volume)
{
   m_volumeBGM = volume;
   //���ʂ�K�p����
   ApplyVolume();
}

/// <summary>
/// SE���ʂ�ݒ肷��
/// </summary>
/// <param name="volume">�ݒ肷�鉹��</param>
void TitleSceneSound::SetVolumeSE(float volume)
{
   m_volumeSE = volume;
   //���ʂ�K�p����
   ApplyVolume();
}


/// <summary>
/// ���ʂ�K�p����
/// </summary>
void TitleSceneSound::ApplyVolume()
{
   //BGM��SE���ꂼ��̉��ʂ�K�p����
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
/// ��n������
/// </summary>
void TitleSceneSound::Finalize()
{
	// Sound�p�̃I�u�W�F�N�g���������
	if(m_soundSE) m_soundSE->release();
	if (m_soundBGM) m_soundBGM->release();
	if (m_system) m_system->release();
}

