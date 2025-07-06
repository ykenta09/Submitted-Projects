/*
	@file	PlayScene.cpp
	@brief	�v���C�V�[���N���X
*/
#include "pch.h"
#include "PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"

#include "Libraries/yamadaLib/Resources.h"

#include "Game/Character/Player/Player.h"
#include "Game/Map/Map.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Sound/SoundManager.h"
#include "Game/GameManager/GameManager.h"
#include "Game/Camera/CameraManager.h"
#include "Game/UI/PlayScene/PlaySceneUI.h"
#include "Game/UI/UIManager.h"
#include "Game/Observer/Messenger.h"

#include "Game/Factory/CharacterFactory.h"
#include "Game/Factory/ManagerFactory.h"
#include "Game/Collision/CollisionObject.h"
#include "Game/Node/Root.h"
#include "Game/Character/Fellow/Fellow.h"
#include "GraphEditor/GraphScene.h"
#include <cassert>
#include "Game/Screen.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/yamadaLib/Math.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayScene::PlayScene()
	:
	m_commonResources{},
	m_view{},
	m_projection{},
	m_map{},
	m_enemy{},
	m_root{},
	m_graphScene{},
	m_isChangeScene(false),
    m_cameraAngle(0.0f),
    m_timer(0.0f)
{
	//�O���t�B�b�N�X�C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayScene::~PlayScene()
{
	Finalize();
	m_root.reset();
	m_graphics->Clear();
	//�C���X�^���X���폜����
	GameManager::GetInstance()->DestroyInstance();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
void PlayScene::Initialize(CommonResources* resources)
{
   assert(resources);
   m_commonResources = resources;

   //�ώ@�҃��X�g�����Z�b�g����
   Messenger::ClearObserverList();
   CollisionManager::GetInstance().ClearObjects();

   //�J�������쐬����
   m_cameraManager = std::make_unique<CameraManager>();
   m_graphics->GetCustomModelRenderer()->SetCamera(m_cameraManager->GetCurrentCamera());

   //�}�b�v�𐶐��A������
   m_map = std::make_unique<Map>(m_commonResources);
   m_map->Initialize();

   //�Q�[���}�l�[�W���[�𐶐��A����������
   m_gameManager = GameManager::GetInstance(m_commonResources, m_map.get());
   m_gameManager->Initialize();

   //�V�[���O���t���[�g�𐶐�����
   m_root = std::make_unique<Root>(nullptr, DirectX::SimpleMath::Vector3::Zero, 0.0f);
   //���[�g�ǉ�����
   m_loadJson = std::make_unique<LoadJson>(m_commonResources, m_root.get(), m_map.get());
   //json�t�@�C������X�e�[�W�f�[�^�����[�h����
   for (int stageNumber = GameParameters::MIN_STAGE_NUM; stageNumber <= GameParameters::MAX_STAGE_NUM; stageNumber++)
   {
	  m_loadJson->LoadGraphData(stageNumber);
	  m_loadJson->LoadCharacterData(stageNumber);
	  m_loadJson->TransferCharactersTo([this](std::unique_ptr<IComponent> character)
		 {
			m_root->Attach(std::move(character));
		 });
   }

   //�T�E���h�𐶐�����
   m_soundManager = std::make_unique<SoundManager>();
   m_soundManager->Initialize(SoundManager::SoundSceneID::PLAY);

   //UI�𐶐�����
   m_uiManager = std::make_unique<UIManager>(m_commonResources, m_root->AsPlayer(), m_root->AsFellow(), m_map.get());
   m_uiManager->Initialize(UIManager::SceneType::PLAY);

   //�p�[�e�B�N���𐶐��A������(�R�[�h�C���ӏ�)
   m_fade = std::make_unique<Fade>();
   m_fade->Create(m_commonResources->GetDeviceResources());
   m_fade->StartFadeIn();

   //�x���G�t�F�N�g�ǉ�
   m_fragment = std::make_unique<Fragment>();
   m_fragment->Create(m_commonResources->GetDeviceResources());

   //�ώ@�҃��X�g���\�[�g����
   Messenger::SortObserverList();
   //�L�[�͈̔̓��X�g�𐶐�����
   Messenger::CreateKeyRangeList();
   //�X�e�[�W�f�[�^�����[�h����
   LoadStage(m_commonResources->GetBlackBoard()->GetStageNumber());

   m_cameraAngle = m_root->AsPlayer()->GetInitialAngle();

   m_endEventCameraAngle = m_cameraAngle - DirectX::XMConvertToRadians(180.0f);
#ifdef _DEBUG
   //�O���t�V�[���𐶐�����
   m_graphScene = std::make_unique<GraphScene>(m_commonResources);
   m_graphScene->Initialize();
#endif // _DEBUG
}

/// <summary>
/// �L�[�{�[�h�������ꂽ���ǂ����𔻒肷��
/// </summary>
/// <param name="state">�L�[�{�[�h�̏��</param>
/// <returns></returns>
inline bool IsKeyPress(DirectX::Keyboard::State& state)
{
	//�L�[�{�[�h�X�e�[�g�̃|�C���^���擾����
	auto ptr = reinterpret_cast<uint32_t*>(&state);
	for (int key = 0; key < 0xff; key++)
	{
		const unsigned int buffer = 1u << (key & 0x1f);
		//�L�[�������ꂽ���ǂ������ׂ�
		if (ptr[(key >> 5)] && buffer) return true;
	}
	//�L�[�͉�����Ă��Ȃ�
	return false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayScene::Update(float elapsedTime)
{
    //�o�ߎ��Ԃ����Z����
	m_timer += elapsedTime;

	//�L�[�{�[�h�̏�Ԃ��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	//�L�[�{�[�h�X�e�[�g�g���b�J�[���X�V����
	m_keyboradStateTracker.Update(m_keyboardState);

	Player* player = m_root->AsPlayer();

	//�X�^�[�g�C�x���g
	StartEvent(elapsedTime, player);
	//�I������
	EndProcessing(elapsedTime);

	//UI���X�V����
	m_uiManager->Update(elapsedTime, m_gameManager->GetTimer());

	//�Q�[���}�l�[�W���[���X�V����
	if (!m_root->AsPlayer()->GetHit())
	{
	   m_gameManager->Update(elapsedTime);
	}

	//�x���Ƀv���C���[�ƓG�̍��W�𑗂�(�_�C�i�~�b�N�L���X�g���g���Ă���̂ŏC���ӏ�)
	Root* root = dynamic_cast<Root*>(m_root.get());
	for (const auto& enemy : root->GetEnemies())
	{
	   m_fragment->SetPosition(player->GetPosition(), enemy->GetPosition());
	}
	
	//�J�����̍X�V
	if (m_gameManager->GetGamePlay())
	{
	   m_cameraManager->Update(DirectX::SimpleMath::Vector3(player->GetPosition().x, 1.0f, player->GetPosition().z), player->GetCameraMatrix(), m_map->GetBoundingBoxes());
	}

#ifdef _DEBUG
	//�ʏ�v���C���́ATPS�J�����ɐݒ�
	if (!m_graphScene->GetEditMode() && m_gameManager->GetGamePlay())
	{
	   m_cameraManager->SetTPSCamera();
	}
	//�O���t�V�[���̍X�V
	m_graphScene->Update(elapsedTime);
	//�G�o�H�쐬�p�J�����ɕύX
	if (m_graphScene->GetEditMode())
	{
	   m_cameraManager->SetTopDownViewCamera();
	}
	if (m_graphScene->GetEditMode()) return;		//�ҏW���[�h���͉����������Ȃ�
#endif // _DEBUG

	//�}�b�v���X�V����
	m_map->SetCelestialSpherePosition(player->GetPosition());
	m_map->Update(elapsedTime);

    //�V�[���O���t���[�g�̍X�V
    if (!player->GetHit())
    {
	   m_root->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, 0.0f, player->GetPosition());
    }

	//�T�E���h��炷
	m_soundManager->Update();

	// �L�[�{�[�h�����������ꍇ�Ƀ��b�Z���W���[�ɃL�[�{�[�h�X�e�[�g�ƃv���C���[�m�[�h�ԍ���ʒm����
	if(m_gameManager->GetGamePlay()) 
	{
	   if (IsKeyPress(m_keyboardState))
	   {
		  Messenger::Notify(m_keyboardState);
	   }
	}

	//�����蔻��
	CollisionManager::GetInstance().Update();
}

/// <summary>
/// �`�悷��
/// </summary>
void PlayScene::Render()
{
	//�J��������r���[�s��Ǝˉe�s����擾����m
    DirectX::SimpleMath::Matrix view = m_cameraManager->GetViewMatrix();
    DirectX::SimpleMath::Matrix projection = m_cameraManager->GetProjectionMatrix();

	//�O���t�B�b�N�X�ɍs���ݒ肷��
	m_graphics->SetProjectiionMatrix(projection);
	m_graphics->SetViewMatrix(view);

	//�I�u�W�F�N�g�̕`�揈��
	m_map->Render(view, projection);
	m_root->Render();

#ifdef _DEBUG
	//�O���t�V�[���̕`��
	m_graphScene->Render();
#endif // _DEBUG

	//�x���̕`��
	m_fragment->Render();

	//ui�}�l�[�W���[
	m_uiManager->Render();

	if (m_root->AsPlayer()->GetHit())
	{
	   m_gameManager->GameOver();
	}

	//�p�[�e�B�N���`��s
	m_fade->Render();

#ifdef _DEBUG
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("DrawObject, %d", m_map->GetDrawObject());
	debugString->AddString("currentStage, %d", m_commonResources->GetBlackBoard()->GetStageNumber());

#endif // _DEBUG
}

/// <summary>
///  ��n������
/// </summary>
void PlayScene::Finalize()
{
   m_graphics->GetCustomModelRenderer()->ClearAllPointLights();
}

/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns></returns>
IScene::SceneID PlayScene::GetNextSceneID() const
{
	if (m_fade->GetFadeOut())
	{
	   return IScene::SceneID::RESULT;
	}
	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}

/// <summary>
/// �X�e�[�W�f�[�^�����[�h����
/// </summary>
/// <param name="stageNumber">���[�h����X�e�[�W�ԍ�</param>
void PlayScene::LoadStage(int stageNumber)
{
   //�ړ���̃X�e�[�W�����[�h����
   m_commonResources->GetBlackBoard()->SetStageNumber(stageNumber);
   CollisionManager::GetInstance().ChangeStageObjects(stageNumber);
   m_map->ChangeStageNumber(stageNumber);
}

/// <summary>
/// �S�[�������𖞂����Ă��邩
/// </summary>
/// <returns></returns>
bool PlayScene::IsGoal()
{
	auto goals = m_map->GetObjectsByType<Goal>(m_commonResources->GetBlackBoard()->GetStageNumber());
	for (const auto& goal : goals)
	{
	   if (goal->IsGaol())
	   {
		  m_gameManager->GameClear();
		  return true;
	   }
	}	
	return false;
}

/// <summary>
/// �Q�[���J�n���̉��o
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="player">�v���C���[</param>
void PlayScene::StartEvent(float elapsedTime, Player* player)
{
   //�Q�[�����n�܂����珈�����Ȃ�
   if (!m_gameManager->GetGameStart()) return;

   //�C�x���g�I�������𖞂����܂��̓X�y�[�X�ŃX�L�b�v�����牉�o���I������
   if (m_cameraAngle <= m_endEventCameraAngle || m_keyboradStateTracker.IsKeyPressed(DirectX::Keyboard::Space))
   {
	  m_cameraAngle = m_endEventCameraAngle;
	  player->SetStartEvent(false);
	  m_gameManager->GamePlay();
	  player->SetCameraPitch(m_cameraAngle);
   }

   //�t�F�[�h���I������牉�o���J�n����
   if (m_fade->GetFadeIn())
   {
	  m_cameraAngle -= CAMERA_ROTATION_ANGLE * elapsedTime;
   }
   DirectX::SimpleMath::Quaternion yawQuat = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, m_cameraAngle);
   //�J�����̍X�V
   m_cameraManager->Update(DirectX::SimpleMath::Vector3(player->GetPosition().x, 1.0f, player->GetPosition().z), DirectX::SimpleMath::Matrix::CreateFromQuaternion(yawQuat), m_map->GetBoundingBoxes());
}

/// <summary>
/// �I������
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void PlayScene::EndProcessing(float elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
   Player* player = m_root->AsPlayer();
   Root* root = dynamic_cast<Root*>(m_root.get());

   //�S�[�������𖞂����Ă��Ȃ��ꍇ�������s��Ȃ�
   if (!IsGoal())
   {
	  if (m_uiManager->GetPlaySceneUI()->GetMissionMessageUI()->GameFinishDraw())
	  {
		 m_fade->StartFadeOut();
	  }
	  return;
   }
   else
   {
	  if (m_uiManager->GetPlaySceneUI()->GetMissionMessageUI()->GameFinishDraw() && (player->GetEndGaolEvent()))
	  {
		 m_fade->StartFadeOut();
	  }
   }

   //�S�[���t���O��S�[���̏������s��
   player->SetGoalEvent(true);
   m_cameraManager->Update(DirectX::SimpleMath::Vector3(0.0f, 5.0f, 50.0f));
   m_cameraManager->SetFixedCamera();

   for (const auto& fellow : root->GetFellows())
   {
	  if (fellow->GetFellowMove())
	  {
		 fellow->GetFellowChasing()->SetGoalFlag(true);
	  }
   }
}
