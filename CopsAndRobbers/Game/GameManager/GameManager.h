/*
	@file	GameManager.h
	@brief	�Q�[���}�l�[�W���[�N���X
*/
#pragma once
#ifndef GAMEMANAGER_DEFINED
#define GAMEMANAGER_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Camera/FixedCamera.h"   
#include "Game/Interface/INonObject.h"

// �O���錾
class CommonResources;
class AStarGraph;
class Map;

class GameManager : public INonObject
{
public:
    static constexpr float DELAY_TIME = { 5.0f };		 //�Q�[���J�n���̃f�B���C�^�C��

	enum class GameState
	{
	   GameStart,
	   GamePlay,
	   GameEnd
	};
public:
   //Graphics�N���X�̃C���X�^���X���擾����
   static GameManager* const GetInstance(CommonResources* resources = nullptr, Map* map = nullptr);
public:
	float GetTimer() { return m_timer; }
	float GetDelayTimer() { return m_delayTimer; }
	//�S�[�����������ǂ���
	void SetIsGoal(bool flag) { m_isGoal = flag; }
	//�Q�[���̏�Ԃ��擾����
	GameState GetGameState() const { return m_gameState; }

	bool GetGameStart() const { return m_gameStart; }
	bool GetGamePlay() const { return m_gamePlay; }
	bool GetGameClear() const { return m_gameClear; }
	bool GetGameOver() const { return m_gameOver; }

	//���ʃ��\�[�X���擾����
	CommonResources* GetCommonResources() { return m_commonResources; }

	//�o�H�T���O���t���擾����
	AStarGraph* GetGraph() { return m_astarGraph.get(); }
	//�}�b�v���擾����
	Map* GetMap() { return m_map; }

	//���݂̂���X�e�[�W�ԍ����擾����
	int GetCurrentStageNumber() const { return m_currentStageNumber; }
	void SetCurrentStageNumber(const int& stageNumber) { m_currentStageNumber = stageNumber; }

public:
	GameManager(CommonResources* resources, Map* map);
	~GameManager();

	void Initialize() override;
	void Update(const float& elapsedTime) override;
	void Render() override;
	void Finalize() override;

	void GoalEvent();

	void OnKeyPressed(const DirectX::Keyboard::Keys& keys) override;
	//�C���X�^���X���폜����
	void DestroyInstance();

	//�Q�[���X�^�[�g�֐�
	void GamePlay();
	//�Q�[���I���֐�
	void GameClear();
	void GameOver();
	//�Q�[���̏�Ԃ����Z�b�g
	void Reset();
private:
    //GameManager�N���X�̃C���X�^���X�ւ̃|�C���^
    static std::unique_ptr<GameManager> m_gameManager;

    std::unique_ptr<AStarGraph> m_astarGraph;		  //AStar�O���t

	CommonResources* m_commonResources;	 // ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;	 //�O���t�B�b�N�X
	Map* m_map;

	DirectX::Keyboard::State m_keyboardState;						 //�L�[�{�[�h�X�e�[�g
	DirectX::Keyboard::KeyboardStateTracker m_keyboradStateTracker;	 //�L�[�{�[�h�X�e�[�g�g���b�J�[

	float m_delayTimer;	//�J�n�f�B���C����

	float m_timer;	//�v���C����

	bool m_isGoal;		//�S�[�������ǂ���(�폜�\��̕ϐ�)

	GameState m_gameState; //�Q�[���̏��
	bool m_gameStart;	   //�Q�[���X�^�[�g�t���O
	bool m_gamePlay;	   //�Q�[���v���C��
	bool m_gameClear;	   //�Q�[���N���A�t���O
	bool m_gameOver;	   //�Q�[���I�[�o�[�t���O

	int m_currentStageNumber;	 //���݂̂���X�e�[�W�̔ԍ�

};
#endif // GAMEMANAGER_DEFINED
