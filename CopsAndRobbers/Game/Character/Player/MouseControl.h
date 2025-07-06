/*
	@file	MouseControl.h
	@brief	マウス制御クラス
*/
#ifndef MOUSE_CONTROL_DEFINED
#define MOUSE_CONTROL_DEFINED
#pragma once
// 前方宣言
class CommonResources;


class MouseControl
{
public:
	//マウスのステートを取得する
	DirectX::Mouse::State GetMouseState() { return m_mouseState; }
	//カメラ用の行列を取得する
	DirectX::SimpleMath::Matrix GetCameraMatrix() { return m_mouseMatrix; }

	//カメラのY軸回転を取得する
	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }

	void SetYaw(const float& yaw) { m_yaw = yaw; }
public:
    //コンストラクタ
	MouseControl(CommonResources* commonResources);
	//デストラクタ
	~MouseControl();
	//初期化する
	void Initialize(CommonResources* resources);
	//更新する
	void Update(float elapsedTime);
	//クォータニオンの計算
	void QuaternionCalculation();
	//キー操作する
	void OnKeyPressed();
private:
	CommonResources* m_commonResources;	// 共通リソース

	DirectX::Mouse::State m_mouseState;	//マウスのステート
	DirectX::Keyboard::State m_keyboardState;	//キーボードステート

	DirectX::SimpleMath::Matrix m_mouseMatrix;	//マウスの行列

	float m_yaw;	//マウスの回転角（X軸）
	float m_pitch;	//マウスの回転角（Y軸）
};
#endif // MOUSE_CONTROL_DEFINED
