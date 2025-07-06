/*
	@file	TitleScene.cpp
	@brief	�^�C�g���V�[���N���X
*/
#include "pch.h"
#include "TitleScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include "Game/Sound/SoundManager.h"
#include "Game/Sound/TitleSceneSound.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Factory/CharacterFactory.h"
#include "Game/Node/Root.h"
#include "Libraries/yamadaLib/LoadJson.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleScene::TitleScene()
	:
	m_commonResources{},
	m_titleTexture{},
	m_isChangeScene{},
	m_soundManager{},
	m_alpha(1.0f),
    m_sceneFade(false),
	m_currentState(SelectState::PLAY),
    m_cursorPosition(CURSOR_INITIALIZE_POSITION)
{
	//�O���t�B�b�N�X�C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleScene::~TitleScene()
{
   Finalize();
   m_root.reset();
   m_graphics->GetCustomModelRenderer()->ClearAllPointLights();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
void TitleScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	//�J�����𐶐��A������
	m_camera = std::make_unique<FixedCamera>(CAMERA_EYE_POSITION, CAMERA_TARGET_POSITION, DirectX::SimpleMath::Vector3::UnitY);

	//�J�X�^�������_�[�ɃJ������ݒ肷��
	m_graphics->GetCustomModelRenderer()->SetCamera(m_camera.get());

	//�V���̐����A������
	m_celestialSphere = std::make_unique<CelestialSphere>(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Quaternion::Identity);
	m_celestialSphere->Initialize();

	m_root = std::make_unique<Root>(nullptr, DirectX::SimpleMath::Vector3::Zero, 0.0f);

	//�I�u�W�F�N�g�f�[�^��ǂݍ���
	LoadObjects();

	m_root->AsPlayer()->ChangeState(m_root->AsPlayer()->GetPlayerTitle());
	Root* root = dynamic_cast<Root*>(m_root.get());
	for (const auto& enemy : root->GetEnemies())
	{
	   enemy->ChangeState(enemy->GetEnemyTitle());
	}

	//���\�[�X��ǂݍ���
	LoadResources();

	//�p�[�e�B�N���𐶐��A������
	m_particle = std::make_unique<Particle>();
	m_particle->Create(m_commonResources->GetDeviceResources());
	m_fade = std::make_unique<Fade>();
	m_fade->Create(m_commonResources->GetDeviceResources());
	m_fade->StartFadeOut();

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	m_soundManager = std::make_unique<SoundManager>();
	m_soundManager->Initialize(SoundManager::SoundSceneID::TITLE);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void TitleScene::Update(float elapsedTime)
{
	//�T�E���h��炷
	m_soundManager->Update();
	//�V�����X�V����
	m_celestialSphere->Update(DirectX::SimpleMath::Vector3::Zero);
	//�V�[���O���t���[�g�̍X�V
	m_root->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, 0.0f, m_root->AsPlayer()->GetPosition());

	//�I�u�W�F�N�g�̍X�V
	for (const auto& object : m_mapObjects)
	{
	   object->Update(elapsedTime);
	}

	//�t�F�[�h�e�N�X�`��
	FadeTexture(elapsedTime);

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	if (kbTracker->pressed.Up || kbTracker->pressed.W)
	{
		ChangeSelectState();
	}
	else if (kbTracker->pressed.Down || kbTracker->pressed.S)
	{
		ChangeSelectState();
	}
	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		if (m_currentState == SelectState::PLAY) //�Z���N�g��ʂɈړ�
		{
			m_soundManager->GetTitle()->SelectionSE();
			if (m_soundManager->GetTitle()->IsSEPlaying())
			{
				m_sceneFade = true;
			}
		}
		else //�Q�[�����I��
		{
			PostQuitMessage(0);
		}
	}

	//�t�F�[�h�A�E�g���I�������V�[����؂�ւ���
	if (m_fade->GetFadeOut()) 
	{
	   m_isChangeScene = true;
	}
}


/// <summary>
/// �`�悷��
/// </summary>
void TitleScene::Render()
{
    //�r���[�s��ƃv���W�F�N�V�����s����擾����
	DirectX::SimpleMath::Matrix view = m_camera->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = m_camera->GetProjectionMatrix();
	//�O���t�B�b�N�X�ɍs���ݒ肷��
	m_graphics->SetProjectiionMatrix(projection);
	m_graphics->SetViewMatrix(view);

	//���f����`�悷��
	DrawModel();

	//�e�N�X�`����`�悷��
	DrawTexture();

	//�p�[�e�B�N���̕`��
	m_particle->Render();
	if (m_sceneFade)
	{
	   m_fade->Render();
	}
}

