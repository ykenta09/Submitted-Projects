/*
	@file	GameManager.cpp
	@brief	�Q�[���}�l�[�W���[�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "FrustumCuller.h"


/// <summary>
/// ������Ƌ��E�{�b�N�X�̔���
/// </summary>
/// <param name="frustum">������</param>
/// <param name="box">������s�����E�{�b�N�X</param>
/// <returns></returns>
bool FrustumCuller::IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingBox& box) const
{
   DirectX::BoundingBox worldAABB = box;
   return frustum.Contains(worldAABB) != DirectX::ContainmentType::DISJOINT;
}

/// <summary>
/// ������Ƌ��E���̔���
/// </summary>
/// <param name="frustum"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool FrustumCuller::IsVisible(const DirectX::BoundingFrustum& frustum, const DirectX::BoundingSphere& sphere) const
{
   //���E�������[���h��ԂŎg�p
   DirectX::BoundingSphere worldSphere = sphere;

   //������Ƌ��E���̌�������
   return frustum.Contains(worldSphere) != DirectX::ContainmentType::DISJOINT;
}




/// <summary>
/// ������𐶐�����
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
/// <returns></returns>
DirectX::BoundingFrustum FrustumCuller::CreateViewFrustum(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
   //�������錾
   DirectX::BoundingFrustum frustum;
   // �v���W�F�N�V�����s�񂩂王����𐶐�
   DirectX::BoundingFrustum::CreateFromMatrix(frustum, proj, true);
   // �r���[�s��̋t�s��Ŏ���������[���h��Ԃɕϊ�
   frustum.Transform(frustum, view.Invert());
   // ���[���h��Ԃ̎������Ԃ�
   return frustum;
}

