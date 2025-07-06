/*
	@file	StageSelectScene.cpp
	@brief	�X�e�[�W�Z���N�g�V�[���N���X
*/
#include "pch.h"
#include "Game/Scene/StageSelectScene.h"
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
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
StageSelectScene::StageSelectScene()
	:
	m_commonResources{},
	m_soundManager{},
    m_stargeNumber(1),
	m_alpha(1.0f),
    m_sceneFade(false),
	m_currentState(SelectState::RIGHT),
    m_cursorPosition(CURSOR_POSITION),
    m_stargeNumberPosition(STARGE_NUMBER_POSITION),
    m_selectPosition(SELECT_POSITION),
    m_startPosition(START_POSITION),
    m_stargePosition(STARGE_POSITION),
    m_menuPosition(MENU_POSITION),
    m_isChangeScene(false)
{
	//�O���t�B�b�N�X�C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StageSelectScene::~StageSelectScene()
{
   Finalize();
   m_graphics->GetCustomModelRenderer()->ClearAllPointLights();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
void StageSelectScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	//�J�����̐���
	m_camera = std::make_unique<FixedCamera>(CAMERA_EYE_POSITION, CAMERA_TARGET_POSITION, DirectX::SimpleMath::Vector3::UnitY);

	//�J�X�^�������_�[�ɃJ������ݒ肷��
	m_graphics->GetCustomModelRenderer()->SetCamera(m_camera.get());

	//�V�[���O���t���[�g�𐶐�����(�L�����N�^�[�f�[�^��ǂݍ���)
	m_root = std::make_unique<Root>(nullptr, DirectX::SimpleMath::Vector3::Zero, 0.0f);

	//�I�u�W�F�N�g�f�[�^��ǂݍ���
	LoadObjects();

	//���\�[�X��ǂݍ���
	LoadResources();

	//�p�[�e�B�N���𐶐��A������
	m_particle = std::make_unique<Particle>();
	m_particle->Create(m_commonResources->GetDeviceResources());
	m_fade = std::make_unique<Fade>();
	m_fade->Create(m_commonResources->GetDeviceResources());
	m_fade->StartFadeOut();

	//�T�E���h�}�l�[�W���[�𐶐��A������
	m_soundManager = std::make_unique<SoundManager>();
	m_soundManager->Initialize(SoundManager::SoundSceneID::TITLE);

	//�v���~�e�B�u�o�b�`�𐶐�
	m_primitiveBatchColor = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_commonResources->GetDeviceResources()->GetD3DDeviceContext());
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void StageSelectScene::Update(float elapsedTime)
{
	//�T�E���h��炷
	m_soundManager->Update();
	//�V�[���O���t���[�g�̍X�V
	m_root->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, 0.0f, DirectX::SimpleMath::Vector3::Zero);

	//�e�N�X�`�����t�F�[�h������
	FadeTexture(elapsedTime);

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	if (kbTracker->pressed.Left || kbTracker->pressed.A)
	{
	   m_currentState = SelectState::LEFT;
	   m_stargeNumber--;
	}
	else if (kbTracker->pressed.Right || kbTracker->pressed.D)
	{
	   m_currentState = SelectState::RIGHT;
	   m_stargeNumber++;
	}
	m_stargeNumber = yamadalib::Math::Clamp(m_stargeNumber, GameParameters::MIN_STAGE_NUM, GameParameters::MAX_STAGE_NUM);
	
	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
	   //�ő�X�e�[�W�܂őI���\
	   if (m_stargeNumber > GameParameters::MAX_STAGE_NUM) return;
	     m_commonResources->GetBlackBoard()->SetStageNumber(m_stargeNumber);
		 m_soundManager->GetTitle()->SelectionSE();
		 if (m_soundManager->GetTitle()->IsSEPlaying())
		 {
			   m_sceneFade = true;
		 }
	}

	//�t�F�[�h�A�E�g���I�������V�[���؂�ւ��̃t���O��ON
	if (m_fade->GetFadeOut()) 
	{
	   m_isChangeScene = true;
	}
}

/// <summary>
/// �`�悷��
/// </summary>
void StageSelectScene::Render()
{

   //�r���[�s��E�v���W�F�N�V�����s����擾����
	DirectX::SimpleMath::Matrix view = m_camera->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = m_camera->GetProjectionMatrix();
	//�O���t�B�b�N�X�ɍs���ݒ肷��
	m_graphics->SetProjectiionMatrix(projection);
	m_graphics->SetViewMatrix(view);

	DirectX::SimpleMath::Matrix modlMat = DirectX::SimpleMath::Matrix::CreateRotationY(ROOM_ROTATION) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(ROOM_POSITION);

	//�I�u�W�F�N�g��`�悷��
	for (const auto& object : m_mapObjects)
	{
	   object->Render();
	}

	//���[�g��`�悷��
	m_root->Render();

	// ���f����`�悷��
	m_graphics->GetCustomModelRenderer()->Draw(*m_roomModel, m_roomdds.Get(), modlMat);

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
void StageSelectScene::Finalize()
{
    m_root.reset();
	m_loadJson.reset();
	m_fade.reset();
	m_camera.reset();
	m_graphics->Clear();
	
}

/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns></returns>
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

/// <summary>
/// ���\�[�X��ǂݍ���
/// </summary>
void StageSelectScene::LoadResources()
{
   //���f�������[�h����
   m_roomModel = yamadalib::Resources::GetInstance()->GetModel("Room");
   // �摜�����[�h����
   m_roomdds = yamadalib::Resources::GetInstance()->GetTexture("Room_DDS");
   m_menuTexture = yamadalib::Resources::GetInstance()->GetTexture("Menu");
   m_selectTexture = yamadalib::Resources::GetInstance()->GetTexture("StageSelect");
   m_stargeTexture = yamadalib::Resources::GetInstance()->GetTexture("Stage");
   m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("Timer");
}

/// <summary>
/// �t�F�[�h
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void StageSelectScene::FadeTexture(float elapsedTime)
{
	if (m_fadeIn) //�t�F�[�h�C��
	{
		m_alpha += FADE_SPEED * elapsedTime;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_fadeIn = false;
		}
	}
	else   //�t�F�[�h�A�E�g
	{
		m_alpha -= FADE_SPEED * elapsedTime;
		if (m_alpha <= 0.3f)
		{
			m_alpha = 0.3f;
			m_fadeIn = true;
		}
	}
}

