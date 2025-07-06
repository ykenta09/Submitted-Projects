/*
	@file	IUIGroup.h
	@brief	UIグループクラス
*/
#pragma once
#ifndef UI_GROUP_DEFINED
#define UI_GROUP_DEFINED

class IUIGroup
{
public:
   virtual void Initialize() = 0;			   //初期化処理
   virtual void Update(const float& elapsedTime, const float& playTime = 0) = 0;		   //更新処理
   virtual void Render() = 0;		   //描画
   virtual ~IUIGroup() = default;
};
#endif // UI_GROUP_DEFINED
