/*
	@file	DeviceObjectBase.cpp
	@brief	
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Map/Base/DeviceObjectBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="initialPosition">�������W</param>
/// <param name="initialQuaternion">������]</param>
/// <param name="scale">�X�P�[��</param>
DeviceObjectBase::DeviceObjectBase(
   const DirectX::SimpleMath::Vector3& initialPosition,
   const DirectX::SimpleMath::Quaternion& initialQuaternion,
   const DirectX::SimpleMath::Vector3& scale)
   :
   m_isInsideControlZone(false),                         //���u�̑���͈͂ɂ��邩�̃t���O��������
   m_isTriggered(false),                                 //���u�̋N�����t���O��������
   m_isEnabled(false),                                   //���u��OFF�ŏ���������
   m_graphics(yamadalib::Graphics::GetInstance())		 //�O���t�B�b�N�X�̃C���X�^���X���擾
{
   m_parameter.position = initialPosition;		   //�I�u�W�F�N�g�̏������W��ݒ�
   m_parameter.rotation = initialQuaternion;	   //�I�u�W�F�N�g�̏�����]��ݒ�
   m_parameter.scale = scale;					   //�I�u�W�F�N�g�̃X�P�[����ݒ�
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
DeviceObjectBase::~DeviceObjectBase()
{
}


/// <summary>
/// �L�[�{�[�h�̓��͏���
/// </summary>
/// <param name="key"></param>
void DeviceObjectBase::OnKeyPressed(const DirectX::Keyboard::Keys& key)
{
   //���u�̑���͈͊O�Ȃ珈�����Ȃ�
   if (!m_isInsideControlZone)return;
   //���u���N�����͓��͂��󂯕t���Ȃ�
   if (m_isTriggered) return;

   //�o�^�����M�~�b�N���N��
   if (key == DirectX::Keyboard::E)
   {
      m_isTriggered = true;            //���u���N�����ɂ���
      m_isEnabled = !m_isEnabled;      //���u��ON�EOFF��؂�ւ���
   }
}

/// <summary>
/// �M�~�b�N�I�u�W�F�N�g��o�^����
/// </summary>
/// <param name="gimmick">�M�~�b�N�I�u�W�F�N�g</param>
void DeviceObjectBase::RegisterGimmick(IGimmick* gimmick)
{
   m_targets.push_back(gimmick);
}


/// <summary>
/// �M�~�b�N���N������
/// </summary>
/// <param name="flag">�M�~�b�N�̋@���t���O</param>
void DeviceObjectBase::Trigger(bool flag)
{
   if (flag)//�A�N�e�B�u
   {
      for (auto const target : m_targets)
      {
         //�������I�����Ă��Ȃ��M�~�b�N����������
         if(!target->IsFinished())  
            target->Active();
      }
   }
   else //��A�N�e�B�u
   {
      for (auto const target : m_targets)
      {
         //�������I�����Ă��Ȃ��M�~�b�N����������
         if (!target->IsFinished())
            target->Deactive();
      }
   }
}

/// <summary>
/// �o�^���ꂽ�M�~�b�N�������I��������
/// </summary>
void DeviceObjectBase::AreAllGimmicksFinished()
{
   for (const auto& target : m_targets)
   {
      if (!target->IsFinished())
      {
         m_isTriggered = true;
         return;
      }     
   }
   //�I���t���O�����Z�b�g����
   GimmicksReset();
   m_isTriggered = false;
}


/// <summary>
/// �o�^�����M�~�b�N�̏I���t���O�����Z�b�g����
/// </summary>
void DeviceObjectBase::GimmicksReset()
{
   for (const auto& target : m_targets)
   {
      target->Reset();
   }
}
