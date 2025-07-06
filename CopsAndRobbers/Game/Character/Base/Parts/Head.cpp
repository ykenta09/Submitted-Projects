/*
	@file	Head.cpp
	@brief	頭クラス
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/yamadaLib/Resources.h"
#include "Game/Screen.h"
#include "Game/Character/Base/Parts/Head.h"
#include "Game/Factory/CharacterPartsFactory.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="resources">共通リソース</param>
/// <param name="model">モデル</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
/// <param name="scale">スケール</param>
/// <param name="partId">パーツのID</param>
Head::Head(
	IComponent* parent,
	CommonResources* resources,
	const CharacterBase::CharacterResources& model,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle,
	const DirectX::SimpleMath::Vector3& scale)
	:
	m_parent(parent),
	m_commonResources(resources),
	m_nodeNumber(CharacterBase::GetNodeCountAfterCountUp()),
	m_partNumber(CharacterBase::GetPartsNumber()),
	m_partID(CharacterBase::HEAD),
	m_initialPosition(initialPosition),
	m_initialAngle(initialAngle),
	m_currentPosition{},
	m_currentAngle{},
	m_model( model.head ),
    m_modelDDS(model.headDDS),
	m_scale( scale )
{
    //グラフィックスのインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
Head::~Head()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void Head::Initialize()
{
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">更新ターゲット座標</param>
inline void Head::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	UNREFERENCED_PARAMETER(target);

	//現在の情報を更新する
	m_currentPosition = position + m_initialPosition;
	m_currentAngle =  angle;

	//ワールド行列を生成する
	m_worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(m_initialPosition) *
	   DirectX::SimpleMath:: Matrix::CreateRotationY(m_currentAngle) *
	   DirectX::SimpleMath:: Matrix::CreateTranslation(m_currentPosition);
}

/// <summary>
/// 描画する
/// </summary>
inline void Head::Render()
{
	// モデルを描画する
    m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_modelDDS.Get(), m_worldMatrix);
}

/// <summary>
/// 後始末する
/// </summary>
void Head::Finalize()
{
	// do nothing.
}

/// <summary>
/// キーボード通知
/// </summary>
/// <param name="keys">押したキー</param>
inline void Head::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}
