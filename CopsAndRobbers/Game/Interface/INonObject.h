/*
	@file	INonObject.h
	@brief	��I�u�W�F�N�g�C���^�[�t�F�C�X�N���X
*/
#ifndef INON_OBJECT_DEFINED
#define INON_OBJECT_DEFINED
#pragma once
#include "pch.h"
#include "Game/Interface/IObserver.h"

class INonObject : public IObserver
{
public:
   virtual ~INonObject() = default;
   virtual void Initialize() = 0;						 //����������
   virtual void Update(const float& elapsedTime) = 0;	 //�X�V����
   virtual void Render() = 0;							 //�`��
   virtual void Finalize() = 0;						    //�㏈��

   //�L�[�{�[�h�������ꂽ�Ƃ��ɌĂяo�����
   virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};
#endif // INON_OBJECT_DEFINED
