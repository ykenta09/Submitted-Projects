/*
	@file	SoundManager.h
	@brief	�T�E���h�}�l�[�W���[�N���X
*/
#ifndef SOUND_MANAGER_DEFINED
#define SOUND_MANAGER_DEFINED
#pragma once

// FMOD�p�̑O���錾
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

// �O���錾
class TitleSceneSound;
class PlaySceneSound;
class ResultSceneSound;;

class SoundManager
{
public:
	enum SoundSceneID : int
	{
		TITLE,
		PLAY,
		RESULT
	};
public:
	//�^�C�g���̃T�E���h���擾����
	TitleSceneSound* GetTitle() { return m_titleSound.get(); }
	PlaySceneSound* GetPlay() { return m_playSound.get(); }
	ResultSceneSound* GetResult() { return m_resultSound.get(); }
public:
	SoundManager();
	//�f�X�g���N�^
	~SoundManager();
	//����������
	void Initialize(SoundSceneID sceneid);
	//�X�V����
	void Update();
	void Finalize();

	void NotifyEnemyApproach();

private:
	//SceneSound
	SoundSceneID m_soundId;

	//�V�[���̃T�E���h
	std::unique_ptr<PlaySceneSound> m_playSound;
	std::unique_ptr<TitleSceneSound> m_titleSound;
	std::unique_ptr<ResultSceneSound> m_resultSound;

};
#endif // SOUND_MANAGER_DEFINED
