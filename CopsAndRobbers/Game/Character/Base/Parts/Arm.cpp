/*
	@file	Arm.cpp
	@brief	腕クラス
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
#include "Game/Character/Base/Parts/Arm.h"
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
Arm::Arm(
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
	m_model{ model.arm },
    m_modelDDS(model.armDDS),
	m_currentPosition(),
	m_scale( scale ),
	m_rotaY(DirectX::XMConvertToRadians(0.0f)),
	m_isMoving( false )
{
    //グラフィックスのインスタンスを取得する
	m_graphics = yamadalib::Graphics::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
Arm::~Arm()
{
	Finalize();
}

/// <summary>
/// 初期化する
/// </summary>
void Arm::Initialize()
{
	if (m_partID == CharacterBase::LEFT_ARM)
	{
		m_rotaZ = DirectX::XMConvertToRadians(LEFT_ARM_INIT_Z_ROT_DEG);
	}
	else
	{
		m_rotaZ = DirectX::XMConvertToRadians(RIGHT_ARM_INIT_Z_ROT_DEG);
	}
	//腕の振り方向の初期値の設定
	m_direction = DirectX::XMConvertToRadians(ARM_SWING_INIT_DEG);
}


/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">更新ターゲット座標</param>
inline void Arm::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
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
	//移動中なら腕を振る
	Move();

	// ワールド行列を更新する
	m_worldMatrix =
	     DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
		 DirectX::SimpleMath::Matrix::CreateRotationX(m_rotaY) *												//腕を振る回転	
		 DirectX::SimpleMath::Matrix::CreateRotationZ(m_rotaZ) *												//腕を胴体にくっ付ける回転
		 DirectX::SimpleMath::Matrix::CreateTranslation(m_initialPosition) *									//腕の初期位置
		 DirectX::SimpleMath::Matrix::CreateRotationY(m_currentAngle) *										//移動した際に回転
		 DirectX::SimpleMath::Matrix::CreateTranslation(m_currentPosition);									//現在の位置
}

/// <summary>
/// 描画する
/// </summary>
inline void Arm::Render()
{
	// モデルを描画する
    m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_modelDDS.Get(), m_worldMatrix);
}

/// <summary>
/// 後始末する
/// </summary>
void Arm::Finalize()
{
	// do nothing.
}


/// <summary>
/// キーボード通知
/// </summary>
/// <param name="keys">押したキー</param>
inline void Arm::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}


/// <summary>
/// 移動中腕を振る
/// </summary>
void Arm::Move()
{
	//動いているときだけ腕を振る
	if (!m_isMoving)
	{
	   // 減衰係数を適用して回転角度を徐々に戻す
	   // 0 ～ 1 の範囲で調整可能（1 に近づくほどゆっくり減衰）
	   m_rotaY *= ARM_SWING_DECAY;
	   return;
	}

	// 腕の振る方向を左右で反転させる
	if (m_partID == CharacterBase::LEFT_ARM)
	{
	   m_rotaY += m_direction;  // 左腕: 正の方向に回転
	   if (m_rotaY >= DirectX::XMConvertToRadians(ARM_SWING_LIMIT_DEG) || m_rotaY <= DirectX::XMConvertToRadians(-ARM_SWING_LIMIT_DEG))
	   {
		  m_direction *= -1;
	   }
	}
	else
	{
	   m_rotaY -= m_direction;  // 右腕: 負の方向に回転
	   if (m_rotaY <= DirectX::XMConvertToRadians(-ARM_SWING_LIMIT_DEG) || m_rotaY >= DirectX::XMConvertToRadians(ARM_SWING_LIMIT_DEG))
	   {
		  m_direction *= -1;
	   }
	}
}
