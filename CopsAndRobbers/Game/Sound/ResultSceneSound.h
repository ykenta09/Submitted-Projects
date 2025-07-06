/*
	@file	ResultSceneSound.h
	@brief	���U���g�T�E���h�N���X
*/
#ifndef RESULT_SOUND_DEFINED
#define RESULT_SOUND_DEFINED
#pragma once

// FMOD�p�̑O���錾
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
	//�f�X�g���N�^
	~ResultSceneSound();
	//����������
	void Initialize();
	//�X�V����
	void Update();
	void Finalize();
public:
	void LoadSound();	
	//���ʐݒ�
	void SetVolumeBGM(float volume);
	void SetVolumeSE(float volume);
	//���ʎ擾
	float GetVolumeBGM() const { return m_volumeBGM; }
	float GetVolumeSE() const { return m_volumeSE; }
	//���ʂ�K������
	void ApplyVolume();

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	//FMOD
	FMOD::System* m_system;	// FMOD�̃V�X�e��
	FMOD::Sound* m_soundSE;	// SE�p�̉����f�[�^
	FMOD::Sound* m_soundBGM;	// BGM�p�̉����f�[�^
	FMOD::Channel* m_channelSE;	// SE���Đ�����`�����l��
	FMOD::Channel* m_channelBGM;// BGM���Đ�����`�����l��
	float m_volumeBGM;			 //BGM�̉���
	float m_volumeSE;			 //SE�̉���
};
#endif // RESULT_SOUND_DEFINED
