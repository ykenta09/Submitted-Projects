/*
	@file	ObjectBase.cpp
	@brief	
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Base/ObjectBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="initialPosition">�������W</param>
/// <param name="initialQuaternion">������]</param>
/// <param name="scale">�X�P�[��</param>
ObjectBase::ObjectBase(
   const DirectX::SimpleMath::Vector3& initialPosition,
   const DirectX::SimpleMath::Quaternion& initialQuaternion,
   const DirectX::SimpleMath::Vector3& scale)
   :
   m_graphics(yamadalib::Graphics::GetInstance())		 //�O���t�B�b�N�X�̃C���X�^���X���擾
{
   m_parameter.position = initialPosition;		   //�I�u�W�F�N�g�̏������W��ݒ�
   m_parameter.rotation = initialQuaternion;	   //�I�u�W�F�N�g�̏�����]��ݒ�
   m_parameter.scale = scale;					   //�I�u�W�F�N�g�̃X�P�[����ݒ�
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
ObjectBase::~ObjectBase()
{
}

