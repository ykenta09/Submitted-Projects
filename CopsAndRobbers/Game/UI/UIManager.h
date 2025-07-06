/*
	@file	UIManager.h
	@brief	UI�}�l�[�W���[�N���X
*/
#pragma once
#ifndef UI_MANAGER_DEFINED
#define UI_MANAGER_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include <unordered_map>
#include "Game/Interface/IUIGroup.h"
#include "Game/UI/TitleScene/TitleSceneUI.h"
#include "Game/UI/PlayScene/PlaySceneUI.h"
#include "Game/UI/ResultScene/ResultSceneUI.h"

// �O���錾
class CommonResources;
class Player;
class Fellow;
class Map;

class UIManager
{
public:
   enum SceneType
   {
	  TITLE,	  //�^�C�g��
	  PLAY,		  //�v���C
	  RESULT	  //���U���g
   };
public:
   IUIGroup* GetCurrentUIGroup() { return m_currentUIGroup; }


   TitleSceneUI* GetTitleSceneUI() const { return m_titleScene.get(); }
   PlaySceneUI* GetPlaySceneUI() const { return m_playScene.get(); }
   ResultSceneUI* GetResultSceneUI() const { return m_resultScene.get(); }
public:
   //�R���X�g���N�^
   UIManager(CommonResources* resoureces);
   UIManager(CommonResources* resoureces, Player* player, Fellow* fellow, Map* map);
   //�f�X�g���N�^
   ~UIManager();

   //����������
   void Initialize(SceneType scene);
   //�X�V����
   void Update(const float& elapsedTime, const float& playTime = 0);
   //�`�悷��
   void Render();
   //��n������
   void Finalize();

   //�X�e�[�W��؂�ւ���
   void ChangeScene(SceneType scene);

private:
   // ���ʃ��\�[�X
   CommonResources* m_commonResources;
   //�O���t�B�b�N�X
   yamadalib::Graphics* m_graphics;

   //�V�[��
   std::unique_ptr<TitleSceneUI> m_titleScene;
   std::unique_ptr<PlaySceneUI> m_playScene;
   std::unique_ptr<ResultSceneUI> m_resultScene;

   IUIGroup* m_currentUIGroup;
   Player* m_player;
   Fellow* m_fellow;
   Map* m_map;
};
#endif // UI_MANAGER_DEFINED
