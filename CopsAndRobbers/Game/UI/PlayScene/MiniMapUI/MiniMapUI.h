/*
	@file	MiniMapUI.h
	@brief	ミニマップUIクラス
*/
#pragma once
#ifndef MINIMAP_UI_DEFINED
#define MINIMAP_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Player/Player.h"
#include "Game/Character/Enemy/Enemy.h"
#include "Game/Character/Fellow/Fellow.h"
#include "Libraries/MyLib/RenderTexture.h"

// 前方宣言
class CommonResources;

class MiniMapUI
{
public:	 //定数
   static constexpr DirectX::SimpleMath::Vector3 MINIMAP_MASK_VERTEX = { 4.48f, 1.37f, 0.0f };	    //ミニマップマスクの頂点座標
   static constexpr DirectX::SimpleMath::Vector3 MINIMAP_MASK_SCALE = { 0.177f, 0.32f, 1.0f };	    //ミニマップマスクのスケール
   static constexpr DirectX::SimpleMath::Vector2 PLAYER_FELLOW_ICON_SCALE = { 0.8f, 0.8f };			//ミニマップのキャラクターアイコンのサイズ
   static constexpr DirectX::SimpleMath::Vector2 BACK_TEXTURE_POSITION = { 1150.0f, 200.0f };	    //ミニマップの背景テクスチャの描画座標
   static constexpr DirectX::SimpleMath::Vector2 PLAYER_TEXTURE_POSITION = { 0.0f, 0.0f };			//プレイヤーテクスチャの初期座標
   static constexpr DirectX::SimpleMath::Vector2 MINIMAP_SCALE = { 0.8f, 0.8f };					//ミニマップのスケール
   static constexpr DirectX::SimpleMath::Vector2 RENDER_TEXTURE_SIZE = { 310.0f, 300.0f };			//レンダーテクスチャのサイズ
   static constexpr DirectX::SimpleMath::Vector2 CIRCLE_POSITION_OFSET = { 120.0f, 120.0f };	    //円形テクスチャの座標補正

   static constexpr float MAP_SPACE = 2.5f;														   //マップの描画スケール
   static constexpr float MAP_SCALE_X_DIVISOR = 13.0f;											   //マップオブジェクトのスケール補正
   static constexpr float MAP_SCALE_Z_DIVISOR = 14.0f;											   //マップオブジェクトのスケール補正
   static constexpr float GOAL_ICON_OFSET = 10.0f;												   //ゴールアイコンの座標補正
   static constexpr float CIRCLE_SCALE = 1.5f;													   //円形テクスチャのスケール補正
   static constexpr float MAX_ICON_SCALE = 5.0f;												   //マップアイコンのスケール最低値
   static constexpr float MIN_ICON_SCALE = 0.1f;												   //マップアイコンのスケール最大値
   static constexpr RECT BACK_RECT = { 0, 0 , 313, 305 };										   //マップ背景のテクスチャ切り抜き座標
   static constexpr RECT OBJECT_RECT = { 317, 38 , 343, 68 };									   //マップオブジェクトのテクスチャ切り抜き座標
   static constexpr RECT PLAYER_RECT = { 317, 10 , 340, 30 };									   //プレイヤーのテクスチャ切り抜き座標
   static constexpr RECT ENEMY_RECT = { 317, 76 , 338, 106 };									   //敵のテクスチャ切り抜き座標
   static constexpr RECT FELLOW_RECT = { 317, 102 , 343, 132 };									   //仲間のテクスチャ切り抜き座標
   static constexpr RECT GOAL_RECT = { 317, 155 , 343, 175 };									   //ゴールのテクスチャ切り抜き座標
public:
   //	関数
   static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:
   //コンストラクタ
   MiniMapUI(CommonResources* m_commonResources, Player* player, Fellow* fellow, Map* map);
   //デストラクタ
	~MiniMapUI();

	//初期化する
	void Initialize();
	//更新する
	void Update();
	//描画する
	void Render();
	//後始末する
	void Finalize();

	//シェーダーの作成
	void CreateShader();

	//ワールド座標をミニマップ座標に変換
	DirectX::SimpleMath::Vector2 WorldToMiniMap(
	   const DirectX::SimpleMath::Vector3& worldPos,
	   const DirectX::SimpleMath::Vector3& mapCenter,
	   float mapScale);

	//ミニマップ背景の描画
	void DrawMiniMapBack();
	//オブジェクト描画
	void DrawMiniMapObject();
	//累積テクスチャを作成する
	void UpdateCompositionTexture();
	//レンダーテクスチャの初期化
	void InitializeCompositionTexture(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, const D3D11_VIEWPORT& viewport);
	//シェーダーを描画する
	void RenderShader();

private:
   //コンストバッファ
   struct ConstBuffer
   {
	  DirectX::SimpleMath::Matrix		matWorld;	  //ワールド行列
	  DirectX::SimpleMath::Matrix		matView;	  //ビュー行列
	  DirectX::SimpleMath::Matrix		matProj;	  //プロジェクション行列
	  DirectX::SimpleMath::Vector4		Diffuse;	  //ディフューズ
   };

private:
	CommonResources* m_commonResources;		 // 共通リソース
	yamadalib::Graphics* m_graphics;		 //グラフィックス

	//キャラクター
	Player* m_player;				//プレイヤー
	Fellow* m_fellow;				//仲間
	Map* m_map;						//マップ 

	//UI背景画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_miniMapBackTexture;			//ミニマップ背景テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_circleTexture;				//マスク用の円形テクスチャ

	//テクスチャの描画座標
	DirectX::SimpleMath::Vector2 m_circlePosition;					 // 円形テクスチャの描画位置
	DirectX::SimpleMath::Vector2 m_backTexturePosition;				 //背景テクスチャの座標
	DirectX::SimpleMath::Vector2 m_playerTexturePosition;			 //プレイヤーテクスチャの描画座標
	DirectX::SimpleMath::Vector2 m_fellowTexturePosition;			 //仲間テクスチャの描画座標
	DirectX::SimpleMath::Vector2 m_mapTexturePosition;

	DirectX::SimpleMath::Vector3 m_miniMapCenter;		 //ミニマップの中心となる座標

	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	D3D11_VIEWPORT m_miniMapViewpor;						   //ミニマップ用ビューポート
	D3D11_VIEWPORT m_mainViewport;							   //メイン画面のビューポート

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;	//	プリミティブバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;									//スプライトバッチ

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;			   //入力レイアウト

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;			    //頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;			    //ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;		   	//ジオメトリシェーダ

	std::unordered_map<int, std::unique_ptr<DX::RenderTexture>> m_renderTexture;	//レンダーテクスチャ
	DirectX::SimpleMath::Vector2 m_renderTextureSize;								// レンダーテクスチャのサイズ

	bool m_isCompositionTextureInitialized;		//レンダーテクスチャの初期化フラグ
	int m_currentStageNumber;					//現在のステージ番号
};
#endif // MINIMAP_UI_DEFINED
