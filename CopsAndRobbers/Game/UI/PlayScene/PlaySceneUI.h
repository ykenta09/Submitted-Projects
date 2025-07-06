/*
	@file	PlaySceneUI.h
	@brief	プレイシーンUIクラス
*/
#pragma once
#ifndef PLAY_SCENE_UI_DEFINED
#define PLAY_SCENE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Game/UI/PlayScene/KeyCountUI/KeyCountUI.h"
#include "Game/Interface/IUIGroup.h"
#include "Game/UI/PlayScene/MissionMessageUI/MissionMessageUI.h"
#include "Game/UI/PlayScene/StaminaUI/StaminaUI.h"
#include "Game/UI/PlayScene/TimerUI/TimerUI.h"
#include "Game/UI/PlayScene/ControlUI/ControlUI.h"
#include "Game/UI/PlayScene/ControlUI/GimmickControlUI.h"

// 前方宣言
class CommonResources;
class Player;
class Fellow;
class Map;
class GameManager;

class PlaySceneUI : public IUIGroup
{
public:
   static constexpr DirectX::SimpleMath::Vector2 HINT_POSITION = DirectX::SimpleMath::Vector2(0.0f, 150.0f);					 //ヒントテクスチャの座標
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_TOP_POSITION = DirectX::SimpleMath::Vector2(0.0f, -150.0f);		 //上の背景テクスチャの座標
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_BOTTOM_POSITION = DirectX::SimpleMath::Vector2(0.0f, 650.0f);	 //下の背景テクスチャの座標
public:
	//ミニマップUIを取得する
	MiniMapUI* GetMiniMap() const { return m_miniMapUI.get(); }

	//ミッションメッセージUIを取得する
	MissionMessageUI* GetMissionMessageUI() const { return m_missionMessageUI.get(); }
public:
    //コンストラクタ
	PlaySceneUI(CommonResources* resoureces, Player* player, Fellow* fellow, Map* map);
	//デストラクタ
	~PlaySceneUI();

	//初期化する
	void Initialize() override;
	//更新する
	void Update(const float& elapsedTime, const float& playTime = 0) override;
	//描画する
	void Render() override;
	//後始末する
	void Finalize();

	//リソースを読み込む
	void LoadResources();

private:
	// 共通リソース
	CommonResources* m_commonResources;
	//グラフィックス
	yamadalib::Graphics* m_graphics;
	//ゲームマネージャー
	GameManager* m_gameManager;
	//キャラクターのポインタ
	Player* m_player;
	Fellow* m_fellow;
	Map* m_map;
	//ミニマップUI
	std::unique_ptr<MiniMapUI> m_miniMapUI;
	//ミッションメッセージUI
	std::unique_ptr<MissionMessageUI> m_missionMessageUI;
	//鍵の数を表示するUI  
	std::unique_ptr<KeyCountUI> m_keyCountUI;
	//スタミナUI
	std::unique_ptr<StaminaUI> m_staminaUI;
	//タイマーUI
	std::unique_ptr<TimerUI> m_timerUI;
	//操作方法のUI
	std::unique_ptr<ControlUI> m_controlUI;
	//ギミック操作UI
	std::unique_ptr<GimmickControlUI> m_gimmickControlUI;

	//UI背景画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	//ヒント画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hintTexture;
};
#endif // PLAY_SCENE_UI_DEFINED
