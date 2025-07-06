/*
	@file	Player.h
	@brief	プレイヤークラス
*/
#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include "Game/Character/Base/CharacterBase.h"
#include "Game/Interface/IState.h"

#include "Game/Character/Player/Stamina.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Player/State/PlayerIdle.h"
#include "Game/Character/Player/State/PlayerMove.h"
#include "Game/Character/Player/State/PlayerSprint.h"
#include "Game/Character/Player/State/PlayerGoal.h"
#include "Game/Character/Player/State/PlayerTitle.h"
#include "Game/Character/Player/MouseControl.h"
#include "Game/Collision/CollisionManager.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/GameParameter.h"

// 前方宣言
class CommonResources;

class Player : public CharacterBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 COLLISION_EXTENTS = { 0.5f, 1.2f, 0.5f };			//境界ボックスのサイズ
   static constexpr DirectX::SimpleMath::Vector3 COLLISION_OFFSET = { 0.0f, 0.8f, 0.0f };			//境界ボックスのサイズ
   static constexpr float MOVE_SPEED = { 6.50f };			    //プレイヤーの移動速度
   static constexpr float AVOIDANCE_DISTANCE = { 20.0f };	    // 回避行動の移動距離
   static constexpr float AVOIDANCE_DURATION = { 0.5f };	    //回避行動時間
   static constexpr float STAMINA_GAUGE = { 1.0f };			    //スタミナゲージ
   static constexpr float SPRINT_COOLTIME = { 0.5f };			//スプリントクールタイム
public:
    //親を取得する
	IComponent* GetParent() const { return m_parent; };	 	
	//初期座標を取得する
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }	
	//現在の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }		
	//現在の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_currentPosition = pos; }	 

	//境界ボックスを取得する
	const DirectX::BoundingBox& GetBoundingBox() { return m_collisionObject->GetCollisionParameter().collision.box; }	
	//カメラ用の行列を生成する
	DirectX::SimpleMath::Matrix GetCameraMatrix() const { return m_controlMouse->GetCameraMatrix(); }	
	//初期回転角を取得する
	const float& GetInitialAngle() { return m_initialAngle; }			
	//プレイヤーの回転角を取得する
	const float& GetAngle() { return m_currentAngle; }		
	//プレイヤーの回転角を取得する
	const float& GetCameraAngle() { return m_cameraAngle; }		
	//プレイヤーの回転角を設定する
	const void SetAngle(const float angle) { m_currentAngle = angle; }	
	//カメラのピッチを設定する
	void SetCameraPitch(const float& angle) { m_controlMouse->SetYaw(angle); }
	//衝突フラグを取得する
	bool GetHit() { return m_isHit; }	

	//移動方向を取得する
	const float& GetMoveAngle() { return m_nextMoveAngle; }	
	//移動方向を設定する
	void SetMoveAngle(const float angle) { m_nextMoveAngle = angle; }	

	//鍵を持っているか
	bool HasKey() const { return m_hasKey; }				    

	//スタートイベントフラグを設定する(仮置き関数)
	void SetStartEvent(bool flag) { m_isStartEvent = flag; }	
	//ゴールしたかのフラグを設定する（仮置き関数）
	void SetGoalEvent(bool flag) { m_isGoal = flag; }		    
	//ゴールイベントが終了したかどうか（仮置き変数）
	void SetEndGaolEvent(bool flag) { m_isGoalEvent = flag; }	
	//ゴールイベントが終了したか取得する(仮置き変数)
	bool GetEndGaolEvent() { return m_isGoalEvent; }		    

	Player* AsPlayer() { return this; }									//プレイヤーのポインタを取得する
	IState* GetCurrentState() const { return m_currentState; }			//プレイヤーの現在の状態を取得する
	PlayerIdle* GetPlayerIdle() { return m_playerIdle.get(); }			//プレイヤーのアイドル状態を取得する
	PlayerMove* GetPlayerMove() { return m_playerMove.get(); }			//プレイヤーの移動状態を取得する
	PlayerSprint* GetPlayerSprint() { return m_playerSprint.get(); }	//プレイヤーの回避状態を取得する
	PlayerGoal* GetPlayerGoal() { return m_playerGoal.get(); }			//プレイヤーゴール状態を取得する
	PlayerTitle* GetPlayerTitle() { return m_playerTitle.get(); }		//プレイヤーのタイトル状態を取得する
	MouseControl* GetMouseControl() { return m_controlMouse.get(); }	//マウスコントロールを取得する
	Stamina* GetStamina() const { return m_stamina.get(); }				//スタミナを取得する
public:
	//コンストラクタ
	Player(IComponent* parent, CommonResources* resources, const GameParameters::CharacterData data, const int& stageNumber);			 
	~Player();	//デストラクタ
	void Initialize();	//初期化する
	//更新する
	void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle,	const DirectX::SimpleMath::Vector3& target);
	//描画する
	inline void Render();
	//後処理する
	void Finalize();	   

	//当たり判定の初期化
	void InitializeCollision();	
	//ゲームデータを設定する
	void SetGameData();		    
	//衝突時の処理
	void HandleCollision(const CollisionObject* other);	
	//キーボードの通知をする
	void OnKeyPressed(const DirectX::Keyboard::Keys& keys);	
	//衝突したときに押し出す処理
	void PushPosition(const DirectX::SimpleMath::Vector3& position);	
	//現在の状態を変更する
	void ChangeState(IState* newState);	
	//所持している鍵を使用する
	void UseKey();		 
private:
   //モデル情報を適用する
   void ApplyModelAndTexture();
private:
    CommonResources* m_commonResources;					 // 共通リソース
	Player* m_player;									 //プレイヤー自身
	IState* m_currentState;							     //プレイヤーの状態

	//衝突判定のパラメータ
	CollisionObject* m_collisionObject;

	std::unique_ptr<PlayerIdle> m_playerIdle;			 //プレイヤーアイドル状態
	std::unique_ptr<PlayerMove> m_playerMove;			 // プレイヤー移動状態
	std::unique_ptr<PlayerSprint> m_playerSprint;		 // プレイヤー回避状態
	std::unique_ptr<PlayerTitle> m_playerTitle;			 //プレイヤータイトル状態(処理が同じのためゴール状態と統合する)
	std::unique_ptr<PlayerGoal> m_playerGoal;			 //プレイヤーゴール移動処理(処理が同じのためタイトル状態と統合する)
	std::unique_ptr<Stamina> m_stamina;					 //スタミナ
	std::unique_ptr<MouseControl> m_controlMouse;		 //マウスのコントロール

	DirectX::SimpleMath::Quaternion m_rotationPlayerAngle;		  // 回転角
	DirectX::SimpleMath::Quaternion m_rotationPlayerMoveAngle;	  // 移動方向の回転角

	float m_nextMoveAngle;				   //移動時の回転角
	float m_cameraAngle;			   //カメラの回転角
	DirectX::SimpleMath::Vector3 m_playerVelocity;	//プレイヤーのベクトル
	DirectX::SimpleMath::Vector3 m_previousVector;	//プレイヤーの前回の移動時のベクトル
	
	float m_sprintCoolTime;	//スプリントクールタイム

	bool m_isHit;			  //敵との衝突フラグ

	bool m_hasKey;			  //鍵を持っているか
	int m_keyNum;			  //所持している鍵の数

	bool m_isStartEvent;	  //スタートイベント中かどうか
	bool m_isGoal;			  //ゴールしたか
	bool m_isGoalEvent;		  //ゴールイベントが終了した
};
#endif // PLAYER_DEFINED
