/*
	@file	MouseControl.h
	@brief	�}�E�X����N���X
*/
#ifndef MOUSE_CONTROL_DEFINED
#define MOUSE_CONTROL_DEFINED
#pragma once
// �O���錾
class CommonResources;


class MouseControl
{
public:
	//�}�E�X�̃X�e�[�g���擾����
	DirectX::Mouse::State GetMouseState() { return m_mouseState; }
	//�J�����p�̍s����擾����
	DirectX::SimpleMath::Matrix GetCameraMatrix() { return m_mouseMatrix; }

	//�J������Y����]���擾����
	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }

	void SetYaw(const float& yaw) { m_yaw = yaw; }
public:
    //�R���X�g���N�^
	MouseControl(CommonResources* commonResources);
	//�f�X�g���N�^
	~MouseControl();
	//����������
	void Initialize(CommonResources* resources);
	//�X�V����
	void Update(float elapsedTime);
	//�N�H�[�^�j�I���̌v�Z
	void QuaternionCalculation();
	//�L�[���삷��
	void OnKeyPressed();
private:
	CommonResources* m_commonResources;	// ���ʃ��\�[�X

	DirectX::Mouse::State m_mouseState;	//�}�E�X�̃X�e�[�g
	DirectX::Keyboard::State m_keyboardState;	//�L�[�{�[�h�X�e�[�g

	DirectX::SimpleMath::Matrix m_mouseMatrix;	//�}�E�X�̍s��

	float m_yaw;	//�}�E�X�̉�]�p�iX���j
	float m_pitch;	//�}�E�X�̉�]�p�iY���j
};
#endif // MOUSE_CONTROL_DEFINED
