/*
	@file	IScene.h
	@brief	�V�[���̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef ISCENE_DEFINED
#define ISCENE_DEFINED

// �O���錾
class CommonResources;

class IScene
{
public:
	// �V�[���h�c
	enum class SceneID : unsigned int
	{
		NONE,
		TITLE,
		PLAY,
		STAGE_SELECT,
		RESULT
	};


public:
	virtual ~IScene() = default;

	virtual void Initialize(CommonResources* resources) = 0;

	virtual void Update(float elapsedTime) = 0;

	virtual void Render() = 0;

	virtual void Finalize() = 0;

	virtual SceneID GetNextSceneID() const = 0;
};
#endif // ISCENE_DEFINED
