/*
	@file	PlaySceneSound.h
	@brief	�T�E���h�}�l�[�W���[�N���X
*/
#ifndef PLAY_SOUND_DEFINED
#define PLAY_SOUND_DEFINED
#pragma once
#include <unordered_map>
#include <string>

// FMOD�p�̑O���錾
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

class PlaySceneSound
{
public:
	PlaySceneSound();
	//�f�X�g���N�^
	~PlaySceneSound();
	//����������
	void Initialize();
	//�X�V����
	void Update();
	void Finalize();
	void PlayerHeartSlowBeat();
public:
	void LoadSound();		//�T�E���h�����[�h����
	void SetVolumeBGM(float volume);
	void SetVolumeSE(float volume);
	void LoadSE(const std::string& path, const std::string& name);
	void LoadBGM(const std::string& path, const std::string& name);
	void PlaySE(const std::string& name);
	void PlayBGM(const std::string& name);
	float GetVolumeBGM() const { return m_volumeBGM; }
	float GetVolumeSE() const { return m_volumeSE; }
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	//FMOD
	FMOD::System* m_system;	// FMOD�̃V�X�e��
	std::unordered_map<std::string, FMOD::Sound*> m_soundSE;
	std::unordered_map<std::string, FMOD::Sound*> m_soundBGM;
	//FMOD::Sound* m_soundSE;	// SE�p�̉����f�[�^
	//FMOD::Sound* m_soundBGM;	// BGM�p�̉����f�[�^
	std::unordered_map<std::string, FMOD::Channel*> m_channelSE;
	FMOD::Channel* m_channelBGM;
	//FMOD::Channel* m_channelSE;	// SE���Đ�����`�����l��
	//FMOD::Channel* m_channelBGM;// BGM���Đ�����`�����l��
	float m_volumeBGM;
	float m_volumeSE;

};
#endif // PLAY_SOUND_DEFINED
