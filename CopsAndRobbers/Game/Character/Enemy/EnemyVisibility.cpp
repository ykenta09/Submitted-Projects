/*
	@file	EnemyVisibility.cpp
	@brief	�G�̎��E�N���X
*/
#include "pch.h"
#include "Game/Character/Enemy/EnemyVisibility.h"
#include "Game/CommonResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Game/Map/Map.h"
#include <cassert>
#include <Game/Screen.h>
#include <Libraries/MyLib/Geometry.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="enemy">�G�{�̂̃|�C���^</param>
EnemyVisibility::EnemyVisibility(Enemy* enemy)
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
    m_enemy(enemy)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyVisibility::~EnemyVisibility()
{
	// do nothing.
}

/// <summary>
/// ����������
/// </summary>
void EnemyVisibility::Initialize()
{
   m_vectorV2 = DirectX::SimpleMath::Vector2(0, 0);
   m_vectorV3 = DirectX::SimpleMath::Vector2(0, 0);
   m_length = VISIBILITIY_DISTANCE;
}

/// <summary>
///  �X�V����
/// </summary>
void EnemyVisibility::Update()
{
   m_vectorV1 = DirectX::SimpleMath::Vector2(m_enemy->GetPosition().x, m_enemy->GetPosition().z);
}

/// <summary>
/// ���E��`�悷��
/// </summary>
/// <param name="map">�}�b�v�̃|�C���^</param>
void EnemyVisibility::DrawVisibility(Map* map)
{
   if (map == nullptr) return;

   //�`�揈�����o�b�`�����ł܂Ƃ߂ď���
   m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
      [this, map](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
      {
         //�G�̐��ʂ̊p�x���擾
         m_angle = DirectX::XMConvertToDegrees(-m_enemy->GetAngle());
         //��`�̐F
         const DirectX::XMVECTOR SemiTransparentRed = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.1f);

         for (int i = 0; i < SEGMENTS; i++)
         {
            float currentAngleStart, currentAngleEnd;
            // ���E�̊p�x�v�Z
            CalculateAngles(i, currentAngleStart, currentAngleEnd);

            //���ʕ����̎��E�̒����x�N�g�����擾����
            DirectX::SimpleMath::Vector2 direction = CalculateDirection();
            
            // ���E�̃x�N�g���v�Z
            CalculateVectors(currentAngleStart, currentAngleEnd, direction, m_vectorV2, m_vectorV3);

            //��Q���Ƃ̌�������p�t���O
            bool pointFlag1 = false, pointFlag2 = false;
            // �O�p�`�̒��_�ݒ�
            SetTriangleVertices(map, m_vectorV2, m_vectorV3, pointFlag1, pointFlag2);

            // ���E�̐�^��`��
            DrawSector(SemiTransparentRed);
         }
      });
}


/// <summary>
/// ���E�̊p�x�v�Z
/// </summary>
/// <param name="segmentIndex">�Z�O�����g�ԍ�</param>
/// <param name="currentAngleStart">�Z�O�����g�J�n�p�x</param>
/// <param name="currentAngleEnd">�Z�O�����g�I���p�x</param>
void EnemyVisibility::CalculateAngles(int segmentIndex, float& currentAngleStart, float& currentAngleEnd)
{
   //�P�Z�O�����g������̊p�x�����v�Z
   float angleStep = 60.0f / SEGMENTS;
   //�Z�O�����g�̎n�_�p�x���v�Z
   currentAngleStart = m_angle - (FIELD_OF_VIEW / 2.0f) + segmentIndex * angleStep;
   //�Z�O�����g�̏I�_�p�x���v�Z
   currentAngleEnd = m_angle - (FIELD_OF_VIEW / 2.0f) + (segmentIndex + 2) * angleStep;
   m_angle += angleStep;
}


