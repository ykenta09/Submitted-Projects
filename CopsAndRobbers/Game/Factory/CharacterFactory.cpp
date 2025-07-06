/*
	@file	CharacterFactory.cpp
	@brief	�L�����N�^�[�̃t�@�N�g���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Observer/Messenger.h"
#include "Game/Factory/CharacterFactory.h"

#include "Game/Character/Player/Player.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Map/Map.h"


/// <summary>
/// �L�����N�^�[�𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="map">�}�b�v�̃|�C���^</param>
/// <param name="data">�L�����N�^�[�̃f�[�^</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <param name="json">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreateCharacter(IComponent* parent, CommonResources* resources, Map* map, const GameParameters::CharacterData data, const int& stageNumber, LoadJson* json)
{
   //�f�[�^�̎�ނ��ƂɃL�����N�^�[�̐������s��
   if (data.name == "Player")
   {
      //�v���C���[�̐���
	  return CreatePlayer(parent, resources, data, stageNumber);
   }
   else if (data.name == "Fellow")
   {
	  //���Ԃ̐���
	  return CreateFellow(parent, resources, map, data, stageNumber);
   }
   else if (data.name == "Enemy")
   {
	  //�G�̐���
	  return CreateEnemy(parent, resources, 0, map, data, stageNumber, json);
   }

   return std::unique_ptr<IComponent>();
}

/// <summary>
/// �v���C���[�𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="data">�L�����N�^�[�̃f�[�^</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreatePlayer(IComponent* parent, CommonResources* resources, const GameParameters::CharacterData data, const int& stageNumber)
{
	std::unique_ptr<IComponent> player;
	//�C���X�^���X�𐶐�����
	player.reset(new Player(parent, resources, data, stageNumber));
	//����������
	player->Initialize();
	//�L�[���A�^�b�`����
	//�O�i��o�^
	Messenger::Attach(DirectX::Keyboard::Keyboard::W, player.get());
	//��i��o�^
	Messenger::Attach(DirectX::Keyboard::Keyboard::S, player.get());
	//���ړ���o�^
	Messenger::Attach(DirectX::Keyboard::Keyboard::A, player.get());
	//�E�ړ���o�^
	Messenger::Attach(DirectX::Keyboard::Keyboard::D, player.get());
	//�����ړ���o�^
	Messenger::Attach(DirectX::Keyboard::Keyboard::LeftShift, player.get());
	
	//�C���X�^���X��Ԃ�
	return std::move(player);
}

/// <summary>
/// ���Ԃ𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="map">�}�b�v�̃|�C���^</param>
/// <param name="data">�L�����N�^�[�̃f�[�^</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreateFellow(IComponent* parent, CommonResources* resources, Map* map, const GameParameters::CharacterData data, const int& stageNumber)
{
	std::unique_ptr<IComponent> fellow;
	//�C���X�^���X�𐶐�����
	fellow.reset(new Fellow(parent, resources, map, data, stageNumber));
	//����������
	fellow->Initialize();
	//�C���X�^���X��Ԃ�
	return std::move(fellow);
}

/// <summary>
/// �G�𐶐�����
/// </summary>
/// <param name="parent">�e�̃|�C���^</param>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="number">�G�̔ԍ�</param>
/// <param name="map">�}�b�v�̃|�C���^</param>
/// <param name="data">�L�����N�^�[�̃f�[�^</param>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <param name="json">json�f�[�^</param>
/// <returns></returns>
std::unique_ptr<IComponent> CharacterFactory::CreateEnemy(IComponent* parent, CommonResources* resources, const int& number, Map* map, const GameParameters::CharacterData data, const int& stageNumber, LoadJson* json)
{
	std::unique_ptr<IComponent> enemy;
	//�G�̃f�[�^��ݒ肷��
	GameParameters::EnemyData enemyData;
	enemyData.charaData = data;									  //�L�����N�^�\�f�[�^
	enemyData.enemyNumber = number;								  //�G�ԍ�
    enemyData.patrolRouteMap = json->GetPatrolRootMap();		  //���񃋁[�g�}�b�v
    enemyData.patrolAdjacencyList = json->GetAdjacencyList();	  //�אڃ��X�g
    enemyData.initalNumber = json->GetInitailVertex(number);	  //�������W�̒��_
	//�C���X�^���X�𐶐�����
	enemy.reset(new Enemy(parent, resources, map, enemyData, stageNumber));
	//����������
	enemy->Initialize();
	//�C���X�^���X��Ԃ�
	return std::move(enemy);
}
