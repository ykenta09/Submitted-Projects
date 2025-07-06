/*
	@file	CelestialSphere.h
	@brief	天球クラス
*/
#pragma once
#ifndef CELESTISL_SPHERE_DEFINDE
#define  CELESTISL_SPHERE_DEFINDE

#include "Libraries/yamadaLib/Graphics.h"

// 前方宣言
class CommonResources;

class CelestialSphere
{
public:
   static constexpr float ROTATION_SPEED = { DirectX::XMConvertToRadians(0.01f) };		   //天球の回転速度
public:
   //コンストラクタ
	CelestialSphere(const DirectX::SimpleMath::Vector3& initialPosition,
		const DirectX::SimpleMath::Quaternion&  initialQuaternion);
	//デストラクタ
	~CelestialSphere();
	//初期化
	void Initialize();
	//更新する
	void Update(const DirectX::SimpleMath::Vector3& centerPosition);
	//描画する
	void Render();
	//後始末をする
	void Finalize();

private:
    yamadalib::Graphics* m_graphics;   //グラフィックス
	DirectX::Model* m_model;	//モデル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_dds;  //テクスチャ

	DirectX::SimpleMath::Vector3 m_position;	//初期座標

	DirectX::SimpleMath::Quaternion m_quaternion;		 //初期回転角
	DirectX::SimpleMath::Quaternion m_currentQuaternion; //現在の回転角
};

#endif // CELESTISL_SPHERE_DEFINDE