/// <summary>
/// ���E�̌����Ƒ傫���̌v�Z
/// </summary>
/// <returns></returns>
DirectX::SimpleMath::Vector2 EnemyVisibility::CalculateDirection()
{
   DirectX::SimpleMath::Vector2 direction(0.0f, 1.0f);
   //���E�̒��������X�P�[�����O
   direction *= m_length;
   //��]�s��ŉ񂵂āA���ۂ̎��E��`�̊e�ӂ̌��������߂�
   return direction;
}

/// <summary>
/// ���E�̃x�N�g���v�Z
/// </summary>
/// <param name="currentAngleStart">�Z�O�����g�̊J�n�p�x</param>
/// <param name="currentAngleEnd">�Z�O�����g�̏I���p�x</param>
/// <param name="direction">���ʕ���</param>
/// <param name="vectorV2">��`�̍��[</param>
/// <param name="vectorV3">��`�̉E�[</param>
void EnemyVisibility::CalculateVectors(float currentAngleStart, float currentAngleEnd, const DirectX::SimpleMath::Vector2& direction, DirectX::SimpleMath::Vector2& vectorV2, DirectX::SimpleMath::Vector2& vectorV3)
{
   //�J�n�p�x������Z������ɉ�]����s��𐶐�
   DirectX::SimpleMath::Matrix rotation1 = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(currentAngleStart));
   //�I���p�x������Z������ɉ�]����s��𐶐�
   DirectX::SimpleMath::Matrix rotation2 = DirectX::SimpleMath::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(currentAngleEnd));

   //���ʃx�N�g�����J�n�p�x��������]���A�G�̌��݈ʒu�����_�Ƃ��ĉ��Z
   //��`�̍��[�̃��[���h���W�����߂�
   vectorV2 = DirectX::SimpleMath::Vector2::Transform(direction, rotation1) + m_vectorV1;
   //���ʃx�N�g�����I���p�x��������]���A���݈ʒu�����_�Ƃ��ĉ��Z
   //��`�̉E�[�̃��[���h���W�����߂�
   vectorV3 = DirectX::SimpleMath::Vector2::Transform(direction, rotation2) + m_vectorV1;
}

/// <summary>
///  ����V1-V2, V1-V3��`��
/// </summary>
/// <param name="vectorV2">��`�̍��[��2D���W</param>
/// <param name="vectorV3">��`�̉E�[��2D���W</param>
void EnemyVisibility::DrawViewLines(const DirectX::SimpleMath::Vector2& vectorV2, const DirectX::SimpleMath::Vector2& vectorV3)
{
   //�G�̍��W�����`���[�ւ̐�����`��
   m_graphics->DrawLine(m_vectorV1, vectorV2 - m_vectorV1, DirectX::Colors::White);
   //�G�̍��W�����`�E�[�ւ̐�����`��
   m_graphics->DrawLine(m_vectorV1, vectorV3 - m_vectorV1, DirectX::Colors::White);
}

/// <summary>
/// �O�p�`�̒��_�ݒ�
/// </summary>
/// <param name="map">�}�b�v�̃|�C���^</param>
/// <param name="vectorV2">��`�̍��[��2D���W</param>
/// <param name="vectorV3">��`�̉E�[��2D���W</param>
/// <param name="pointFlag1">���[���Ō�_�������������ǂ���</param>
/// <param name="pointFlag2">�E�[�Ō�_�������������ǂ���</param>
void EnemyVisibility::SetTriangleVertices(Map* map, const DirectX::SimpleMath::Vector2& vectorV2, const DirectX::SimpleMath::Vector2& vectorV3, bool& pointFlag1, bool& pointFlag2)
{
   //�I�u�W�F�N�g�̋��E�{�b�N�X�Ɣ�����s��
   for (const auto& object : map->GetBoundingBoxes())
   {
      DirectX::SimpleMath::Vector2 corners[4];
      // �I�u�W�F�N�g�̃R�[�i�[���v�Z
      CalculateObjectCorners(object, corners);

      //��Q����4�ӂ��y�A�Œ�`
      std::pair<DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2> edges[4] = {
         {corners[0], corners[1]},
         {corners[1], corners[2]},
         {corners[2], corners[3]},
         {corners[3], corners[0]}
      };

      //�e�ӂ��ƂɎ��E��`�̗��Ӄx�N�g���Ƃ̌���������s��
      for (const auto& edge : edges)
      {
         const DirectX::SimpleMath::Vector2& objectV1 = edge.first;
         const DirectX::SimpleMath::Vector2& objectV2 = edge.second;

         // ��������ƎO�p�`�̒��_�ݒ�
         CheckIntersection(m_vectorV1, vectorV2, objectV1, objectV2, m_intersectionPoint1, pointFlag1);
         CheckIntersection(m_vectorV1, vectorV3, objectV1, objectV2, m_intersectionPoint2, pointFlag2);
      }
   }
}

