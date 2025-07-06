/*
	@file	IGimmick.h
	@brief	�M�~�b�N�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#include <SimpleMath.h>
#ifndef IGIMMICK_DEFINED
#define IGIMMICK_DEFINED

class IGimmick
{
public:
   //�A�N�e�B�u�ɂ���
   virtual void Active() = 0;
   //��A�N�e�B�u�ɂ���
   virtual void Deactive() = 0;
   //�������I��������
   virtual bool IsFinished() = 0;
   //�����I���t���O�����Z�b�g����
   virtual void Reset() = 0;
};
#endif // IGIMMICK_DEFINED
