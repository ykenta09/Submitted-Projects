/*
	@file	PlayerIdle.h
	@brief	�v���C���[�ҋ@��ԃN���X
*/
#pragma once
#ifndef PLAYER_IDLE_DEFINED
#define PLAYER_IDLE_DEFINED

#include "Game/Interface/IState.h"

// �O���錾
class Player;

class PlayerIdle : public IState
{
public:
	//�R���X�g���N�^
	PlayerIdle(Player* player);
	//�f�X�g���N�^
	~PlayerIdle();
	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime);
	//�`�悷��
	void Render() ;
	//�㏈������
	void Finalize();
private:
	//�v���C���[
	Player* m_player;
};
#endif // PLAYER_IDLE_DEFINED