/// <summary>
/// �I�u�W�F�N�g�̃R�[�i�[���v�Z
/// </summary>
/// <param name="object">�Ώۂ̋��E�{�b�N�X</param>
/// <param name="corners">���ʂ��i�[����z��</param>
void EnemyVisibility::CalculateObjectCorners(const DirectX::BoundingBox& object, DirectX::SimpleMath::Vector2 corners[4])
{
   //���E�{�b�N�X�̒��S���W�Ɣ��a���g���āAXZ���ʏ��4���̍��W���v�Z����
   corners[0] = DirectX::SimpleMath::Vector2(object.Center.x - object.Extents.x, object.Center.z - object.Extents.z);
   corners[1] = DirectX::SimpleMath::Vector2(object.Center.x + object.Extents.x, object.Center.z - object.Extents.z);
   corners[2] = DirectX::SimpleMath::Vector2(object.Center.x + object.Extents.x, object.Center.z + object.Extents.z);
   corners[3] = DirectX::SimpleMath::Vector2(object.Center.x - object.Extents.x, object.Center.z + object.Extents.z);
}

/// <summary>
/// ��������ƎO�p�`�̒��_�ݒ�
/// </summary>
/// <param name="start">���E�̒��S�_</param>
/// <param name="end">���E�̏I�_</param>
/// <param name="objectV1">��Q���ӂ̎n�_</param>
/// <param name="objectV2">��Q���ӂ̏I�_</param>
/// <param name="trianglePoint">�O�p�`�̒��_���</param>
/// <param name="pointFlag">��_�����������ǂ���</param>
void EnemyVisibility::CheckIntersection(const DirectX::SimpleMath::Vector2& start, const DirectX::SimpleMath::Vector2& end, const DirectX::SimpleMath::Vector2& objectV1, const DirectX::SimpleMath::Vector2& objectV2, DirectX::SimpleMath::Vector2& trianglePoint, bool& pointFlag)
{
   //�����̓����蔻����s��
   if (IntersectLines2D(start, end, objectV1, objectV2))
   {
      //�������Ă���Ό�_���v�Z
      DirectX::SimpleMath::Vector2 intersectionPoint = IntersectPointLines2D(start, end, objectV1, objectV2);
      //��ԋ������Z����_�ɏ㏑������
      if ((intersectionPoint - m_vectorV1).Length() < (trianglePoint - m_vectorV1).Length())
      {
         trianglePoint = intersectionPoint;
         pointFlag = true;
      }
   }
   else if (!pointFlag)
   {
      //��_��������Ȃ�����
      trianglePoint = end;
   }
}


/// <summary>
/// ���E�̐�^��`��
/// </summary>
/// <param name="color">�`�悷��F</param>
void EnemyVisibility::DrawSector(const DirectX::XMVECTOR& color)
{
   //��`��`�悷��
   m_graphics->DrawFilledSector(*m_graphics->GetPrimitiveBatch(),
      DirectX::SimpleMath::Vector3(m_enemy->GetPosition().x, 0.01f, m_enemy->GetPosition().z),
      color,
      m_intersectionPoint1,
      m_intersectionPoint2);
}

