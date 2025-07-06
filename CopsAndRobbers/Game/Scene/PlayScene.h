/*
	@file	PlayScene.h
	@brief	�v���C�V�[���N���X
*/
#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED

#include "Game/Interface/IScene.h"
#include "Game/Interface/IComponent.h"
#include "Game/Interface/IComposite.h"
#include "Game/Interface/INonObject.h"
#include "Game/Map/Goal/Goal.h"

#include "StepTimer.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/yamadaLib/LoadJson.h"
#include "Game/Particle/Fade.h"
#include "Game/Particle/CircularFade.h"
#include "Game/Particle/Partcle.h"
#include "Game/FrustumCuller.h"

#include "Game/Particle/Fragment.h"

// �O���錾
class CommonResources;
class SoundManager;
class Map;
class Enemy;
class GameManager;
class CameraManager;
class UIManager;
class Fellow;
class GraphScene;

namespace mylib
{
	class GridFloor;
	class TPSCamera;
}

class PlayScene final : public IScene
{
public:
   static constexpr float CAMERA_ROTATION_ANGLE = { DirectX::XMConvertToRadians(40.0f) };				//�J�����̉�]��

public:
    //�R���X�g���N�^
	PlayScene();
	//�f�X�g���N�^
	~PlayScene() override;

	//����������
	void Initialize(CommonResources* resources) override;
	//�X�V����
	void Update(float elapsedTime)override;
	//�`�悷��
	void Render() override;
	//��n��������
	void Finalize() override;

	//���̃V�[��ID���擾����
	SceneID GetNextSceneID() const;

	//�X�e�[�W�����[�h����
	void LoadStage(int stageNumber);

	//�S�[�������𖞂�������
	bool IsGoal();
	//�X�^�[�g����
	void StartEvent(float elapsedTime, Player* player);
	//�I������
	void EndProcessing(float elapsedTime);
private:
	CommonResources* m_commonResources;	  	// ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;	    //�O���t�B�b�N�X
	CollisionManager* m_collision;		    //�Փ˔���
	GameManager* m_gameManager;		  //�Q�[���}�l�[�W���[

	std::unique_ptr<SoundManager> m_soundManager;	  //�T�E���h
	std::unique_ptr<CameraManager> m_cameraManager;	  //�J�����}�l�[�W���[
	std::unique_ptr<UIManager> m_uiManager;			  //UI�}�l�[�W���[
	std::unique_ptr<LoadJson> m_loadJson;			  //json�t�@�C��

	DirectX::Keyboard::State m_keyboardState;						 //�L�[�{�[�h�X�e�[�g
	DirectX::Keyboard::KeyboardStateTracker m_keyboradStateTracker;	 //�L�[�{�[�h�X�e�[�g�g���b�J�[
	DirectX::Mouse::State m_mouseState;								 //�}�E�X�̃X�e�[�g

	DirectX::SimpleMath::Matrix m_view;			// �r���[�s��
	DirectX::SimpleMath::Matrix m_projection;	// �ˉe�s��

	std::unique_ptr<IComposite> m_root;	//�V�[���O���t���[�g
	std::vector<std::unique_ptr<INonObject>> m_nonObjects; 	   //��I�u�W�F�N�g�̃C���X�^���X�z��
	std::unique_ptr<Map> m_map;	//�}�b�v
	std::unique_ptr<Enemy> m_enemy;	//�G
	std::unique_ptr<Fellow> m_fellow;	//����
	std::unique_ptr<GraphScene> m_graphScene;	//�O���t�V�[��
	std::unique_ptr<Fade> m_fade;				//�p�[�e�B�N��
	std::unique_ptr<Fragment> m_fragment;		//�x���G�t�F�N�g

	bool m_isChangeScene;			// �V�[���`�F���W�t���O
	float m_cameraAngle;			//�X�^�[�g�C�x���g�p
	float m_endEventCameraAngle;
	float m_timer;					//�t���[�����J�E���g����
	int m_enemyMaxIndex;			//�X�e�[�W�̓G�̑���
	std::vector<Enemy*> m_enemies;	//�G�̔z��	 
};
#endif // PLAY_SCENE_DEFINED
