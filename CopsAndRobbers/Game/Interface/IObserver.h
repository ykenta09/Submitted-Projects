/*
	@file	IObserver.h
	@brief	�I�u�U�[�o�[�̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef IOBSERVER_DEFINED
#define IOBSERVER_DEFINED

class ISubject;

class IObserver
{
public:
	//�L�[�{�[�h�������ꂽ�Ƃ��ɌĂяo�����
	virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};
#endif // IOBSERVER_DEFINED
