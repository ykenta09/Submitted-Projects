/*
	@file	IComponent.h
	@brief	コンポーネントのインターフェイスクラス
*/
#pragma once
#ifndef ICOMPONENT_DEFINED
#define ICOMPONENT_DEFINED

#include "Game/CommonResources.h"
#include "Game/Collider.h"
#include "Game/Interface/IObserver.h"
#include "Game/Collider.h"

class Player;
class Enemy;
class Fellow;

class IComponent : public IObserver
{
public:
   //ノード番号を取得する
   virtual int GetNodeNumber() const = 0;
   //パーツIDを取得する
   virtual int GetPartID() const = 0;
   //パーツ番号を取得する
   virtual int GetPartNumber() const = 0;
   //現在の座標を取得する
   virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
   //現在の座標を設定する
   virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;
   //現在の回転角を取得する
   virtual float GetAngle() const = 0;
   //現在の回転角を設定する
   virtual void SetAngle(const float& angle) = 0;
   //スケールを取得する
   virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;
   //モデルを取得する
   virtual DirectX::Model* GetModel() const = 0;

   virtual Player* AsPlayer() = 0;
   virtual Enemy* AsEnemy() = 0;
   virtual Fellow* AsFellow() = 0;

public:
   //仮想デストラクタ
   virtual ~IComponent() = default;
   //初期化
   virtual void Initialize() = 0;
   //更新
   virtual void Update(const float& elapsedTime,
	  const DirectX::SimpleMath::Vector3& position,
	  const float& angle,
	  const DirectX::SimpleMath::Vector3& target) = 0;
   //描画
   virtual void Render() = 0;
   //後処理
   virtual void Finalize() = 0;

   //キーボードが押されたときに呼び出される
   virtual void OnKeyPressed(const DirectX::Keyboard::Keys& key) = 0;
};
#endif // IPLAYER_DEFINED
