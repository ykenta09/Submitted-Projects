/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
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
/// コンストラクタ
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
	//グラフィックスインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
PlayScene::~PlayScene()
{
	Finalize();
	m_root.reset();
	m_graphics->Clear();
	//インスタンスを削除する
	GameManager::GetInstance()->DestroyInstance();
}

/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
void PlayScene::Initialize(CommonResources* resources)
{
   assert(resources);
   m_commonResources = resources;

   //観察者リストをリセットする
   Messenger::ClearObserverList();
   CollisionManager::GetInstance().ClearObjects();

   //カメラを作成する
   m_cameraManager = std::make_unique<CameraManager>();
   m_graphics->GetCustomModelRenderer()->SetCamera(m_cameraManager->GetCurrentCamera());

   //マップを生成、初期化
   m_map = std::make_unique<Map>(m_commonResources);
   m_map->Initialize();

   //ゲームマネージャーを生成、初期化する
   m_gameManager = GameManager::GetInstance(m_commonResources, m_map.get());
   m_gameManager->Initialize();

   //シーングラフルートを生成する
   m_root = std::make_unique<Root>(nullptr, DirectX::SimpleMath::Vector3::Zero, 0.0f);
   //ルート追加する
   m_loadJson = std::make_unique<LoadJson>(m_commonResources, m_root.get(), m_map.get());
   //jsonファイルからステージデータをロードする
   for (int stageNumber = GameParameters::MIN_STAGE_NUM; stageNumber <= GameParameters::MAX_STAGE_NUM; stageNumber++)
   {
	  m_loadJson->LoadGraphData(stageNumber);
	  m_loadJson->LoadCharacterData(stageNumber);
	  m_loadJson->TransferCharactersTo([this](std::unique_ptr<IComponent> character)
		 {
			m_root->Attach(std::move(character));
		 });
   }

   //サウンドを生成する
   m_soundManager = std::make_unique<SoundManager>();
   m_soundManager->Initialize(SoundManager::SoundSceneID::PLAY);

   //UIを生成する
   m_uiManager = std::make_unique<UIManager>(m_commonResources, m_root->AsPlayer(), m_root->AsFellow(), m_map.get());
   m_uiManager->Initialize(UIManager::SceneType::PLAY);

   //パーティクルを生成、初期化(コード修正箇所)
   m_fade = std::make_unique<Fade>();
   m_fade->Create(m_commonResources->GetDeviceResources());
   m_fade->StartFadeIn();

   //警告エフェクト追加
   m_fragment = std::make_unique<Fragment>();
   m_fragment->Create(m_commonResources->GetDeviceResources());

   //観察者リストをソートする
   Messenger::SortObserverList();
   //キーの範囲リストを生成する
   Messenger::CreateKeyRangeList();
   //ステージデータをロードする
   LoadStage(m_commonResources->GetBlackBoard()->GetStageNumber());

   m_cameraAngle = m_root->AsPlayer()->GetInitialAngle();

   m_endEventCameraAngle = m_cameraAngle - DirectX::XMConvertToRadians(180.0f);
#ifdef _DEBUG
   //グラフシーンを生成する
   m_graphScene = std::make_unique<GraphScene>(m_commonResources);
   m_graphScene->Initialize();
#endif // _DEBUG
}

/// <summary>
/// キーボードが押されたかどうかを判定する
/// </summary>
/// <param name="state">キーボードの状態</param>
/// <returns></returns>
inline bool IsKeyPress(DirectX::Keyboard::State& state)
{
	//キーボードステートのポインタを取得する
	auto ptr = reinterpret_cast<uint32_t*>(&state);
	for (int key = 0; key < 0xff; key++)
	{
		const unsigned int buffer = 1u << (key & 0x1f);
		//キーが押されたかどうか調べる
		if (ptr[(key >> 5)] && buffer) return true;
	}
	//キーは押されていない
	return false;
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayScene::Update(float elapsedTime)
{
    //経過時間を加算する
	m_timer += elapsedTime;

	//キーボードの状態を取得する
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	//キーボードステートトラッカーを更新する
	m_keyboradStateTracker.Update(m_keyboardState);

	Player* player = m_root->AsPlayer();

	//スタートイベント
	StartEvent(elapsedTime, player);
	//終了処理
	EndProcessing(elapsedTime);

	//UIを更新する
	m_uiManager->Update(elapsedTime, m_gameManager->GetTimer());

	//ゲームマネージャーを更新する
	if (!m_root->AsPlayer()->GetHit())
	{
	   m_gameManager->Update(elapsedTime);
	}

	//警告にプレイヤーと敵の座標を送る(ダイナミックキャストを使っているので修正箇所)
	Root* root = dynamic_cast<Root*>(m_root.get());
	for (const auto& enemy : root->GetEnemies())
	{
	   m_fragment->SetPosition(player->GetPosition(), enemy->GetPosition());
	}
	
	//カメラの更新
	if (m_gameManager->GetGamePlay())
	{
	   m_cameraManager->Update(DirectX::SimpleMath::Vector3(player->GetPosition().x, 1.0f, player->GetPosition().z), player->GetCameraMatrix(), m_map->GetBoundingBoxes());
	}

#ifdef _DEBUG
	//通常プレイ中は、TPSカメラに設定
	if (!m_graphScene->GetEditMode() && m_gameManager->GetGamePlay())
	{
	   m_cameraManager->SetTPSCamera();
	}
	//グラフシーンの更新
	m_graphScene->Update(elapsedTime);
	//敵経路作成用カメラに変更
	if (m_graphScene->GetEditMode())
	{
	   m_cameraManager->SetTopDownViewCamera();
	}
	if (m_graphScene->GetEditMode()) return;		//編集モード中は下を処理しない
#endif // _DEBUG

	//マップを更新する
	m_map->SetCelestialSpherePosition(player->GetPosition());
	m_map->Update(elapsedTime);

    //シーングラフルートの更新
    if (!player->GetHit())
    {
	   m_root->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, 0.0f, player->GetPosition());
    }

	//サウンドを鳴らす
	m_soundManager->Update();

	// キーボードを押下げた場合にメッセンジャーにキーボードステートとプレイヤーノード番号を通知する
	if(m_gameManager->GetGamePlay()) 
	{
	   if (IsKeyPress(m_keyboardState))
	   {
		  Messenger::Notify(m_keyboardState);
	   }
	}

	//当たり判定
	CollisionManager::GetInstance().Update();
}

