/*
	@file	CharacterBase.cpp
	@brief	キャラクターベースクラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Factory/CharacterPartsFactory.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/yamadaLib/Math.h"

// パーツカウンター
int CharacterBase::s_partsNumber = 0;
// ノードカウンター
int CharacterBase::s_nodeCount = 0;

/// <summary>
/// 
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
CharacterBase::CharacterBase(
	IComponent* parent,
	CommonResources* resources,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle,
	const DirectX::SimpleMath::Vector3& scale)
	:
	m_parent(parent),
	m_commonResources(resources),
	m_nodeNumber(CharacterBase::GetNodeCountAfterCountUp()),
	m_partNumber(CharacterBase::GetPartsNumber()),
	m_partID(CharacterBase::Character),
	m_initialPosition(initialPosition),
	m_initialAngle(DirectX::XMConvertToRadians(initialAngle)),
	m_scale( scale ),
    m_currentPosition{ DirectX::SimpleMath::Vector3::Zero },
    m_currentAngle{ 0.0f },
	m_moveAngle( 0.0f ),
    m_time(0.0f),
    m_objectStageNumber(0),
	m_shadow{},
    m_applyInitialRotation(true)
{
    //グラフィックスのインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
CharacterBase::~CharacterBase()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void CharacterBase::Initialize()
{
	using namespace DirectX::SimpleMath;

	//プレイヤーのパーツを生成
	CharacterBase::Attach(CharacterPartsFactory::CreateBodyParts(this, m_commonResources, m_modelResources, BODY_POSITION, m_initialAngle, Vector3::One));
	CharacterBase::Attach(CharacterPartsFactory::CreateHeadParts(m_parent, m_commonResources, m_modelResources, HEAD_POSITION, m_initialAngle, Vector3::One));
	CharacterBase::Attach(CharacterPartsFactory::CreateArmParts(m_parent, m_commonResources, m_modelResources, LEFT_ARM_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::LEFT_ARM));
	CharacterBase::Attach(CharacterPartsFactory::CreateArmParts(m_parent, m_commonResources, m_modelResources, RIGHT_ARM_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::RIGHT_ARM));
	CharacterBase::Attach(CharacterPartsFactory::CreateFootParts(m_parent, m_commonResources, m_modelResources, LEFT_FOOT_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::LEFT_FOOT));
	CharacterBase::Attach(CharacterPartsFactory::CreateFootParts(m_parent, m_commonResources, m_modelResources, RIGHT_FOOT_POSITION, m_initialAngle + PARTS_ROT_DEG, Vector3::One, CharacterBase::RIGHT_FOOT));

	//影を生成する
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Initialize();

	m_currentPosition = m_initialPosition;
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">更新ターゲット座標</param>
void CharacterBase::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	m_time = elapsedTime;

	//現在の情報を更新
	m_currentPosition = position;

	//m_currentPosition.y -= GRAVITY_SCALE;

	if(m_applyInitialRotation)
	{
	   m_currentAngle = m_initialAngle;
	}
	else
	{
	   // 線形補間で回転を徐々に変化させる
	   m_currentAngle = yamadalib::Math::LerpAngle(m_currentAngle, angle, elapsedTime * ROTATE_LERP_SPEED); // 回転速度（調整可能）;
	}

	// キャラクターパーツを更新する
	for (auto& characterPart : m_characterParts)
	{
		characterPart->Update(elapsedTime, m_currentPosition, m_currentAngle, target);
	}
}

/// <summary>
/// パーツを追加する
/// </summary>
/// <param name="characterParts">追加するパーツ</param>
void CharacterBase::Attach(std::unique_ptr<IComponent> characterParts)
{
	// 初期化する
	m_characterParts.emplace_back(std::move(characterParts));
}

/// <summary>
/// パーツを削除する
/// </summary>
/// <param name="characterParts">削除するパーツ</param>
void CharacterBase::Detach(std::unique_ptr<IComponent> characterParts)
{
}

/// <summary>
/// 描画する
/// </summary>
inline void CharacterBase::Render()
{
	//影を描画する
	m_shadow->Render(DirectX::SimpleMath::Vector3(m_currentPosition.x, SHADOW_Y, m_currentPosition.z));
	
	//キャラクターパーツを描画する
	for (auto& characterPart : m_characterParts)
	{
		characterPart->Render();
	}
}

/// <summary>
/// 後始末する
/// </summary>
void CharacterBase::Finalize()
{
	//キャラクターパーツをリセットする
	m_characterParts.clear();
}

/// <summary>
/// キーボード通知
/// </summary>
/// <param name="keys">押したキー</param>
inline void CharacterBase::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// 衝突したときにキャラクターを押し出す
/// </summary>
/// <param name="position">押し出す座標</param>
void CharacterBase::PushPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_currentPosition = position;
	//キャラクターパーツの更新をする
	for (auto& characterPart : m_characterParts)
	{
		characterPart->Update(m_time, m_currentPosition, m_angleY + m_moveAngle, DirectX::SimpleMath::Vector3::Zero);
	}
}

/// <summary>
/// 初期回転を無効にする
/// </summary>
void CharacterBase::LostInitialRotation()
{
   m_applyInitialRotation = false;
}

/// <summary>
/// パーツモデルを設定する
/// </summary>
/// <param name="resources">モデルのリソース</param>
void CharacterBase::SetPartModels(const CharacterResources& resources)
{
   m_modelResources = resources;
}
