/*
	@file	Fellow.h
	@brief	���ԃN���X
*/
#ifndef FELLOW_DEFINED
#define FELLOW_DEFINED
#pragma once
#include "Game/Interface/IState.h"
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Character/Fellow/State/FellowIdle.h"
#include "Game/Character/Fellow/State/FellowChasing.h"
#include "Game/Character/Fellow/State/FellowRouteSearch.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Libraries/Params/GameParameter.h"
#include "Game/Astar/MultiStageAStar.h"

#include "Game/Map/Map.h"

// �O���錾
class CommonResources;

class Fellow : public CharacterBase
{
public:
   static constexpr float MOVE_SPEED = { 6.50f };			   //���Ԃ̈ړ����x
   static constexpr float STOPPING_DISTANCE = { 2.3f };		   //���Ԃ̒ǂ�������̂���߂鋗��
public:
    //�������W���擾����
    DirectX::SimpleMath::Vector3 GetInitialPosition() { return m_initialPosition; }	
	//���݂̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }		
	//���݂̍��W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_currentPosition = pos; }	

	//���݂̉�]�p���擾����
	const float& GetAngle() { return m_currentAngle; };
	//���݂̉�]�p��ݒ肷��
	void SetAngle(const float& angle) { m_currentAngle = angle; }	
	//�X�P�[���擾����
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }	
	//�����o������
	void SetPushPos(const DirectX::SimpleMath::Vector3& push) { m_currentPosition += push; }	
	//�������Ƃ��ł��邩�ǂ���
	bool GetFellowMove() { return m_fellow->GetFellowIdle()->GetFellowMove(); }

	//���g�̃|�C���^���擾����
	Fellow* AsFellow()  { return m_fellow; }
	//���݂̏�Ԃ��擾����
	IState* GetCurrentState() const { return m_currentState; }		
	//�A�C�h����Ԃ��擾����
	FellowIdle* GetFellowIdle() { return m_fellowIdle.get(); }				
	//�T����Ԃ��擾����
	FellowRouteSearch* GetFellowRouteSearch() const { return m_fellowSearch.get(); }	
	//�ǐՏ�Ԃ��擾����
	FellowChasing* GetFellowChasing() { return m_fellowChasing.get(); }		

	//�E�F�C�|�C���g���擾����
	int GetWayPointIndex() const { return m_wayPointIndex; }							
	//�E�F�C�|�C���g��ݒ肷��
	void SetWayPointIndex(const int& wayPointIndex) { m_wayPointIndex = wayPointIndex; }	

	//�^�[�Q�b�g���W���擾����
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_targetPosition; }

	// �X�e�[�W�Ԍo�H���Z�b�g
	void SetMultiStagePath(const std::vector<GameParameters::MultiStageNode>& path) { m_multiStagePath = path; }
	const std::vector<GameParameters::MultiStageNode>& GetMultiStagePath() const { return m_multiStagePath; }
	//AStar���擾����
	MultiStageAStar* GetMultiStageAStar() const { return m_multiStageAStar.get(); }
public:
	//�R���X�g���N�^
	Fellow(
	   IComponent* parent,									   //�e
	   CommonResources* resources,							   //���ʃ��\�[�X
	   Map* map,											   //�}�b�v�|�C���^
	   const GameParameters::CharacterData data,
	   const int& stageNumber);			   //�X�P�[��
	//�f�X�g���N�^
	~Fellow();
	void Initialize();
	void Update(
		const float& elapsedTime,							//�o�ߎ���
		const DirectX::SimpleMath::Vector3& position,		//���݂̍��W
		const float& angle,									//���݂̉�]�p
		const DirectX::SimpleMath::Vector3& target);		//�ڕW�̍��W
	void Render();
	void Finalize();

	void InitializeCollision();											//�����蔻��̏�����
	void HandleCollision(const CollisionObject* other);					//�Փˎ��̏���
	void ChangeState(IState* newState);									//��Ԃ�ύX����
	void PushPosition(const DirectX::SimpleMath::Vector3& position);	//�����o������
private:
   //���f������K�p����
   void ApplyModelAndTexture();
private:
    CommonResources* m_commonResources;					 // ���ʃ��\�[�X
	Fellow* m_fellow;	//���g�̃|�C���^

	IState* m_currentState;								//���݂̏��
	std::unique_ptr<FellowIdle> m_fellowIdle;			//���ԑҋ@���
	std::unique_ptr<FellowChasing> m_fellowChasing;		// ���ԒǐՏ��
	std::unique_ptr<FellowRouteSearch> m_fellowSearch;	//���Ԃ̒T�����
	std::unique_ptr<MultiStageAStar> m_multiStageAStar;	//ASar

	std::vector<GameParameters::MultiStageNode> m_multiStagePath; // �X�e�[�W�ԑΉ��̌o�H

	//�Փ˔���
	CollisionObject* m_collisionObject;	//�Փ˔���̃p�����[�^

	std::vector<int> m_shortestPath;	//�ŒZ�o�H�̔ԍ��z��
	int m_wayPointIndex;	//�E�F�C�|�C���g
	float m_searchTime;	//�T���܂ł̎��Ԃ��Ǘ�

	Map* m_map;	//�}�b�v
};
#endif // FELLOW_DEFINED
