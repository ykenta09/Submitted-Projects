/*
	@file	Light.cpp
	@brief	ライトクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Map/Light/Light.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include "Game/Collision/CollisionObject.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="resources">共通リソース</param>
/// <param name="param">オブジェクトパラメータ</param>
/// <param name="lightColor">ライトカラー</param>
Light::Light(CommonResources* resources, const GameParameters::ObjectParameter& param, const DirectX::SimpleMath::Vector3& lightColor, const float& range, const bool& modelDraw)
	:
    ObjectBase(param.initialPos, param.initialRot, param.scale),
    m_commonResources(resources),
    m_objectStageNumber(param.stageParam.objectNumber),
    m_color(lightColor),
    m_range(range),
    m_modelDraw(modelDraw),
    m_collisionObject(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Light::~Light()
{
}

/// <summary>
/// 初期化する
/// </summary>
void Light::Initialize()
{
	// モデルを読み込む
	SetModel(yamadalib::Resources::GetInstance()->GetModel("Light"));
	m_dds = yamadalib::Resources::GetInstance()->GetTexture("Light_DDS");

	//ライトの設定
	SettingLight();
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
void Light::Update(const float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}


/// <summary>
/// 描画する
/// </summary>
void Light::Render()
{
   //モデル描画が不要な場合は描画しない
   if (!m_modelDraw) return;
	// ワールド行列を更新する
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(GetScale()) *
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetRotation()) *
		DirectX::SimpleMath::Matrix::CreateTranslation(GetPosition());

	// モデルを描画する
	m_graphics->GetCustomModelRenderer()->Draw(*GetModel(), m_dds.Get(), world);
}

/// <summary>
/// 後始末する
/// </summary>
void Light::Finalize()
{
	// do nothing.
}

/// <summary>
/// 衝突判定の登録
/// </summary>
void Light::InitializeCollision()
{
}

/// <summary>
/// 衝突時のイベント処理
/// </summary>
/// <param name="other">衝突対象のコリジョンオブジェクト</param>
void Light::HandleCollision(const CollisionObject* other)
{
   UNREFERENCED_PARAMETER(other);
}

/// <summary>
/// ライトの設定
/// </summary>
void Light::SettingLight()
{
   //ライトのパラメータ設定
   CustomModelRenderer::PointLight lights;
   lights.position = GetPosition() - DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
   lights.range = m_range;
   lights.color = m_color;
   m_graphics->GetCustomModelRenderer()->SetPointLightParameters(lights);
}
