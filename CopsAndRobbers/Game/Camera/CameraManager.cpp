/*
	@file	CameraManager.cpp
	@brief	�J�����}�l�[�W���N���X
*/
#include "pch.h"
#include "CameraManager.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>

/// <summary>
/// �R���X�g���N�^
/// �����̃J����(TPS�J�����A�g�b�v�_�E���r���[�J�����A�Œ�J����)�𐶐�
/// </summary>
CameraManager::CameraManager()
	:
   m_currentCamera{}
{
   m_tpsCamera = std::make_unique<TPSCamera>();
   m_topDownViewCamera = std::make_unique<TopDownViewCamera>();
   m_fixedCamera = std::make_unique<FixedCamera>(DirectX::SimpleMath::Vector3(0.0f, 10.0f, 60.0f), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 40.0f));
   //�����J����
   m_currentCamera = m_tpsCamera.get();
}

/// <summary>
/// �f�X�g���N�^
/// �\�[�X�̌�n�����s��
/// </summary>
CameraManager::~CameraManager()
{
	Finalize();
}

/// <summary>
/// �J�����̏�Ԃ��X�V����
/// </summary>
/// <param name="newTarget">�J��������������^�[�Q�b�g���W</param>
/// <param name="rotate">�J�����̉�]�s��</param>
/// <param name="boundingBoxes">�J�����̏Փ˔��苫�E�{�b�N�X�̔z��</param>
void CameraManager::Update(
   const DirectX::SimpleMath::Vector3& newTarget,
   const DirectX::SimpleMath::Matrix& rotate,
   const std::vector<DirectX::BoundingBox>& boundingBoxes)
{
   m_currentCamera->Update(newTarget, rotate, boundingBoxes);
}

/// <summary>
/// ���\�[�X�̉���E��n��������
/// </summary>
void CameraManager::Finalize()
{
}
