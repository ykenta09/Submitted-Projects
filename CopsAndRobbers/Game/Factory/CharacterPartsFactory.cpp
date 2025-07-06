/*
	@file	CharacterPartsFactory.cpp
	@brief	�L�����N�^�[�p�[�c�̃t�@�N�g���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Factory/CharacterPartsFactory.h"

#include "Game/Character/Base/Parts/Head.h"
#include "Game/Character/Base/Parts/Body.h"
#include "Game/Character/Base/Parts/Arm.h"
#include "Game/Character/Base/Parts/Foot.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateCharacter(
   IComponent* parent,
   CommonResources* resources,
   const DirectX::SimpleMath::Vector3 initialPosition,
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale)
{
	std::unique_ptr<IComponent> character;
	//�C���X�^���X�𐶐�����
	character.reset(new CharacterBase(parent, resources, initialPosition, initialAngle, scale));
	//����������
	character->Initialize();
	//�C���X�^���X��Ԃ�
	return std::move(character);
}


/// <summary>
/// ���𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="model">���f��</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateHeadParts(
   IComponent* parent, 
   CommonResources* resources, 
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition, 
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale)
{
	//����錾����
	std::unique_ptr<IComponent> headParts;
	//�C���X�^���X�𐶐�����
	headParts.reset(new Head(parent, resources, model,initialPosition, initialAngle, scale));
	//����������
	headParts->Initialize();

	//�C���X�^���X��Ԃ�
	return std::move(headParts);
}


/// <summary>
/// ���̂𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="model">���f��</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateBodyParts(
   IComponent* parent, 
   CommonResources* resources,
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition,
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale)
{
	//���̂�錾����
	std::unique_ptr<IComponent> bodyParts;
	//�C���X�^���X�𐶐�����
	bodyParts.reset(new Body(parent, resources, model, initialPosition, initialAngle, scale));
	//����������
	bodyParts->Initialize();

	//�C���X�^���X��Ԃ�
	return std::move(bodyParts);
}

/// <summary>
/// �r�𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="model">���f��</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateArmParts(
   IComponent* parent, 
   CommonResources* resources,
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition,
   const float& initialAngle,
   const DirectX::SimpleMath::Vector3& scale, 
   const CharacterBase::PartID& partId)
{
	//�r��錾����
	std::unique_ptr<IComponent> armParts;
	//�C���X�^���X�𐶐�����
	armParts.reset(new Arm(parent, resources, model, initialPosition, initialAngle, scale, partId));
	//����������
	armParts->Initialize();

	//�C���X�^���X��Ԃ�
	return std::move(armParts);
}

/// <summary>
/// ���𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="model">���f��</param>
/// <param name="initialPosition">�������W</param>
/// <param name="initialAngle">������]</param>
/// <param name="scale">�X�P�[��</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterPartsFactory::CreateFootParts(
   IComponent* parent, 
   CommonResources* resources,
   const CharacterBase::CharacterResources& model,
   const DirectX::SimpleMath::Vector3& initialPosition,
   const float& initialAngle, 
   const DirectX::SimpleMath::Vector3& scale,
   const CharacterBase::PartID& partId)
{
	//����錾����
	std::unique_ptr<IComponent> footParts;
	//�C���X�^���X�𐶐�����
	footParts.reset(new Foot(parent, resources, model, initialPosition, initialAngle, scale, partId));
	//����������
	footParts->Initialize();

	//�C���X�^���X��Ԃ�
	return std::move(footParts);
}
