/*
	@file	CharacterBase.h
	@brief	キャラクターベースクラス
*/
#pragma once
#ifndef CHARACTER_BASE_DEFINED
#define CHARACTER_BASE_DEFINED

#include "Game/Interface/IComposite.h"
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Shadow/Shadow.h"

// 前方宣言
class CommonResources;

class CharacterBase : public IComposite
{
public:
   // パーツ配置用定数
   static constexpr DirectX::SimpleMath::Vector3 BODY_POSITION = { 0.0f, 0.75f,0.0f };			   //胴体の座標
   static constexpr DirectX::SimpleMath::Vector3 HEAD_POSITION = { 0.0f, 0.75f,0.0f };			   //頭の座標
   static constexpr DirectX::SimpleMath::Vector3 LEFT_ARM_POSITION = { -0.3f, 1.1f,0.0f };		   //左腕の座標
   static constexpr DirectX::SimpleMath::Vector3 RIGHT_ARM_POSITION = { 0.3f, 1.1f,0.0f };		   //右腕の座標
   static constexpr DirectX::SimpleMath::Vector3 LEFT_FOOT_POSITION = { -0.25f, 0.5f,0.0f };	   //左足の座標
   static constexpr DirectX::SimpleMath::Vector3 RIGHT_FOOT_POSITION = { 0.25f, 0.5f,0.0f };	   //右足の座標
   static constexpr float PARTS_ROT_DEG = DirectX::XMConvertToRadians(6.0f);   // パーツ初期回転角度（度）
   static constexpr float ROTATE_LERP_SPEED = 10.0f; // 回転補間速度
   static constexpr float SHADOW_Y = -0.4f;  // 影の描画Y座標
   static constexpr float GRAVITY_SCALE = 0.01f;  // 重力のスケール
public:
    //キャラクタータイプ
    enum class CharacterType
    {
	   Player = 0,
	   Fellow,
	   Enemy
	};
	//パーツID
	enum PartID : int
	{
		Character = -1,
		HEAD = 0,
		BODY = 1,
		LEFT_ARM = 2,
		RIGHT_ARM = 3,
		LEFT_FOOT = 4,
		RIGHT_FOOT = 5
	};
	//パーツモデル
	struct CharacterResources
	{
	   DirectX::Model* head;									  //頭のモデル
	   DirectX::Model* body;									  //胴体のモデル  
	   DirectX::Model* arm;										  //腕のモデル
	   DirectX::Model* foot;									  //足のモデル
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> headDDS;  //頭のテクスチャ
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bodyDDS;  //胴体のテクスチャ
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> armDDS;	  //腕のテクスチャ
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> footDDS;  //足のテクスチャ
	};
	
public:
   //ノードカウントアップした後ノードカウントを取得する
   static int GetNodeCountAfterCountUp() { return s_nodeCount++; }
   //ノードカウントを取得する
   static int GetNodeCount() { return s_nodeCount; }

   // パーツ番号をリセットする(-1から開始する)
   static void ResetPartsNumber() { s_partsNumber = -1; }
   // プレイヤーパーツ番号を取得する
   static int GetPartsNumber() { return s_partsNumber; }
   // プレイヤーパーツ番号増やす
   static void IncrementPartsNumber() { s_partsNumber++; }
public:
	//親を取得する
	IComponent* GetParent() const { return m_parent; };
	//ノード番号を取得する
	int GetNodeNumber() const { return m_nodeNumber; }
	//パーツIDを取得する
	int GetPartID() const { return m_partID; }
	//パーツ番号を取得する
	int GetPartNumber() const { return m_partNumber; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	//現在の座標を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const  { return m_currentPosition; }
	//現在の座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)  { m_currentPosition = pos; }
	//現在の回転角を取得する
	float GetAngle() const { return m_currentAngle; };
	//現在の回転角を設定する
	void SetAngle(const float& angle) { m_currentAngle = angle; }
	IComponent* GetLastAttachedNode() const { return nullptr; }

	//オブジェクトのステージ番号を取得する
	int GetObjectStageNumber() const { return m_objectStageNumber; }
	void SetObjectStageNumber(const int& number) { m_objectStageNumber = number; }

	//キャラクターのポインタを取得する
	Player* AsPlayer() override { return nullptr; }
	Enemy* AsEnemy() override { return nullptr; }
	Fellow* AsFellow() override { return nullptr; }

	//スケール取得する
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	//モデルを取得する
	DirectX::Model* GetModel() const { return NULL; }
	//モデルを設定する
	void SetPartModels(const CharacterResources& resources);

	//プレイヤーの回転角を取得する
	const float& GetAngle()  { return m_angleY; }
	//プレイヤーの回転角を設定する
	const void SetAngle(const float angle) { m_angleY = angle; }
	const float& GetMoveAngle() { return m_moveAngle; }

	DirectX::SimpleMath::Vector3 GetInitialPosition() { return m_initialPosition; }
	//移動方向を受け取る
	void SetMoveAngle(const float angle) { m_moveAngle = angle; }

	//キャラクタータイプを取得する
	CharacterType GetCharacterType() const { return m_characterType; }
	//キャラクタータイプを設定する
	void SetCharacterType(CharacterType type) { m_characterType = type; }
public:
	//コンストラクタ
	CharacterBase(
		IComponent* parent,
		CommonResources* resources,
		const DirectX::SimpleMath::Vector3& initialPosition, 
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale);
	//デストラクタ
	~CharacterBase();
	//初期化する
	void Initialize();
	//更新する
	void Update(const float& elapsedTime,
		const DirectX::SimpleMath::Vector3& position,
		const float& angle,
		const DirectX::SimpleMath::Vector3& target);
	//プレイヤーのパーツを追加する
	void Attach(std::unique_ptr<IComponent> playerParts) ;
	//プレイヤーのパーツを削除する
	void Detach(std::unique_ptr<IComponent> playerParts) ;
	//描画する
	inline void Render() ;
	//後処理する
	void Finalize() ;

	//キーボードの通知をする
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys) ;
	//衝突したときに押し出す処理
	void PushPosition(const DirectX::SimpleMath::Vector3& position);
	//初期回転を消す
	void LostInitialRotation();
private:
	   static int s_nodeCount;		 //ノード数
	   static int s_partsNumber;	//パーツ数
protected:
   yamadalib::Graphics* m_graphics;	   //グラフィックス
   IComponent* m_parent;			   //親
   CommonResources* m_commonResources; // 共通リソース

   std::vector<std::unique_ptr<IComponent>> m_characterParts;	//パーツ配列
   std::unique_ptr<Shadow> m_shadow;	//影
   CharacterResources m_modelResources;	//モデルリソース
   CharacterType m_characterType;   //キャラクタータイプ

   DirectX::SimpleMath::Vector3 m_initialPosition;	  //初期座標
   DirectX::SimpleMath::Vector3 m_currentPosition;	  //現在の座標
   float m_initialAngle;							  //初期回転角
   float m_currentAngle;							  //現在の回転
   DirectX::SimpleMath::Vector3 m_scale;   //スケール

   DirectX::SimpleMath::Vector3 m_targetPosition;   //ターゲットの座標

   bool m_applyInitialRotation;   //初期回転を適用するかのフラグ

   int m_nodeNumber;	//ノード番号
   int m_partNumber;	//パーツ番号
   int m_partID;		//パーツID

   float m_angleY;		// 回転角
   float m_moveAngle;	//移動時の回転角

   float m_time;		//経過時間
   int m_objectStageNumber;	 //オブジェクトが存在するステージ番号
};
#endif // CHARACTER_BASE_DEFINED
