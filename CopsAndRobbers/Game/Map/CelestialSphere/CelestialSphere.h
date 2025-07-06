/*
	@file	CelestialSphere.h
	@brief	�V���N���X
*/
#pragma once
#ifndef CELESTISL_SPHERE_DEFINDE
#define  CELESTISL_SPHERE_DEFINDE

#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;

class CelestialSphere
{
public:
   static constexpr float ROTATION_SPEED = { DirectX::XMConvertToRadians(0.01f) };		   //�V���̉�]���x
public:
   //�R���X�g���N�^
	CelestialSphere(const DirectX::SimpleMath::Vector3& initialPosition,
		const DirectX::SimpleMath::Quaternion&  initialQuaternion);
	//�f�X�g���N�^
	~CelestialSphere();
	//������
	void Initialize();
	//�X�V����
	void Update(const DirectX::SimpleMath::Vector3& centerPosition);
	//�`�悷��
	void Render();
	//��n��������
	void Finalize();

private:
    yamadalib::Graphics* m_graphics;   //�O���t�B�b�N�X
	DirectX::Model* m_model;	//���f��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //�e�N�X�`��

	DirectX::SimpleMath::Vector3 m_position;	//�������W

	DirectX::SimpleMath::Quaternion m_quaternion;		 //������]�p
	DirectX::SimpleMath::Quaternion m_currentQuaternion; //���݂̉�]�p
};

#endif // CELESTISL_SPHERE_DEFINDE
