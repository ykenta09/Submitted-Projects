/*
	@file	GameParameter.h
	@brief	ゲームパラメーター
*/
#pragma once
#include "pch.h"
#include "Game/Collision/CollisionManager.h"

namespace Params
{
   struct ObjectParameter
   {
	  const DirectX::Model* model = nullptr;			 //モデル
	  DirectX::SimpleMath::Vector3 position;			 //座標
	  DirectX::SimpleMath::Quaternion rotation;			 //回転
	  DirectX::SimpleMath::Vector3 scale;				 //スケール


	  //コンストラクタ
	  ObjectParameter()
		 :
		 position(DirectX::SimpleMath::Vector3::Zero),
		 rotation(DirectX::SimpleMath::Quaternion::Identity),
		 scale(DirectX::SimpleMath::Vector3::One)	
	  {}
   };
}