/// <summary>
/// 描画する
/// </summary>
void PlayScene::Render()
{
	//カメラからビュー行列と射影行列を取得するm
    DirectX::SimpleMath::Matrix view = m_cameraManager->GetViewMatrix();
    DirectX::SimpleMath::Matrix projection = m_cameraManager->GetProjectionMatrix();

	//グラフィックスに行列を設定する
	m_graphics->SetProjectiionMatrix(projection);
	m_graphics->SetViewMatrix(view);

	//オブジェクトの描画処理
	m_map->Render(view, projection);
	m_root->Render();

#ifdef _DEBUG
	//グラフシーンの描画
	m_graphScene->Render();
#endif // _DEBUG

	//警告の描画
	m_fragment->Render();

	//uiマネージャー
	m_uiManager->Render();

	if (m_root->AsPlayer()->GetHit())
	{
	   m_gameManager->GameOver();
	}

	//パーティクル描画s
	m_fade->Render();

#ifdef _DEBUG
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("DrawObject, %d", m_map->GetDrawObject());
	debugString->AddString("currentStage, %d", m_commonResources->GetBlackBoard()->GetStageNumber());

#endif // _DEBUG
}

/// <summary>
///  後始末する
/// </summary>
void PlayScene::Finalize()
{
   m_graphics->GetCustomModelRenderer()->ClearAllPointLights();
}

/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns></returns>
IScene::SceneID PlayScene::GetNextSceneID() const
{
	if (m_fade->GetFadeOut())
	{
	   return IScene::SceneID::RESULT;
	}
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

/// <summary>
/// ステージデータをロードする
/// </summary>
/// <param name="stageNumber">ロードするステージ番号</param>
void PlayScene::LoadStage(int stageNumber)
{
   //移動先のステージをロードする
   m_commonResources->GetBlackBoard()->SetStageNumber(stageNumber);
   CollisionManager::GetInstance().ChangeStageObjects(stageNumber);
   m_map->ChangeStageNumber(stageNumber);
}

/// <summary>
/// ゴール条件を満たしているか
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
/// ゲーム開始時の演出
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="player">プレイヤー</param>
void PlayScene::StartEvent(float elapsedTime, Player* player)
{
   //ゲームが始まったら処理しない
   if (!m_gameManager->GetGameStart()) return;

   //イベント終了条件を満たすまたはスペースでスキップしたら演出を終了する
   if (m_cameraAngle <= m_endEventCameraAngle || m_keyboradStateTracker.IsKeyPressed(DirectX::Keyboard::Space))
   {
	  m_cameraAngle = m_endEventCameraAngle;
	  player->SetStartEvent(false);
	  m_gameManager->GamePlay();
	  player->SetCameraPitch(m_cameraAngle);
   }

   //フェードが終わったら演出を開始する
   if (m_fade->GetFadeIn())
   {
	  m_cameraAngle -= CAMERA_ROTATION_ANGLE * elapsedTime;
   }
   DirectX::SimpleMath::Quaternion yawQuat = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, m_cameraAngle);
   //カメラの更新
   m_cameraManager->Update(DirectX::SimpleMath::Vector3(player->GetPosition().x, 1.0f, player->GetPosition().z), DirectX::SimpleMath::Matrix::CreateFromQuaternion(yawQuat), m_map->GetBoundingBoxes());
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void PlayScene::EndProcessing(float elapsedTime)
{
   UNREFERENCED_PARAMETER(elapsedTime);
   Player* player = m_root->AsPlayer();
   Root* root = dynamic_cast<Root*>(m_root.get());

   //ゴール条件を満たしていない場合処理を行わない
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

   //ゴールフラグやゴールの処理を行う
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
