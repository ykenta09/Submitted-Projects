/*
	@file	IUIGroup.h
	@brief	UI�O���[�v�N���X
*/
#pragma once
#ifndef UI_GROUP_DEFINED
#define UI_GROUP_DEFINED

class IUIGroup
{
public:
   virtual void Initialize() = 0;			   //����������
   virtual void Update(const float& elapsedTime, const float& playTime = 0) = 0;		   //�X�V����
   virtual void Render() = 0;		   //�`��
   virtual ~IUIGroup() = default;
};
#endif // UI_GROUP_DEFINED
