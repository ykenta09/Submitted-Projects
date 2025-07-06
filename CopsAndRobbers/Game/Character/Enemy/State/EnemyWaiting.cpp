/*
	@file	EnemyWaiting.cpp
	@brief	�G�̑ҋ@�N���X
*/
#include "pch.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Enemy/State/EnemyWaiting.h"
#include <Libraries/yamadaLib/Resources.h>
#include "GraphEditor/GraphScene.h"
#include "Libraries/yamadaLib/Math.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemy">�G�̃|�C���^</param>
EnemyWaiting::EnemyWaiting(Enemy* enemy)
	:
	m_enemy(enemy),
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_waitTime (0.0f),
    m_isRotationComplete(false)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyWaiting::~EnemyWaiting()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
void EnemyWaiting::Initialize()
{	
}


/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyWaiting::Update(const float& elapsedTime)
{
   //���̈ړ���̕����ɉ�]����
   RotateToNextDestination(elapsedTime);
}


/// <summary>
/// �`�悷��
/// </summary>
void EnemyWaiting::Render()
{

#ifdef _DEBUG

#endif // _DEBUG

}


/// <summary>
/// ��n������
/// </summary>
void EnemyWaiting::Finalize()
{
	// do nothing.
}

/// <summary>
/// ���̈ړ�������ɉ�]
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void EnemyWaiting::RotateToNextDestination(const float elapsedTime)
{
   m_waitTime += elapsedTime;

   // �ڕW�p�x�̌v�Z
   const float rotationSpeed = DirectX::XMConvertToRadians(60.0f);
   // ���݂̊p�x���玟�̈ړ���̕���
   float angleDiff = m_enemy->GetNextAngle() - m_enemy->GetAngle();

   // �ŒZ�����ŉ�]���邽�߂ɁA�p�x���𐳋K������
   if (std::abs(angleDiff) > DirectX::XM_PI)
   {
      if (angleDiff > 0)
      {
         angleDiff -= 2 * DirectX::XM_PI;
      }
      else
      {
         angleDiff += 2 * DirectX::XM_PI;
      }
   }

   //�ҋ@���Ԃ��I���������]���J�n
   if (m_waitTime >= Enemy::WAIT_INTERVAL)
   {
      // �p�x���Ɋ�Â��ĉ�]
      m_enemy->SetAngle(yamadalib::Math::Lerp(m_enemy->GetAngle(), m_enemy->GetAngle() + angleDiff, rotationSpeed * elapsedTime));

      // ��]�������������m�F
      if (std::abs(angleDiff) <= DirectX::XMConvertToRadians(1.0f))
      {
         m_isRotationComplete = true;
      }
      else
      {
         m_isRotationComplete = false;
      }

      if (m_isRotationComplete)
      {
         m_waitTime = 0.0f;
         m_enemy->ChangeState(m_enemy->GetEnemyWandering());
      }
   }
}