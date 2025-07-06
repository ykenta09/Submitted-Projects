/*
	@file	ResultScene.h
	@brief	リザルトシーンクラス
*/
#pragma once
#ifndef RESULT_SCENE_DEFINED
#define RESULT_SCENE_DEFINED
#include "Game/Interface/IScene.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Particle/CircularFade.h"

// 前方宣言
class CommonResources;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

class ResultScene final : public IScene
{
public:
   static constexpr DirectX::SimpleMath::Vector2 BACK_TEXTURE_POSITION = { 0.0f, 0.0f };			  //背景テクスチャの描画座標
   static constexpr DirectX::SimpleMath::Vector2 TIMER_POSITION = { 590.0f, 305.0f };				  //クリア時間の描画座標
   static constexpr DirectX::SimpleMath::Vector2 CLEAR_TIME_TEXT_POSITION = { 190.0f, 280.0f };		  //タイムテキストテクスチャ描画座標
   static constexpr DirectX::SimpleMath::Vector2 PUSHKEY_TEXT_POSITION = { 170.0f, 440.0f };		  //プッシュキーテキストテクスチャ描画座標
   static constexpr DirectX::SimpleMath::Vector2 RESULT_BOARD_POSITION = { 90.0f, 70.0f };			  //リザルトボードテクスチャの描画座標
   static constexpr DirectX::SimpleMath::Vector2 TEXT_POSITION = { 300.0f, 50.0f };					  //リザルトテキストの描画座標
   static constexpr RECT COLON_RECT = { 480, 0, 528, 128 };			 //コロンのテクスチャ座標
   static constexpr int DRAW_TIME_OFSET = 48;						 //時間の描画補正
   static constexpr float FADE_SPEED = 0.5f;						 //フェードの速度

public:
    //コンストラクタ
	ResultScene();
	//デストラクタ
    ~ResultScene() override;

	//初期化する
    void Initialize(CommonResources* resources) override;
	//更新する
    void Update(float elapsedTime)override;
	//描画する
    void Render() override;
	//後始末する
    void Finalize() override;
	//次のシーンIDを取得する
    SceneID GetNextSceneID() const;

	//リソースを読み込む
	void LoadResources();
	//クリアタイムを描画する
	void DrawClearTime();
	//テクスチャをフェードする
	void FadeTexture(float elapsedTime);
private:
	CommonResources* m_commonResources;	// 共通リソース
	yamadalib::Graphics* m_graphics;	//グラフィックス

	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;	// スプライトフォント
	std::unique_ptr<CircularFade> m_circularFade;		//円形のフェード

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;				 //背景テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultTexture;			 //リザルトテキストテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;			 //タイマーの画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTimeTextTexture;	 //クリアタイムテキスト画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushKeyTextTexture;		 //プッシュキーテキスト画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultBoardTexture;		 //リザルトボード画像

	DirectX::SimpleMath::Vector2 m_textPosition;			//リザルトテキスト座標
	DirectX::SimpleMath::Vector2 m_timerPosition;	//クリアタイムを描画する座標
	DirectX::SimpleMath::Vector2 m_clearTimeTextPosition;	//クリアタイムテキストを描画する座標
	DirectX::SimpleMath::Vector2 m_resultTextPosition;	//リザルトテキストを描画する座標
	DirectX::SimpleMath::Vector2 m_resultBoardPosition;	//リザルトボードを描画する座標

	float m_timer;	//タイマー
	float m_clearTime;	//クリアタイム
	bool m_fadeIn;		//フェードインフラグ
	float m_alpha;		//アルファ値

	bool m_isChangeScene;	// シーンチェンジフラグ
};
#endif // RESULT_SCENE_DEFINED