/// <summary>
/// ���ݑI�����Ă����Ԃ�ύX����
/// </summary>
void StageSelectScene::ChangeSelectState()
{
	if (m_currentState == SelectState::LEFT)
	{
		m_currentState = SelectState::RIGHT;
		m_cursorPosition.y += CURSOR_MOVE_POSITION;
	}
	else
	{
		m_currentState = SelectState::LEFT;
		m_cursorPosition.y -= CURSOR_MOVE_POSITION;
	}
	m_alpha = 1.0f;
}

/// <summary>
/// �e�N�X�`����`�悷��
/// </summary>
void StageSelectScene::DrawTexture()
{
   DirectX::XMVECTORF32 modifiedColor = { 1.0f, 1.0f, 1.0f, m_alpha };
   m_graphics->DrawTexture(m_menuTexture, m_menuPosition, nullptr, MENU_COLOR);
   m_graphics->DrawTexture(m_selectTexture, m_selectPosition, &SELECT_RECT, DirectX::Colors::White);
   m_graphics->DrawTexture(m_selectTexture, m_startPosition, &START_RECT, modifiedColor);
   m_graphics->DrawTexture(m_selectTexture, LEFT_ARROW_POSITION, &LEFT_ARROW_RECT, modifiedColor);
   m_graphics->DrawTexture(m_selectTexture, RIGHT_ARROW_POSITION, &RIGHT_ARROW_RECT, modifiedColor);

   if (m_currentState == SelectState::LEFT)
   {
	  m_graphics->DrawTexture(m_selectTexture, LEFT_ARROW_POSITION, &LEFT_ARROW_RECT, modifiedColor);
	  m_graphics->DrawTexture(m_selectTexture, RIGHT_ARROW_POSITION, &RIGHT_ARROW_RECT);
   }
   else if (m_currentState == SelectState::RIGHT)
   {
	  m_graphics->DrawTexture(m_selectTexture, LEFT_ARROW_POSITION, &LEFT_ARROW_RECT);
	  m_graphics->DrawTexture(m_selectTexture, RIGHT_ARROW_POSITION, &RIGHT_ARROW_RECT, modifiedColor);
   }

   m_graphics->DrawTexture(m_stargeTexture, m_stargePosition, nullptr, DirectX::Colors::White);
   DrawStageNumber(m_stargeNumber, m_stargeNumberPosition);

}

/// <summary>
/// �X�e�[�W�ԍ��̕`��
/// </summary>
/// <param name="stageNumber">�X�e�[�W�ԍ�</param>
/// <param name="position">1�`����W</param>
void StageSelectScene::DrawStageNumber(int stageNumber, const DirectX::SimpleMath::Vector2& position)
{
   // ���݂̕`��ʒu��������
   DirectX::SimpleMath::Vector2 drawPosition = position;
   // �X�v���C�g�o�b�`�̊J�n
   m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_graphics->GetCommonStates()->NonPremultiplied());

   // ������`�悷�邽�߂̃����_�֐�
   auto drawDigit = [&](int digit)
   {
	  // RECT�̐ݒ�
	  RECT rect;
	  rect.top = 0;
	  rect.bottom = DIGIT_HEIGHT;  // �����̍���
	  rect.left = static_cast<LONG>(DIGIT_WIDth * digit);  // �����̍��[
	  rect.right = rect.left + DIGIT_WIDth;  // �����̉E�[

	  // �e�N�X�`���̕`��
	  m_graphics->GetSpriteBatch()->Draw(
		 m_timerTexture.Get(),         // �e�N�X�`��
		 drawPosition,                 // �`��ʒu
		 &rect,                        // �\�[�X��`
		 DirectX::Colors::White,       // �F
		 0.0f,                         // ��]�p�x (�K�v�Ȃ�w��\)
		 DirectX::SimpleMath::Vector2(0, 0), // ���_ (�f�t�H���g�͍���)
		 DRAW_SCALE                        // �X�P�[�� (�g��{��)
	  );

	  // ���̐����̕`��ʒu���E�Ɉړ�
	  drawPosition.x += DIGIT_WIDth + SPACING;  // �����̕��ɍ��킹�Ĉړ�
   };

   // �X�e�[�W�ԍ���1�����`��
   std::string stageStr = std::to_string(stageNumber);  // �X�e�[�W�ԍ��𕶎���ɕϊ�
   for (char ch : stageStr)
   {
	  int digit = ch - '0';  // �������琔���ɕϊ�
	  drawDigit(digit);      // �e������`��
   }
   // �X�v���C�g�o�b�`�̏I���
   m_graphics->GetSpriteBatch()->End();
}

/// <summary>
/// �I�u�W�F�N�g�f�[�^��ǂݍ���
/// </summary>
void StageSelectScene::LoadObjects()
{
   const std::string& filename = "Resources\\GameData\\StageSelectData.json";
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
