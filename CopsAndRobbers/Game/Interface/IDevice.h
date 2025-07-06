/*
	@file	IDevice.h
	@brief	�M�~�b�N���u�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#include <SimpleMath.h>
#include "Game/Interface/IGimmick.h"
#ifndef IDEVICE_DEFINED
#define IDEVICE_DEFINED

class IDevice
{
public:
   virtual void RegisterGimmick(IGimmick* gimmick) = 0;
   virtual void Trigger(bool flag) = 0;
};
#endif // IDEVICE_DEFINED
