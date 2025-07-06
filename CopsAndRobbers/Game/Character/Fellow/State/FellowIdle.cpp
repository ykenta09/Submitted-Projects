/*
	@file	FellowIdle.cpp
	@brief	�v���C���[�̑ҋ@��ԃN���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Fellow/State/FellowIdle.h"
#include "Game/Map/Map.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="fellow">���Ԗ{�̂̃|�C���^</param>
FellowIdle::FellowIdle(Fellow* fellow, Map* map)
	:
	m_fellow(fellow),
    m_map(map),
    m_isMove(false)
{
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FellowIdle::~FellowIdle()
{
	Finalize();
}


/// <summary>
/// ����������
/// </summary>
void FellowIdle::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void FellowIdle::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	//�������ԂȂ�T������
	if (m_isMove)
	{
	   //�ŏ��̍��G
	   m_fellow->ChangeState(m_fellow->GetFellowRouteSearch());
	   m_fellow->GetFellowRouteSearch()->SetSearchTime(0);
	   m_fellow->GetFellowRouteSearch()->SetAstar(m_fellow->GetTargetPosition());
	}
	else
	{
	   MoveFellow();
	}
}

/// <summary>
/// �`�悷��
/// </summary>
void FellowIdle::Render()
{
#ifdef _DEBUG
#endif // _DEBUG
}

/// <summary>
/// ��n������
/// </summary>
void FellowIdle::Finalize()
{
}


/// �S�����󂢂���ǐՏ�ԂɕύX
/// </summary>
void FellowIdle::MoveFellow()
{
   //�C���ӏ�
   auto prisons = m_map->GetObjectsByType<Prison>(m_fellow->GetObjectStageNumber());
   for (auto* prison : prisons)
   {
	  if (prison->GetOpenEndPrison())
	  {
		 m_isMove = true;
	  }
   }
}
