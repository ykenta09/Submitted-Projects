/*
	@file	IRoot.h
	@brief	ルートのインターフェイスクラス
*/
#pragma once
#ifndef IROOT_DEFINED
#define IROOT_DEFINED

#include "Game/CommonResources.h"
#include <vector>

class Enemy;
class Fellow;

class IRoot
{
public:
   virtual const std::vector<Enemy*>& GetEnemies() const = 0;
   virtual const std::vector<Fellow*>& GetFellows() const = 0;
};
#endif // IROOT_DEFINED
