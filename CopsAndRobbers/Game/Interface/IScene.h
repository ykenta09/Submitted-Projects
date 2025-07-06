/*
	@file	IScene.h
	@brief	シーンのインターフェイスクラス
*/
#pragma once
#ifndef ISCENE_DEFINED
#define ISCENE_DEFINED

// 前方宣言
class CommonResources;

class IScene
{
public:
	// シーンＩＤ
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
