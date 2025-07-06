/*
	@file	ResultSound.cpp
	@brief	���U���g�T�E���h�V�[���N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Sound/ResultSceneSound.h"
//FMOD�̃C���N���[�h
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include "PlaySceneSound.h"


/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
ResultSceneSound::~ResultSceneSound()
{
}


/// <summary>
///  ����������
/// </summary>
void ResultSceneSound::Initialize()
{
	//�T�E���h��ǂݍ���
	LoadSound();
}


/// <summary>
/// �X�V����
/// </summary>
void ResultSceneSound::Update()
{
	FMOD_RESULT result;

	// ��d�Đ����Ȃ�
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
	}

	// FMOD�̃V�X�e�����X�V����
	result = m_system->update();
	assert(result == FMOD_OK);

}


/// <summary>
/// �T�E���h��ǂݍ���
/// </summary>
void ResultSceneSound::LoadSound()
{
	// �V�X�e�����C���X�^���X������
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// �V�X�e��������������
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SE�����[�h����
	result = m_system->createSound("Resources/Sounds/Blast.ogg", FMOD_DEFAULT, nullptr, &m_soundSE);
	assert(result == FMOD_OK);

	// BGM�����[�h����
	result = m_system->createSound("Resources/Sounds/PlaySceneBGM.ogg", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);

}


/// <summary>
/// BGM�̉��ʂ�ݒ肷��
/// </summary>
/// <param name="volume">�ݒ肷�鉹��</param>
void ResultSceneSound::SetVolumeBGM(float volume)
{
   m_volumeBGM = volume;
   //���ʂ�K�p����
   ApplyVolume();
}


/// <summary>
/// SE�̉��ʂ�ݒ肷��
/// </summary>
/// <param name="volume">�ݒ肷�鉹��</param>
void ResultSceneSound::SetVolumeSE(float volume)
{
   m_volumeSE = volume;
   //���ʂ�K�p����
   ApplyVolume();
}

/// <summary>
/// ���ʂ�K�p����
/// </summary>
void ResultSceneSound::ApplyVolume()
{
   //BGM��SE���ꂼ�ꉹ�ʂ�K�p����
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
void ResultSceneSound::Finalize()
{
	// Sound�p�̃I�u�W�F�N�g���������
	m_soundSE->release();
	m_soundBGM->release();
	m_system->release();
}

