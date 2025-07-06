/*
	@file	FellowIdle.h
	@brief	���ԑҋ@��ԃN���X
*/
#pragma once
#ifndef FELLOW_IDLE_DEFINED
#define FELLOW_IDLE_DEFINED

#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;
class Fellow;
class Map;

class FellowIdle : public IState
{
public:
   bool GetFellowMove() { return m_isMove; }
public:
	//�R���X�g���N�^
	FellowIdle(Fellow* fellow, Map* map);
	//�f�X�g���N�^
	~FellowIdle();
	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render();
	//�㏈������
	void Finalize();
	//�������Ԃɂ��邩
	void MoveFellow();
private:
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�v���C���[
	Fellow* m_fellow;
	Map* m_map;	//�}�b�v


	bool m_isMove;	//�G���������Ƃ��ł��邩
};
#endif // FELLOW_IDLE_DEFINED
