/*
	@file	ICamera.h
	@brief	�J�����̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#include <SimpleMath.h>
#ifndef ICAMERA_DEFINED
#define ICAMERA_DEFINED

// �O���錾
class CommonResources;

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual void Update(
	   const DirectX::SimpleMath::Vector3& newTarget = DirectX::SimpleMath::Vector3::Zero,
	   const DirectX::SimpleMath::Matrix& rotate = DirectX::SimpleMath::Matrix::Identity,
	   const std::vector<DirectX::BoundingBox>& boundingBoxes = std::vector<DirectX::BoundingBox>()) = 0;

	//�r���[�s����擾����
	virtual const DirectX::SimpleMath::Matrix& GetViewMatrix() const = 0;
	//�v���W�F�N�V�����s����擾����
	virtual const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const = 0;
	//�J�������W���擾����
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const = 0;

};
#endif // ICAMERA_DEFINED
