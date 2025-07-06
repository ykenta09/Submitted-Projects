/*
	@file	StageSelectScene.h
	@brief	ステージセレクトシーンクラス
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

// 前方宣言
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
	//選択している物
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
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_EYE_POSITION = { 0.0f, 1.5f, 15.0f };	   //カメラの座標
   static constexpr DirectX::SimpleMath::Vector3 CAMERA_TARGET_POSITION = { 0.0f, 3.5f, -40.0f };  //カメラの注視点
   static constexpr DirectX::SimpleMath::Vector3 ROOM_POSITION = { -1.0f, -3.0f, -10.0f };		   //部屋モデルの座標
   static constexpr DirectX::SimpleMath::Vector2 SELECT_POSITION = { 200.0f, 30.0f };			   //セレクト画像の座標
   static constexpr DirectX::SimpleMath::Vector2 START_POSITION = { 470.0f, 550.0f };			   //スタートテクスチャの座標
   static constexpr DirectX::SimpleMath::Vector2 LEFT_ARROW_POSITION = { 30.0f, 300.0f };		   //左移動のテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 RIGHT_ARROW_POSITION = { 1100.0f, 300.0f };	   //右移動のテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 STARGE_POSITION = { 400.0f, 220.0f };			   //ステージ情報テクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 MENU_POSITION = { 0.0f, 0.0f };				   //メニューテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 CURSOR_POSITION = { 120.0f, 370.0f };			   //カーソルテクスチャ座標
   static constexpr DirectX::SimpleMath::Vector2 STARGE_NUMBER_POSITION = { 750.0f, 330.0f };	   //ステージ番号描画座標

   static constexpr float CURSOR_MOVE_POSITION = 130.0f;							//カーソルの移動量
   static constexpr float ROOM_ROTATION = DirectX::XMConvertToRadians(0.0f);		//部屋の回転角
   static constexpr int DIGIT_WIDth = 57;					// 数字の幅
   static constexpr int DIGIT_HEIGHT = 96;					// 数字の高さ
   static constexpr int SPACING = 5;						// 数字の間隔
   static constexpr float DRAW_SCALE = 1.5f;				//ステージ番号描画スケール
   static constexpr float FADE_SPEED = { 0.5f };			//フェード速度

   static constexpr RECT START_RECT = { 0, 180 , 900, 320 };									//スタートテクスチャ座標
   static constexpr RECT SELECT_RECT = { 0, 0 , 910, 180 };										//セレクトテクスチャ座標
   static constexpr RECT LEFT_ARROW_RECT = { 0, 320 , 145, 500 };								//左移動テクスチャ座標
   static constexpr RECT RIGHT_ARROW_RECT = { 150, 320 , 280, 500 };							//右移動テクスチャ座標
   static constexpr DirectX::XMVECTORF32 MENU_COLOR = { 0.5f, 0.5f, 0.5f, 0.5f };				//メニューテクスチャ座標

public:
   //コンストラクタ
   StageSelectScene();
   //デストラクタ
    ~StageSelectScene() override;

	//初期化
    void Initialize(CommonResources* resources) override;
	//更新
    void Update(float elapsedTime)override;
	//描画
    void Render() override;
	//後始末
    void Finalize() override;
	//次のシーンIDを取得する
    SceneID GetNextSceneID() const;

	//リソースを読み込む
	void LoadResources();
	//テクスチャフェード
	void FadeTexture(float elapsedTime);
	//現在の選択している状態を変更
	void ChangeSelectState();
	//テクスチャを描画する
	void DrawTexture();
	//ステージナンバーを描画する
	void DrawStageNumber(int stageNumber, const DirectX::SimpleMath::Vector2& position);
private:
   //オブジェクトを読み込む
   void LoadObjects();
private:
	CommonResources* m_commonResources;				  // 共通リソース
	yamadalib::Graphics* m_graphics;				  //グラフィックス
	std::unique_ptr<SoundManager> m_soundManager;	  //サウンド
	std::unique_ptr<FixedCamera> m_camera;			  //カメラ
	SelectState m_currentState;						  //現在選択している状態
	std::unique_ptr<CelestialSphere> m_celestialSphere;	//天球
	std::unique_ptr<IComposite> m_root;				  //シーングラフルート

	//オブジェクト配列
	std::vector<std::unique_ptr<IMapObject>> m_mapObjects;

	//パーティクル
	std::unique_ptr<Particle> m_particle;
	std::unique_ptr<Fade> m_fade;
	std::unique_ptr<LoadJson> m_loadJson;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_menuTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_selectTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stargeTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;
	
	//座標
	DirectX::SimpleMath::Vector2 m_cursorPosition;
	DirectX::SimpleMath::Vector2 m_stargeNumberPosition;

	//2D座標
	DirectX::SimpleMath::Vector2 m_selectPosition;
	DirectX::SimpleMath::Vector2 m_startPosition;
	DirectX::SimpleMath::Vector2 m_stargePosition;
	DirectX::SimpleMath::Vector2 m_menuPosition;

	//モデル
	DirectX::Model* m_roomModel;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomdds;  //テクスチャ

	// シーンチェンジフラグ
	bool m_isChangeScene;
	bool m_fadeIn;
	bool m_sceneFade;
	float m_alpha;

	//ステージ番号
	int m_stargeNumber;

	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatchColor;
};
#endif // STAGE_SELECT_SCENE_DEFINED
