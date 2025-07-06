/*
	@file	TitleScene.h
	@brief	タイトルシーンクラス
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

// 前方宣言
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
	//選択している物
	enum SelectState
	{
		PLAY = 0,
		QUIT
	};
public:
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_EYE_POSITION = { 0.0f, 2.0f, 15.0f };		  //カメラの座標
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_TARGET_POSITION = { 0.0f, 3.5f, -40.0f };	  //カメラの注視点
   static constexpr DirectX::SimpleMath::Vector3 PRISON_POSITION = { -0.1f, -0.5f, 8.5f };			  //牢屋座標
   static constexpr DirectX::SimpleMath::Vector3 FELLOW_POSITION = { -1.5f, 0.3f, 8.7f };			  //仲間座標
   static constexpr DirectX::SimpleMath::Vector3 PLAYER_POSITION = { -8.5f, 0.0f, 10.5f };			  //プレイヤー座標
   static constexpr DirectX::SimpleMath::Vector3 ENEMY_POSITION = { -11.0f, 0.0f, 10.5f };			  //敵座標

   static constexpr DirectX::SimpleMath::Vector2 TITLE_TEXTURE_POSITION = { 250.0f, -50.0f };		  //タイトルテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 START_TEXTURE_POSITION = { 450.0f, 350.0f };		  //スタートテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 END_TEXTURE_POSITION = { 450.0f, 500.0f };			  //エンドテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 MENU_TEXTURE_POSITION = { 0.0f, 0.0f };			  //メニューテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 CURSOR_INITIALIZE_POSITION = { 0.0f, 360.0f };		  //カーソル初期座標
   static constexpr RECT START_RECT = { 0, 0, 810, 150 };											  //スタートテクスチャ座標
   static constexpr RECT END_RECT = { 0, 150, 810, 250 };											  //エンドテクスチャ座標
   static constexpr DirectX::XMVECTORF32 MENU_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };					  //メニューカラー
   static constexpr DirectX::XMVECTORF32 CURSOR_COLOR = { 0.3f, 0.3f, 0.3f, 0.3f };					  //カーソルカラー
   static constexpr float FADE_SPEED = { 0.5f };													  //フェード速度

   static constexpr float CURSOR_MOVE_POSITION = 130.0f;							//カーソルの移動量
   const GameParameters::ObjectParameter TITLE_PRISON_PARAMETER =					//牢屋のパラメータ
   {
	   PRISON_POSITION,										//初期座標
	   DirectX::SimpleMath::Quaternion::Identity,			//初期回転
	   DirectX::SimpleMath::Vector3::One,					//スケール
	   {0, DirectX::SimpleMath::Vector3::Zero}				//ステージパラメータ
   };
public:
    //コンストラクタ
    TitleScene();
	//デストラクタ
    ~TitleScene() override;

	//初期化
    void Initialize(CommonResources* resources) override;
	//更新する
    void Update(float elapsedTime)override;
	//描画する
    void Render() override;
	//後始末する
    void Finalize() override;

	//次のシーンIDを取得する
    SceneID GetNextSceneID() const;
private:
   //リソースを読み込む
   void LoadResources();
   //テクスチャを描画する
   void DrawTexture();
   //モデルを描画する
   void DrawModel();
   //現在の選択している状態を変更
   void ChangeSelectState();
   //フェードテクスチャ
   void FadeTexture(float elapsedTime);
   //データからオブジェクトを生成する
   void LoadObjects();
private:
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス
	yamadalib::Graphics* m_graphics;
	//サウンド
	std::unique_ptr<SoundManager> m_soundManager;
	//カメラ
	std::unique_ptr<FixedCamera> m_camera;
	//現在選択している状態
	SelectState m_currentState;
	//牢屋
	std::unique_ptr<Prison> m_prison;
	//天球
	std::unique_ptr<CelestialSphere> m_celestialSphere;
	std::unique_ptr<LoadJson> m_loadJson;
	//仲間
	std::unique_ptr<Fellow> m_fellow;
	//シーングラフルート
	std::unique_ptr<IComposite> m_root;
	//オブジェクト配列
	std::vector<std::unique_ptr<IMapObject>> m_mapObjects;

	//パーティクル
	std::unique_ptr<Particle> m_particle;
	std::unique_ptr<Fade> m_fade;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_menuTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cursorTexture;
	//座標
	DirectX::SimpleMath::Vector2 m_cursorPosition;

	//モデル
	DirectX::Model* m_titleModel;

	// シーンチェンジフラグ
	bool m_isChangeScene;
	bool m_fadeIn;
	bool m_sceneFade;
	float m_alpha;

};
#endif // TITLE_SCENE_DEFINED
