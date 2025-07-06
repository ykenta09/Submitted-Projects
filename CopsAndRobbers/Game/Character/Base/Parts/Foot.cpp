/*
	@file	Foot.cpp
	@brief	足クラス
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
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Character/Base/Parts/Foot.h"
#include "Game/Factory/CharacterPartsFactory.h"
#include "Libraries/yamadaLib/Math.h"

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
Foot::Foot(
	IComponent* parent,
	CommonResources* resources,
	const CharacterBase::CharacterResources& model,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle,
	const DirectX::SimpleMath::Vector3& scale,
	CharacterBase::PartID partId)
	:
	m_parent( parent),
	m_commonResources(resources),
	m_nodeNumber(CharacterBase::GetNodeCountAfterCountUp()),
	m_partNumber(CharacterBase::GetPartsNumber()),
	m_partID(partId),
	m_initialPosition(initialPosition),
	m_initialAngle(initialAngle),
	m_currentPosition{},
	m_currentAngle{},
	m_model( model.foot ),
    m_modelDDS(model.footDDS),
	m_scale( scale ),
	m_direction(DirectX::XMConvertToRadians(FOOT_SWING_INIT_DEG)),
	m_rotaY(DirectX::XMConvertToRadians(FOOT_ROT_OFFSET_Y)),
	m_isMoving( false )
{
    //グラフィックスのインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
Foot::~Foot()
{
	Finalize();
}


/// <summary>
/// 初期化する
/// </summary>
void Foot::Initialize()
{
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">更新ターゲット座標</param>
inline void Foot::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	UNREFERENCED_PARAMETER(target);

	// 位置が変化していれば移動中フラグを立てる
	if (m_currentPosition != position)
		m_isMoving = true;
	else
		m_isMoving = false;

	//現在の情報を更新する
	m_currentPosition = position;
	m_currentAngle = angle;

	//移動中なら足を動かす
	Move();

	//ワールド行列を生成する
	m_worldMatrix =
	    DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
		DirectX::SimpleMath::Matrix::CreateRotationX(m_rotaY) *
		DirectX::SimpleMath::Matrix::CreateTranslation(m_initialPosition) *
		DirectX::SimpleMath::Matrix::CreateRotationY(m_currentAngle) *
		DirectX::SimpleMath::Matrix::CreateTranslation(m_currentPosition);
}

/// <summary>
/// 描画する
/// </summary>
inline void Foot::Render()
{
	// モデルを描画する
    m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_modelDDS.Get(), m_worldMatrix);
}

/// <summary>
/// 後始末する
/// </summary>
void Foot::Finalize()
{
	// do nothing.
}

/// <summary>
/// キーボード通知
/// </summary>
/// <param name="keys">押したキー</param>
inline void Foot::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// 移動中足を動かす
/// </summary>
void Foot::Move()
{
	//動いているときだけ足を動かす
	if (!m_isMoving)
	{
	   // 減衰係数を適用して回転角度を徐々に戻す
	   // 0 ～ 1 の範囲で調整可能（1 に近づくほどゆっくり減衰）
	   m_rotaY *= FOOT_SWING_DECAY;
	   return;
	}

	// 足を動かす方向を左右で反転させる
	if (m_partID == CharacterBase::LEFT_FOOT)
	{
	   m_rotaY += m_direction;  // 左足: 正の方向に回転
	   if (m_rotaY >= DirectX::XMConvertToRadians(FOOT_SWING_LIMIT_DEG) || m_rotaY <= DirectX::XMConvertToRadians(-FOOT_SWING_LIMIT_DEG))
	   {
		  m_direction *= -1;
	   }
	}
	else
	{
	   m_rotaY -= m_direction;  // 右足: 負の方向に回転
	   if (m_rotaY <= DirectX::XMConvertToRadians(-FOOT_SWING_LIMIT_DEG) || m_rotaY >= DirectX::XMConvertToRadians(FOOT_SWING_LIMIT_DEG))
	   {
		  m_direction *= -1;
	   }
	}
}
