/*
	@file	EnemyTransmission .h
	@brief	�G�̓`�B��ԃN���X
*/
#ifndef ENEMY_TRANSMISSION_DEFINED
#define ENEMY_TRANSMISSION_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include <GraphEditor/Graph.h>

// �O���錾
class CommonResources;
class GraphScene;
class Enemy;


class EnemyTransmission : public IState
{
public:
   //�G�̓`�B����
   static const float TRANSMISSION_DISTANCE;
public:
public:
	//�R���X�g���N�^
    EnemyTransmission(Enemy* enemy);
	//�f�X�g���N�^
	~EnemyTransmission();

	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();

	//�`�B����
	void Transmission();
private:
	//�G
	Enemy* m_enemy;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�f�[�^
	GraphScene* m_graphScene;
	//�O���t�B�b�N�X�@�f�o�b�N�p
	yamadalib::Graphics* m_graphics;
	//�v���C���[�̉�]�s��
	DirectX::SimpleMath::Matrix m_enemyMatrix;

	//�^�[�Q�b�g�̍��W
	DirectX::SimpleMath::Vector3 m_targetPosition;
	//���E�{�b�N�X
	DirectX::BoundingBox m_boundingBox;
};
#endif // ENEMY_TRANSMISSION_DEFINED
