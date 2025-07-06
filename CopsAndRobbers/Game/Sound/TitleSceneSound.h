/*
	@file	TitleSceneSound.h
	@brief	�T�E���h�}�l�[�W���[�N���X
*/
#ifndef TITLE_SOUND_DEFINED
#define TITLE_SOUND_DEFINED
#pragma once

// FMOD�p�̑O���錾
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class TitleSceneSound
{
public:
	TitleSceneSound();
	//�f�X�g���N�^
	~TitleSceneSound();
	//����������
	void Initialize();
	//�X�V����
	void Update();
	void Finalize();
public:
	//������ǂݍ���
	void LoadSound();
	//SE���Ăяo��
	void SelectionSE();
	//SE���Đ������ǂ����m�F����
	bool IsSEPlaying();
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
#endif // TITLE_SOUND_DEFINED
