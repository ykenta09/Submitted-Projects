/*
	@file	ManagerFactory.cpp
	@brief	�}�l�[�W���[�̃t�@�N�g���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "Game/Factory/ManagerFactory.h"
#include "Game/Observer/Messenger.h"
#include "Game/Map/Map.h"


/// <summary>
/// �Q�[���}�l�[�W���[�𐶐�
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
/// <returns></returns>
std::unique_ptr<INonObject> ManagerFactory::CreateGameManager(CommonResources* resources, Map* map)
{
   //�Q�[���}�l�[�W���[��錾����
   std::unique_ptr<INonObject> gameManager;
   //�C���X�^���X�𐶐�����
   gameManager.reset(new GameManager(resources, map));
   //����������
   gameManager->Initialize();
   //Space�L�[��o�^����
   Messenger::Attach(DirectX::Keyboard::Keyboard::Space, gameManager.get());

   //�C���X�^���X��Ԃ�
   return std::move(gameManager);
}
