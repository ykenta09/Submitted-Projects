/*
	@file	StageSelectScene.h
	@brief	�X�e�[�W�Z���N�g�V�[���N���X
*/
#pragma once
#ifndef STAGE_SELECT_SCENE_DEFINED
#define STAGE_SELECT_SCENE_DEFINED
#include "Game/Interface/IScene.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Camera/FixedCamera.h"
#include "Game/Map/Prison/Prison.h"
#include "Game/Map/CelestialSphere/CelestialSphere.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Particle/Partcle.h"
#include "Game/Particle/Fade.h"

// �O���錾
class CommonResources;
class SoundManager;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class StageSelectScene final : public IScene
{
public:
	//�I�����Ă��镨
	enum SelectState
	{
		LEFT = 0,
		RIGHT,
		QUIT
	};

	enum Stage_Difficulty
	{
	   EASY,
	   NORMAL,
	   HARD
	};
public:
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_EYE_POSITION = { 0.0f, 1.5f, 15.0f };	   //�J�����̍��W
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_TARGET_POSITION = { 0.0f, 3.5f, -40.0f };  //�J�����̒����_
   static constexpr DirectX::SimpleMath::Vector3 ROOM_POSITION = { -1.0f, -3.0f, -10.0f };		   //�������f���̍��W
   static constexpr DirectX::SimpleMath::Vector2 SELECT_POSITION = { 200.0f, 30.0f };			   //�Z���N�g�摜�̍��W
   static constexpr DirectX::SimpleMath::Vector2 START_POSITION = { 470.0f, 550.0f };			   //�X�^�[�g�e�N�X�`���̍��W
   static constexpr DirectX::SimpleMath::Vector2 LEFT_ARROW_POSITION = { 30.0f, 300.0f };		   //���ړ��̃e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 RIGHT_ARROW_POSITION = { 1100.0f, 300.0f };	   //�E�ړ��̃e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 STARGE_POSITION = { 400.0f, 220.0f };			   //�X�e�[�W���e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 MENU_POSITION = { 0.0f, 0.0f };				   //���j���[�e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 CURSOR_POSITION = { 120.0f, 370.0f };			   //�J�[�\���e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 STARGE_NUMBER_POSITION = { 750.0f, 330.0f };	   //�X�e�[�W�ԍ��`����W

   static constexpr float CURSOR_MOVE_POSITION = 130.0f;							//�J�[�\���̈ړ���
   static constexpr float ROOM_ROTATION = DirectX::XMConvertToRadians(0.0f);		//�����̉�]�p
   static constexpr int DIGIT_WIDth = 57;					// �����̕�
   static constexpr int DIGIT_HEIGHT = 96;					// �����̍���
   static constexpr int SPACING = 5;						// �����̊Ԋu
   static constexpr float DRAW_SCALE = 1.5f;				//�X�e�[�W�ԍ��`��X�P�[��
   static constexpr float FADE_SPEED = { 0.5f };			//�t�F�[�h���x

   static constexpr RECT START_RECT = { 0, 180 , 900, 320 };									//�X�^�[�g�e�N�X�`�����W
   static constexpr RECT SELECT_RECT = { 0, 0 , 910, 180 };										//�Z���N�g�e�N�X�`�����W
   static constexpr RECT LEFT_ARROW_RECT = { 0, 320 , 145, 500 };								//���ړ��e�N�X�`�����W
   static constexpr RECT RIGHT_ARROW_RECT = { 150, 320 , 280, 500 };							//�E�ړ��e�N�X�`�����W
   static constexpr DirectX::XMVECTORF32 MENU_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };				//���j���[�e�N�X�`�����W

public:
   //�R���X�g���N�^
   StageSelectScene();
   //�f�X�g���N�^
    ~StageSelectScene() override;

	//������
    void Initialize(CommonResources* resources) override;
	//�X�V
    void Update(float elapsedTime)override;
	//�`��
    void Render() override;
	//��n��
    void Finalize() override;
	//���̃V�[��ID���擾����
    SceneID GetNextSceneID() const;

	//���\�[�X��ǂݍ���
	void LoadResources();
	//�e�N�X�`���t�F�[�h
	void FadeTexture(float elapsedTime);
	//���݂̑I�����Ă����Ԃ�ύX
	void ChangeSelectState();
	//�e�N�X�`����`�悷��
	void DrawTexture();
	//�X�e�[�W�i���o�[��`�悷��
	void DrawStageNumber(int stageNumber, const DirectX::SimpleMath::Vector2& position);
private:
   //�I�u�W�F�N�g��ǂݍ���
   void LoadObjects();
private:
	CommonResources* m_commonResources;				  // ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;				  //�O���t�B�b�N�X
	std::unique_ptr<SoundManager> m_soundManager;	  //�T�E���h
	std::unique_ptr<FixedCamera> m_camera;			  //�J����
	SelectState m_currentState;						  //���ݑI�����Ă�����
	std::unique_ptr<CelestialSphere> m_celestialSphere;	//�V��
	std::unique_ptr<IComposite> m_root;				  //�V�[���O���t���[�g

	//�I�u�W�F�N�g�z��
	std::vector<std::unique_ptr<IMapObject>> m_mapObjects;

	//�p�[�e�B�N��
	std::unique_ptr<Particle> m_particle;
	std::unique_ptr<Fade> m_fade;
	std::unique_ptr<LoadJson> m_loadJson;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_menuTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stargeTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;
	
	//���W
	DirectX::SimpleMath::Vector2 m_cursorPosition;
	DirectX::SimpleMath::Vector2 m_stargeNumberPosition;

	//2D���W
	DirectX::SimpleMath::Vector2 m_selectPosition;
	DirectX::SimpleMath::Vector2 m_startPosition;
	DirectX::SimpleMath::Vector2 m_stargePosition;
	DirectX::SimpleMath::Vector2 m_menuPosition;

	//���f��
	DirectX::Model* m_roomModel;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomdds;  //�e�N�X�`��

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	bool m_fadeIn;
	bool m_sceneFade;
	float m_alpha;

	//�X�e�[�W�ԍ�
	int m_stargeNumber;

	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatchColor;
};
#endif // STAGE_SELECT_SCENE_DEFINED
