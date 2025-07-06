/*
	@file	TitleScene.cpp
	@brief	タイトルシーンクラス
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
/// コンストラクタ
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
	//グラフィックスインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
   Finalize();
   m_root.reset();
   m_graphics->GetCustomModelRenderer()->ClearAllPointLights();
}

/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
void TitleScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	//カメラを生成、初期化
	m_camera = std::make_unique<FixedCamera>(CAMERA_EYE_POSITION, CAMERA_TARGET_POSITION, DirectX::SimpleMath::Vector3::UnitY);

	//カスタムレンダーにカメラを設定する
	m_graphics->GetCustomModelRenderer()->SetCamera(m_camera.get());

	//天球の生成、初期化
	m_celestialSphere = std::make_unique<CelestialSphere>(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Quaternion::Identity);
	m_celestialSphere->Initialize();

	m_root = std::make_unique<Root>(nullptr, DirectX::SimpleMath::Vector3::Zero, 0.0f);

	//オブジェクトデータを読み込む
	LoadObjects();

	m_root->AsPlayer()->ChangeState(m_root->AsPlayer()->GetPlayerTitle());
	Root* root = dynamic_cast<Root*>(m_root.get());
	for (const auto& enemy : root->GetEnemies())
	{
	   enemy->ChangeState(enemy->GetEnemyTitle());
	}

	//リソースを読み込む
	LoadResources();

	//パーティクルを生成、初期化
	m_particle = std::make_unique<Particle>();
	m_particle->Create(m_commonResources->GetDeviceResources());
	m_fade = std::make_unique<Fade>();
	m_fade->Create(m_commonResources->GetDeviceResources());
	m_fade->StartFadeOut();

	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	m_soundManager = std::make_unique<SoundManager>();
	m_soundManager->Initialize(SoundManager::SoundSceneID::TITLE);
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void TitleScene::Update(float elapsedTime)
{
	//サウンドを鳴らす
	m_soundManager->Update();
	//天球を更新する
	m_celestialSphere->Update(DirectX::SimpleMath::Vector3::Zero);
	//シーングラフルートの更新
	m_root->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, 0.0f, m_root->AsPlayer()->GetPosition());

	//オブジェクトの更新
	for (const auto& object : m_mapObjects)
	{
	   object->Update(elapsedTime);
	}

	//フェードテクスチャ
	FadeTexture(elapsedTime);

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	if (kbTracker->pressed.Up || kbTracker->pressed.W)
	{
		ChangeSelectState();
	}
	else if (kbTracker->pressed.Down || kbTracker->pressed.S)
	{
		ChangeSelectState();
	}
	// スペースキーが押されたら
	if (kbTracker->pressed.Space)
	{
		if (m_currentState == SelectState::PLAY) //セレクト画面に移動
		{
			m_soundManager->GetTitle()->SelectionSE();
			if (m_soundManager->GetTitle()->IsSEPlaying())
			{
				m_sceneFade = true;
			}
		}
		else //ゲームを終了
		{
			PostQuitMessage(0);
		}
	}

	//フェードアウトが終わったらシーンを切り替える
	if (m_fade->GetFadeOut()) 
	{
	   m_isChangeScene = true;
	}
}


/// <summary>
/// 描画する
/// </summary>
void TitleScene::Render()
{
    //ビュー行列とプロジェクション行列を取得する
	DirectX::SimpleMath::Matrix view = m_camera->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = m_camera->GetProjectionMatrix();
	//グラフィックスに行列を設定する
	m_graphics->SetProjectiionMatrix(projection);
	m_graphics->SetViewMatrix(view);

	//モデルを描画する
	DrawModel();

	//テクスチャを描画する
	DrawTexture();

	//パーティクルの描画
	m_particle->Render();
	if (m_sceneFade)
	{
	   m_fade->Render();
	}
}

/// <summary>
/// 後始末する
/// </summary>
void TitleScene::Finalize()
{
	// do nothing.
}

/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns></returns>
IScene::SceneID TitleScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
	   return IScene::SceneID::STAGE_SELECT;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

/// <summary>
/// フェードテクスチャ
/// </summary>
/// <param name="elapsedTime">経過時間</param>
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
/// オブジェクトデータを読み込む
/// </summary>
void TitleScene::LoadObjects()
{
   //シーングラフルートを生成する(キャラクターデータを作成)
   const std::string& filename = "Resources\\GameData\\TitleSceneData.json";
   m_loadJson = std::make_unique<LoadJson>(m_commonResources, nullptr, nullptr);
   m_loadJson->LoadCharacterData(filename);
   m_loadJson->TransferCharactersTo([this](std::unique_ptr<IComponent> character)
	  {
		 m_root->Attach(std::move(character));
	  });


   //オブジェクトのデータを読み込む
   m_loadJson->LoadObject(filename);
   m_mapObjects = m_loadJson->GetMapObjects();

}

/// <summary>
/// 現在選択している状態を変更する
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
/// リソースを読み込む
/// </summary>
void TitleScene::LoadResources()
{
   // 画像をロードする
   m_titleTexture = yamadalib::Resources::GetInstance()->GetTexture("Title");
   m_textTexture = yamadalib::Resources::GetInstance()->GetTexture("Text");
   m_menuTexture = yamadalib::Resources::GetInstance()->GetTexture("Menu");
   m_cursorTexture = yamadalib::Resources::GetInstance()->GetTexture("Cursor");
}

/// <summary>
/// テクスチャを描画する
/// </summary>
void TitleScene::DrawTexture()
{
   //メニューのカラー
   DirectX::XMVECTORF32 modifiedColor = { 1.0f, 1.0f, 1.0f, m_alpha };
   //テクスチャを描画する
   m_graphics->DrawTexture(m_menuTexture, MENU_TEXTURE_POSITION, nullptr, MENU_COLOR);
   m_graphics->DrawTexture(m_titleTexture, TITLE_TEXTURE_POSITION, nullptr);
   m_graphics->DrawTexture(m_cursorTexture, m_cursorPosition, nullptr, CURSOR_COLOR);
   //選択している状態で描画を変更する
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
/// モデルを描画する
/// </summary>
void TitleScene::DrawModel()
{
   //天球を描画する
   m_celestialSphere->Render();

   //ルートの描画
   m_root->Render();

   // モデルを描画する
   for (const auto& object : m_mapObjects)
   {
	  object->Render();
   }
}
