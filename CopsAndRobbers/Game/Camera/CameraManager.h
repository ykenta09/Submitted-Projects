/*
	@file	CameraManager.h
	@brief  �J�����̃}�l�[�W���N���X
*/
#pragma once
#ifndef CAMERA_MANAGER_DEFINED
#define CAMERA_MANAGER_DEFINED

#include "Game/Interface/ICamera.h"
#include "Game/Camera/FixedCamera.h"
#include "Game/Camera/TopDownViewCamera.h"
#include "Game/Camera/TPSCamera.h"

// �O���錾
class CommonResources;

class CameraManager
{
public:
   //���݂̃J�����̃r���[�s����擾����
   DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_currentCamera->GetViewMatrix(); }
   //���݂̃v���W�F�N�V�����s����擾����
   DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return m_currentCamera->GetProjectionMatrix(); }
   //�J�������W���擾����
   const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_currentCamera->GetEyePosition(); }

   //���݂̃J�������擾����
   ICamera* GetCurrentCamera() const { return m_currentCamera; }
public:
   //�R���X�g���N�^
   CameraManager();
   //�f�X�g���N�^
	~CameraManager();
	//�X�V����
	void Update(
	   const DirectX::SimpleMath::Vector3& newTarget = DirectX::SimpleMath::Vector3::Zero,
	   const DirectX::SimpleMath::Matrix& rotate = DirectX::SimpleMath::Matrix::Identity,
	   const std::vector<DirectX::BoundingBox>& boundingBoxes = std::vector<DirectX::BoundingBox>());
	//��n��������
	void Finalize();
public:
   //���݂̃J������TPS�J�����ɐ؂�ւ���
   void SetTPSCamera() { m_currentCamera = m_tpsCamera.get(); };
   //���݂̃J�����������낵�J�����ɐ؂�ւ���
   void SetTopDownViewCamera() { m_currentCamera = m_topDownViewCamera.get(); }
   //���݂̃J������TPS�J�����ɐ؂�ւ���
   void SetFixedCamera() { m_currentCamera = m_fixedCamera.get(); }
private:
   ICamera* m_currentCamera;								 //���݃A�N�e�B�u�ȃJ����
   std::unique_ptr<TPSCamera> m_tpsCamera;					 //TPS�J����
   std::unique_ptr<TopDownViewCamera> m_topDownViewCamera;   //�����낵�J����
   std::unique_ptr<FixedCamera> m_fixedCamera;				 //�Œ�J����

};
#endif // CAMERA_MANAGER_DEFINED
