/*
	@file	EnemyTransmission.cpp
	@brief	�G�̓`�B�N���X
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyTransmission.h"
#include <random>
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"


//�G�̓`�B����
const float EnemyTransmission::TRANSMISSION_DISTANCE = 5.0f;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
EnemyTransmission::EnemyTransmission(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphScene{},
	m_enemyMatrix( DirectX::SimpleMath::Matrix::Identity ),
	m_graphics(yamadalib::Graphics::GetInstance())
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyTransmission::~EnemyTransmission()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void EnemyTransmission::Initialize()
{	
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void EnemyTransmission::Update(const float& elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
   Transmission();
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void EnemyTransmission::Render()
{

#ifdef _DEBUG

#endif // _DEBUG
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void EnemyTransmission::Finalize()
{
	// do nothing.
}

//����ɓ`�B����
void EnemyTransmission::Transmission()
{
}
