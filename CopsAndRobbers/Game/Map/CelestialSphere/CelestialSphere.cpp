/*
	@file	CelestialSphere.cpp
	@brief	天球クラス
*/
#include "pch.h"
#include "CelestialSphere.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Libraries/yamadaLib/Resources.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialQuaternion">初期回転</param>
CelestialSphere::CelestialSphere(const DirectX::SimpleMath::Vector3& initialPosition, const DirectX::SimpleMath::Quaternion& initialQuaternion)
	:
	m_position{ initialPosition },
	m_quaternion { initialQuaternion },
    m_graphics(yamadalib::Graphics::GetInstance())
{
}

/// <summary>
/// デストラクタ
/// </summary>
CelestialSphere::~CelestialSphere()
{
}

/// <summary>
/// 初期化する
/// </summary>
void CelestialSphere::Initialize()
{
	//モデルを読み込む
	m_model = yamadalib::Resources::GetInstance()->GetModel("CelestialSphere");
	m_dds = yamadalib::Resources::GetInstance()->GetTexture("CelestialSphere_DDS");
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="centerPosition">天球の中心座標</param>
void CelestialSphere::Update(const DirectX::SimpleMath::Vector3& centerPosition)
{
	//座標を更新する
    m_position = centerPosition;
	//回転させる
	m_currentQuaternion *= DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, ROTATION_SPEED);
}

/// <summary>
/// 描画する
/// </summary>
void CelestialSphere::Render()
{
	//ワールド行列を更新する
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_currentQuaternion) *
	   DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//モデルを描画する
	m_graphics->GetCustomModelRenderer()->Draw(*m_model, m_dds.Get(), world);
}

/// <summary>
/// 後始末する
/// </summary>
void CelestialSphere::Finalize()
{
	// do nothing.
}


