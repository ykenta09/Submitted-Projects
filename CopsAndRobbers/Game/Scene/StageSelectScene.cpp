/*
	@file	StageSelectScene.cpp
	@brief	ステージセレクトシーンクラス
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
/// コンストラクタ
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
	//グラフィックスインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
StageSelectScene::~StageSelectScene()
{
   Finalize();
   m_graphics->GetCustomModelRenderer()->ClearAllPointLights();
}

/// <summary>
/// 初期化する
/// </summary>
/// <param name="resources">共通リソース</param>
void StageSelectScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	//カメラの生成
	m_camera = std::make_unique<FixedCamera>(CAMERA_EYE_POSITION, CAMERA_TARGET_POSITION, DirectX::SimpleMath::Vector3::UnitY);

	//カスタムレンダーにカメラを設定する
	m_graphics->GetCustomModelRenderer()->SetCamera(m_camera.get());

	//シーングラフルートを生成する(キャラクターデータを読み込む)
	m_root = std::make_unique<Root>(nullptr, DirectX::SimpleMath::Vector3::Zero, 0.0f);

	//オブジェクトデータを読み込む
	LoadObjects();

	//リソースを読み込む
	LoadResources();

	//パーティクルを生成、初期化
	m_particle = std::make_unique<Particle>();
	m_particle->Create(m_commonResources->GetDeviceResources());
	m_fade = std::make_unique<Fade>();
	m_fade->Create(m_commonResources->GetDeviceResources());
	m_fade->StartFadeOut();

	//サウンドマネージャーを生成、初期化
	m_soundManager = std::make_unique<SoundManager>();
	m_soundManager->Initialize(SoundManager::SoundSceneID::TITLE);

	//プリミティブバッチを生成
	m_primitiveBatchColor = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_commonResources->GetDeviceResources()->GetD3DDeviceContext());
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void StageSelectScene::Update(float elapsedTime)
{
	//サウンドを鳴らす
	m_soundManager->Update();
	//シーングラフルートの更新
	m_root->Update(elapsedTime, DirectX::SimpleMath::Vector3::Zero, 0.0f, DirectX::SimpleMath::Vector3::Zero);

	//テクスチャをフェードさせる
	FadeTexture(elapsedTime);

	// キーボードステートトラッカーを取得する
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
	
	// スペースキーが押されたら
	if (kbTracker->pressed.Space)
	{
	   //最大ステージまで選択可能
	   if (m_stargeNumber > GameParameters::MAX_STAGE_NUM) return;
	     m_commonResources->GetBlackBoard()->SetStageNumber(m_stargeNumber);
		 m_soundManager->GetTitle()->SelectionSE();
		 if (m_soundManager->GetTitle()->IsSEPlaying())
		 {
			   m_sceneFade = true;
		 }
	}

	//フェードアウトが終わったらシーン切り替えのフラグをON
	if (m_fade->GetFadeOut()) 
	{
	   m_isChangeScene = true;
	}
}

/// <summary>
/// 描画する
/// </summary>
void StageSelectScene::Render()
{

   //ビュー行列・プロジェクション行列を取得する
	DirectX::SimpleMath::Matrix view = m_camera->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = m_camera->GetProjectionMatrix();
	//グラフィックスに行列を設定する
	m_graphics->SetProjectiionMatrix(projection);
	m_graphics->SetViewMatrix(view);

	DirectX::SimpleMath::Matrix modlMat = DirectX::SimpleMath::Matrix::CreateRotationY(ROOM_ROTATION) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(ROOM_POSITION);

	//オブジェクトを描画する
	for (const auto& object : m_mapObjects)
	{
	   object->Render();
	}

	//ルートを描画する
	m_root->Render();

	// モデルを描画する
	m_graphics->GetCustomModelRenderer()->Draw(*m_roomModel, m_roomdds.Get(), modlMat);

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
void StageSelectScene::Finalize()
{
    m_root.reset();
	m_loadJson.reset();
	m_fade.reset();
	m_camera.reset();
	m_graphics->Clear();
	
}

/// <summary>
/// 次のシーンIDを取得する
/// </summary>
/// <returns></returns>
IScene::SceneID StageSelectScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		return IScene::SceneID::PLAY;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

/// <summary>
/// リソースを読み込む
/// </summary>
void StageSelectScene::LoadResources()
{
   //モデルをロードする
   m_roomModel = yamadalib::Resources::GetInstance()->GetModel("Room");
   // 画像をロードする
   m_roomdds = yamadalib::Resources::GetInstance()->GetTexture("Room_DDS");
   m_menuTexture = yamadalib::Resources::GetInstance()->GetTexture("Menu");
   m_selectTexture = yamadalib::Resources::GetInstance()->GetTexture("StageSelect");
   m_stargeTexture = yamadalib::Resources::GetInstance()->GetTexture("Stage");
   m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("Timer");
}

/// <summary>
/// フェード
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void StageSelectScene::FadeTexture(float elapsedTime)
{
	if (m_fadeIn) //フェードイン
	{
		m_alpha += FADE_SPEED * elapsedTime;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_fadeIn = false;
		}
	}
	else   //フェードアウト
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
/// 現在選択している状態を変更する
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
/// テクスチャを描画する
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
/// ステージ番号の描画
/// </summary>
/// <param name="stageNumber">ステージ番号</param>
/// <param name="position">1描画座標</param>
void StageSelectScene::DrawStageNumber(int stageNumber, const DirectX::SimpleMath::Vector2& position)
{
   // 現在の描画位置を初期化
   DirectX::SimpleMath::Vector2 drawPosition = position;
   // スプライトバッチの開始
   m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_graphics->GetCommonStates()->NonPremultiplied());

   // 数字を描画するためのラムダ関数
   auto drawDigit = [&](int digit)
   {
	  // RECTの設定
	  RECT rect;
	  rect.top = 0;
	  rect.bottom = DIGIT_HEIGHT;  // 数字の高さ
	  rect.left = static_cast<LONG>(DIGIT_WIDth * digit);  // 数字の左端
	  rect.right = rect.left + DIGIT_WIDth;  // 数字の右端

	  // テクスチャの描画
	  m_graphics->GetSpriteBatch()->Draw(
		 m_timerTexture.Get(),         // テクスチャ
		 drawPosition,                 // 描画位置
		 &rect,                        // ソース矩形
		 DirectX::Colors::White,       // 色
		 0.0f,                         // 回転角度 (必要なら指定可能)
		 DirectX::SimpleMath::Vector2(0, 0), // 原点 (デフォルトは左上)
		 DRAW_SCALE                        // スケール (拡大倍率)
	  );

	  // 次の数字の描画位置を右に移動
	  drawPosition.x += DIGIT_WIDth + SPACING;  // 数字の幅に合わせて移動
   };

   // ステージ番号を1桁ずつ描画
   std::string stageStr = std::to_string(stageNumber);  // ステージ番号を文字列に変換
   for (char ch : stageStr)
   {
	  int digit = ch - '0';  // 文字から数字に変換
	  drawDigit(digit);      // 各数字を描画
   }
   // スプライトバッチの終わり
   m_graphics->GetSpriteBatch()->End();
}

/// <summary>
/// オブジェクトデータを読み込む
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

   //オブジェクトのデータを読み込む
   m_loadJson->LoadObject(filename);
   m_mapObjects = m_loadJson->GetMapObjects();
}
