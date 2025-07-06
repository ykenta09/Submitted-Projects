/*
	@file	TitleScene.h
	@brief	�^�C�g���V�[���N���X
*/
#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED
#include "Game/Interface/IScene.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Camera/FixedCamera.h"
#include "Game/Map/Prison/Prison.h"
#include "Game/Map/CelestialSphere/CelestialSphere.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Game/Particle/Partcle.h"
#include "Game/Particle/Fade.h"
#include "Libraries/yamadaLib/GameParameter.h"

// �O���錾
class CommonResources;
class SoundManager;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}


class TitleScene final : public IScene
{
public:
	//�I�����Ă��镨
	enum SelectState
	{
		PLAY = 0,
		QUIT
	};
public:
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_EYE_POSITION = { 0.0f, 2.0f, 15.0f };		  //�J�����̍��W
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_TARGET_POSITION = { 0.0f, 3.5f, -40.0f };	  //�J�����̒����_
   static constexpr DirectX::SimpleMath::Vector3 PRISON_POSITION = { -0.1f, -0.5f, 8.5f };			  //�S�����W
   static constexpr DirectX::SimpleMath::Vector3 FELLOW_POSITION = { -1.5f, 0.3f, 8.7f };			  //���ԍ��W
   static constexpr DirectX::SimpleMath::Vector3 PLAYER_POSITION = { -8.5f, 0.0f, 10.5f };			  //�v���C���[���W
   static constexpr DirectX::SimpleMath::Vector3 ENEMY_POSITION = { -11.0f, 0.0f, 10.5f };			  //�G���W

   static constexpr DirectX::SimpleMath::Vector2 TITLE_TEXTURE_POSITION = { 250.0f, -50.0f };		  //�^�C�g���e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 START_TEXTURE_POSITION = { 450.0f, 350.0f };		  //�X�^�[�g�e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 END_TEXTURE_POSITION = { 450.0f, 500.0f };			  //�G���h�e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 MENU_TEXTURE_POSITION = { 0.0f, 0.0f };			  //���j���[�e�N�X�`�����W
   static constexpr DirectX::SimpleMath::Vector2 CURSOR_INITIALIZE_POSITION = { 0.0f, 360.0f };		  //�J�[�\���������W
   static constexpr RECT START_RECT = { 0, 0, 810, 150 };											  //�X�^�[�g�e�N�X�`�����W
   static constexpr RECT END_RECT = { 0, 150, 810, 250 };											  //�G���h�e�N�X�`�����W
   static constexpr DirectX::XMVECTORF32 MENU_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };					  //���j���[�J���[
   static constexpr DirectX::XMVECTORF32 CURSOR_COLOR = { 0.3f, 0.3f, 0.3f, 0.3f };					  //�J�[�\���J���[
   static constexpr float FADE_SPEED = { 0.5f };													  //�t�F�[�h���x

   static constexpr float CURSOR_MOVE_POSITION = 130.0f;							//�J�[�\���̈ړ���
   const GameParameters::ObjectParameter TITLE_PRISON_PARAMETER =					//�S���̃p�����[�^
   {
	   PRISON_POSITION,										//�������W
	   DirectX::SimpleMath::Quaternion::Identity,			//������]
	   DirectX::SimpleMath::Vector3::One,					//�X�P�[��
	   {0, DirectX::SimpleMath::Vector3::Zero}				//�X�e�[�W�p�����[�^
   };
public:
    //�R���X�g���N�^
    TitleScene();
	//�f�X�g���N�^
    ~TitleScene() override;

	//������
    void Initialize(CommonResources* resources) override;
	//�X�V����
    void Update(float elapsedTime)override;
	//�`�悷��
    void Render() override;
	//��n������
    void Finalize() override;

	//���̃V�[��ID���擾����
    SceneID GetNextSceneID() const;
private:
   //���\�[�X��ǂݍ���
   void LoadResources();
   //�e�N�X�`����`�悷��
   void DrawTexture();
   //���f����`�悷��
   void DrawModel();
   //���݂̑I�����Ă����Ԃ�ύX
   void ChangeSelectState();
   //�t�F�[�h�e�N�X�`��
   void FadeTexture(float elapsedTime);
   //�f�[�^����I�u�W�F�N�g�𐶐�����
   void LoadObjects();
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
	//�T�E���h
	std::unique_ptr<SoundManager> m_soundManager;
	//�J����
	std::unique_ptr<FixedCamera> m_camera;
	//���ݑI�����Ă�����
	SelectState m_currentState;
	//�S��
	std::unique_ptr<Prison> m_prison;
	//�V��
	std::unique_ptr<CelestialSphere> m_celestialSphere;
	std::unique_ptr<LoadJson> m_loadJson;
	//����
	std::unique_ptr<Fellow> m_fellow;
	//�V�[���O���t���[�g
	std::unique_ptr<IComposite> m_root;
	//�I�u�W�F�N�g�z��
	std::vector<std::unique_ptr<IMapObject>> m_mapObjects;

	//�p�[�e�B�N��
	std::unique_ptr<Particle> m_particle;
	std::unique_ptr<Fade> m_fade;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_menuTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cursorTexture;
	//���W
	DirectX::SimpleMath::Vector2 m_cursorPosition;

	//���f��
	DirectX::Model* m_titleModel;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;
	bool m_fadeIn;
	bool m_sceneFade;
	float m_alpha;

};
#endif // TITLE_SCENE_DEFINED
