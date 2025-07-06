/*
	@file	Stamina.h
	@brief	�X�^�~�i�Ǘ��N���X
*/
#pragma once
#ifndef STAMINA_DEFINED
#define STAMINA_DEFINED
#include "Libraries/yamadaLib/Graphics.h"

// �O���錾
class CommonResources;


class Stamina
{
public:
	static constexpr float MAX_STAMINA_GUAGE = 150.0f;		//�ő�X�^�~�i��
	static constexpr float CONSUME_STAMINA = 15.0f;			//�����X�^�~�i��
public:
	//�X�^�~�i�Q�[�W���擾����
	float GetStaminaGuage() { return m_staminaGuage; }
	//�X�^�~�i�̍ő�l���擾����
	float GetMaxStaminaGuage() { return MAX_STAMINA_GUAGE; }
	//�������X�^�~�i�����邩
	bool IsAvoidance();
	//�X�^�~�i�������
	void ConsumeStamina();
public:
    //�R���X�g���N�^
	Stamina();
	//�f�X�g���N�^
	~Stamina();
	//����������
	void Initialize();
	//�X�V����
	void Update(float elapsedTime);
	//�X�^�~�i���R��
	void NaturalStaminaRecovery(const float& elapsedTime);
private:
	yamadalib::Graphics* m_graphics;	//�O���t�B�b�N�X
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	// �Q�[�W�̉摜

	float m_staminaGuage;	//�X�^�~�i�Q�[�W
	float m_frameCounter;	//�t���[�����[�g�J�E���^�[
};
#endif // STAMINA_DEFINED
