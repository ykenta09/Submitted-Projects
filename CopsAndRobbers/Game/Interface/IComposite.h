/*
	@file	IPlayer.h
	@brief	プレイヤーのインターフェイスクラス
*/
#pragma once
#ifndef ICOMPOSITE_DEFINED
#define ICOMPOSITE_DEFINED

#include "Game/Interface/IComponent.h"


class IComposite : public IComponent
{
public:
	//パーツを追加する
	virtual void Attach(std::unique_ptr<IComponent> playerParts) = 0;
	//パーツを削除する
	virtual void Detach(std::unique_ptr<IComponent> playerParts) = 0;
	virtual IComponent* GetLastAttachedNode() const = 0;

};
#endif // ICOMPOSITE_DEFINED