/// <summary>
/// ��n������
/// </summary>
void TitleScene::Finalize()
{
	// do nothing.
}

/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns></returns>
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
	   return IScene::SceneID::STAGE_SELECT;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

/// <summary>
/// �t�F�[�h�e�N�X�`��
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void TitleScene::FadeTexture(float elapsedTime)
{
	if (m_fadeIn)
	{
		m_alpha += FADE_SPEED * elapsedTime;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_fadeIn = false;
		}
	}
	else
	{
		m_alpha -= FADE_SPEED * elapsedTime;
		if (m_alpha <= 0.1f)
		{
			m_alpha = 0.3f;
			m_fadeIn = true;
		}
	}
}

/// <summary>
/// �I�u�W�F�N�g�f�[�^��ǂݍ���
/// </summary>
void TitleScene::LoadObjects()
{
   //�V�[���O���t���[�g�𐶐�����(�L�����N�^�[�f�[�^���쐬)
   const std::string& filename = "Resources\\GameData\\TitleSceneData.json";
   m_loadJson = std::make_unique<LoadJson>(m_commonResources, nullptr, nullptr);
   m_loadJson->LoadCharacterData(filename);
   m_loadJson->TransferCharactersTo([this](std::unique_ptr<IComponent> character)
	  {
		 m_root->Attach(std::move(character));
	  });


   //�I�u�W�F�N�g�̃f�[�^��ǂݍ���
   m_loadJson->LoadObject(filename);
   m_mapObjects = m_loadJson->GetMapObjects();

}

/// <summary>
/// ���ݑI�����Ă����Ԃ�ύX����
/// </summary>
void TitleScene::ChangeSelectState()
{
	if (m_currentState == SelectState::PLAY)
	{
		m_currentState = SelectState::QUIT;
		m_cursorPosition.y += CURSOR_MOVE_POSITION;
	}
	else
	{
		m_currentState = SelectState::PLAY;
		m_cursorPosition.y -= CURSOR_MOVE_POSITION;
	}
	m_alpha = 1.0f;
}

/// <summary>
/// ���\�[�X��ǂݍ���
/// </summary>
void TitleScene::LoadResources()
{
   // �摜�����[�h����
   m_titleTexture = yamadalib::Resources::GetInstance()->GetTexture("Title");
   m_textTexture = yamadalib::Resources::GetInstance()->GetTexture("Text");
   m_menuTexture = yamadalib::Resources::GetInstance()->GetTexture("Menu");
   m_cursorTexture = yamadalib::Resources::GetInstance()->GetTexture("Cursor");
}

/// <summary>
/// �e�N�X�`����`�悷��
/// </summary>
void TitleScene::DrawTexture()
{
   //���j���[�̃J���[
   DirectX::XMVECTORF32 modifiedColor = { 1.0f, 1.0f, 1.0f, m_alpha };
   //�e�N�X�`����`�悷��
   m_graphics->DrawTexture(m_menuTexture, MENU_TEXTURE_POSITION, nullptr, MENU_COLOR);
   m_graphics->DrawTexture(m_titleTexture, TITLE_TEXTURE_POSITION, nullptr);
   m_graphics->DrawTexture(m_cursorTexture, m_cursorPosition, nullptr, CURSOR_COLOR);
   //�I�����Ă����Ԃŕ`���ύX����
   if (m_currentState == SelectState::PLAY)
   {
	  m_graphics->DrawTexture(m_textTexture, START_TEXTURE_POSITION, &START_RECT, modifiedColor);
	  m_graphics->DrawTexture(m_textTexture, END_TEXTURE_POSITION, &END_RECT);
   }
   else if(m_currentState == SelectState::QUIT)
   {
	  m_graphics->DrawTexture(m_textTexture, START_TEXTURE_POSITION, &START_RECT);
	  m_graphics->DrawTexture(m_textTexture, END_TEXTURE_POSITION, &END_RECT, modifiedColor);
   }
}

/// <summary>
/// ���f����`�悷��
/// </summary>
void TitleScene::DrawModel()
{
   //�V����`�悷��
   m_celestialSphere->Render();

   //���[�g�̕`��
   m_root->Render();

   // ���f����`�悷��
   for (const auto& object : m_mapObjects)
   {
	  object->Render();
   }
}
