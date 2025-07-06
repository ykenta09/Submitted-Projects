/*
	@file	EnemyVisibility.h
	@brief	�G�̎��E�N���X
*/
#pragma once
#ifndef ENEMY_VISIBILITY_DEFINED
#define ENEMY_VISIBILITY_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Enemy/Enemy.h"

// �O���錾
class CommonResources;
class Map;

class EnemyVisibility
{
public: 
   static constexpr float FIELD_OF_VIEW = { 120.0f };		   //���E�̍L��
   static constexpr float VISIBILITIY_DISTANCE = { 12.0f };	   //���E�̋���
   static constexpr int  SEGMENTS = { 16 };					   //���E�̍L��
public:
   //�R���X�g���N�^
   EnemyVisibility(Enemy* enemy);
   //�f�X�g���N�^
   ~EnemyVisibility();
   //����������
	void Initialize();
	//�X�V����
	void Update();

	//���E��`�悷��
	void DrawVisibility(Map* map);
	// ���E�̊p�x�v�Z
	void CalculateAngles(int segmentIndex, float& currentAngleStart, float& currentAngleEnd);
	// ���E�̌����Ƒ傫���̌v�Z
	DirectX::SimpleMath::Vector2 CalculateDirection();
	// ���E�̃x�N�g���v�Z
	void CalculateVectors(float currentAngleStart, float currentAngleEnd, const DirectX::SimpleMath::Vector2& direction, DirectX::SimpleMath::Vector2& m_vectorV2, DirectX::SimpleMath::Vector2& m_vectorV3);
	void DrawViewLines(const DirectX::SimpleMath::Vector2& m_vectorV2, const DirectX::SimpleMath::Vector2& m_vectorV3);
	// �O�p�`�̒��_�ݒ�
	void SetTriangleVertices(Map* map, const DirectX::SimpleMath::Vector2& vectorV2, const DirectX::SimpleMath::Vector2& vectorV3, bool& pointFlag1, bool& pointFlag2);
	// �I�u�W�F�N�g�̃R�[�i�[���v�Z
	void CalculateObjectCorners(const DirectX::BoundingBox& object, DirectX::SimpleMath::Vector2 corners[4]);
	// ��������ƎO�p�`�̒��_�ݒ�
	void CheckIntersection(const DirectX::SimpleMath::Vector2& start, const DirectX::SimpleMath::Vector2& end, const DirectX::SimpleMath::Vector2& objectV1, const DirectX::SimpleMath::Vector2& objectV2, DirectX::SimpleMath::Vector2& trianglePoint, bool& pointFlag);
	// ���E�̐�^��`��
	void DrawSector(const DirectX::XMVECTOR& color);
private:
	yamadalib::Graphics* m_graphics;	//�O���t�B�b�N�X
	Enemy* m_enemy;					   //�G

	bool m_isViewHit;	   //���E���ɓ����Ă��邩
	float m_angle;		   //�G�̐��ʊp�x
	float m_length;		   //���E�̍ő勗��

	// === ���E�̍��W ===
	DirectX::SimpleMath::Vector2 m_vectorV1;	//���E�̒��S���W
	DirectX::SimpleMath::Vector2 m_vectorV2;	//���E�̍��[���W
	DirectX::SimpleMath::Vector2 m_vectorV3;	//���E�̉E�[���W

	// === ��_ ===
	DirectX::SimpleMath::Vector2 m_intersectionPoint1;		//���[�̏�Q���Ƃ̌�_
	DirectX::SimpleMath::Vector2 m_intersectionPoint2;		//�E�[�̏�Q���Ƃ̌�_
};
#endif // ENEMY_VISIBILITY_DEFINED
