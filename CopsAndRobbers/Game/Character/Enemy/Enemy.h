/*
	@file	Enemy.h
	@brief	�G�N���X
*/
#ifndef ENEMY_DEFINED
#define ENEMY_DEFINED
#pragma once
#include "Game/Character/Base/CharacterBase.h"
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Enemy/State/EnemyChasing.h"
#include "Game/Character/Enemy/State/EnemyWaiting.h"
#include "Game/Character/Enemy/State/EnemyWandering.h"
#include "Game/Character/Enemy/State/EnemyWanderingReturn.h"
#include "Game/Character/Enemy/State/EnemyTransmission.h"
#include "Game/Character/Enemy/EnemyVisibility.h"
#include "Game/Character/Enemy/State/EnemyRouteSearch.h"
#include "Game/Character/Enemy/State/EnemyTitle.h"
#include "Game/Astar/AStar.h"
#include "Game/Map/Map.h"
#include <unordered_map>
#include "Game/Astar/MultiStageAStar.h"
#include <GraphEditor/Graph.h>

// �O���錾
class CommonResources;
class GraphScene;
class EnemyVisibility;

class Enemy : public CharacterBase
{
public:
   static constexpr float MOVE_SPEED = { 6.60f };			//�G�̈ړ����x
   static constexpr float STOPPING_DISTANCE = { 1.5f };		//�G�̒ǂ�������̂���߂鋗��
   static constexpr float WANDER_INTERVAL = { 3.0f };		//���񃋁[�g�ς���Ԋu
   static constexpr float WAIT_INTERVAL = { 2.0f };			//�ҋ@����
   static constexpr float TIME_TO_SEARCH = { 0.5f };		//�o�H�T���܂ł̎���
public:
   //���݂̍��W���擾����
   DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }	
   //���݂̍��W��ݒ肷��
   void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_currentPosition = pos; }   

   //�G�̏�����]�p���擾����
   const float& GetInitialAngle() { return m_initialAngle; }   

   //�G�̉�]�p���擾����
   const float& GetAngle() { return m_currentAngle; }	
   //�G�̉�]�p��ݒ肷��
   const void SetAngle(const float angle) { m_currentAngle = angle; }  
   //�G�̎��̈ړ�������ݒ肷��
   const void SetNextAngle(const float angle) { m_nextAngle = angle; } 
   //�G�̎��̈ړ��������擾����
   const float& GetNextAngle() { return m_nextAngle; }					

   //�����o������
   void SetPushPos(const DirectX::SimpleMath::Vector3& push) { m_currentPosition += push;}   

   //�������W���擾����
   DirectX::SimpleMath::Vector3 GetInitialPosition() { return m_initialPosition; }   

   //�G�ԍ���ݒ肷��
   void SetEnemyNumber(const int& number) { m_enemyNumber = number; }   
   //�G�ԍ����擾����
   const int& GetEnemyNumber() { return m_enemyNumber; }				

   //�^�[�Q�b�g�̕������擾����
   DirectX::SimpleMath::Vector3 GetTargetDirection() { return m_targetDirection; }

   //�ŒZ�o�H���N���A����
   void ResetShortestPath() { m_shortestPath.clear(); }				
   //�ŒZ�o�H���擾����
   std::vector<int> GetShortestPath() const { return m_shortestPath; }   
   //�ŒZ�o�H��ݒ肷��
   void SetShortestPath(const std::vector<int>& shortestPath) { m_shortestPath = shortestPath; }  
   // �E�F�C�|�C���g���擾����
   int GetWayPointIndex() const { return m_wayPointIndex; }				
   // �E�F�C�|�C���g��ݒ肷��
   void SetWayPointIndex(const int& wayPointIndex) { m_wayPointIndex = wayPointIndex; }   

   // �X�e�[�W�Ԍo�H���Z�b�g
   void SetMultiStagePath(const std::vector<GameParameters::MultiStageNode>& path) { m_multiStagePath = path; }
   const std::vector<GameParameters::MultiStageNode>& GetMultiStagePath() const { return m_multiStagePath; }
   //AStar���擾����
   MultiStageAStar* GetMultiStageAStar() const { return m_multiStageAStar.get(); }


   //���݂̏�Ԃ��擾����
   IState* GetCurrentState() const { return m_currentState; }					
   //�G�̜p�j��Ԃ��擾����
   EnemyWandering* GetEnemyWandering() const { return m_enemyWandering.get(); }					    
   //�G�̜p�j�ɖ߂��Ԃ��擾����
   EnemyWanderingReturn* GetEnemyWanderingReturn() const { return m_enemyWanderingReturn.get(); }   
   //�G�̒ǐՏ�Ԃ��擾����
   EnemyChasing* GetEnemyChasing() const { return m_enemyChasing.get(); }						   
   //�G�̑ҋ@��Ԃ��擾����
   EnemyWaiting* GetEnemyWaiting() const { return m_enemyWaiting.get(); }					
   //�G�̌o�H�T����Ԃ��擾����
   EnemyRouteSearch* GetEnemyRouteSearch() const { return m_enemyRouteSearch.get(); }	
   //�G�̃^�C�g����Ԃ��擾����
   EnemyTitle* GetEnemyTitle() const { return m_enemyTitle.get(); }								   
   //���g���擾����
   Enemy* AsEnemy()  { return m_enemy; }														  
public:
	//�L�[�{�[�h�̒ʒm������
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys);
	void PushPosition(const DirectX::SimpleMath::Vector3& position);
public:
	//�R���X�g���N�^
	Enemy(
		IComponent* parent,						
		CommonResources* resources,
	    Map* map,
	    const GameParameters::EnemyData data,
	    const int& stageNumber);
	//�f�X�g���N�^
	~Enemy();

	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target);
	//�`�悷��
	void Render();
	//��n������
	void Finalize();
	//�����蔻��̏�����
	void InitializeCollision();
	//�Փˎ��̏���
	void HandleCollision(const CollisionObject* other);
	//�������
	bool InView(bool object);
	//���C�ƃ}�b�v�I�u�W�F�N�g�̔���
	bool RayMapObject();
	//��Ԃ�ύX����
	void ChangeState(IState* newState);
	//���G
	void Search();
private:
   //���f������K�p����
   void ApplyModelAndTexture();
private:
   // === �X�e�[�g�Ǘ�  ===
	Enemy* m_enemy;													//���g�̃|�C���^
	IState* m_currentState;											//���݂̏��
	std::unique_ptr<EnemyWaiting> m_enemyWaiting;					//�G�̑ҋ@���
	std::unique_ptr<EnemyWandering> m_enemyWandering;				//�G�̜p�j���
	std::unique_ptr<EnemyWanderingReturn> m_enemyWanderingReturn;	//�p�j�ɖ߂���
	std::unique_ptr<EnemyChasing> m_enemyChasing;					//�G�̒ǐՏ��
	std::unique_ptr<EnemyRouteSearch> m_enemyRouteSearch;			//�G�̌o�H�T�����
	std::unique_ptr<EnemyVisibility> m_visibility;				    //�G�̎��E
	std::unique_ptr<EnemyTitle> m_enemyTitle;					    //�G�̃^�C�g�����
	std::unique_ptr<MultiStageAStar> m_multiStageAStar;			    //ASar

	// === �o�H�T���V�X�e�� ===
	Map* m_map;															   //�}�b�v
	std::vector<int> m_shortestPath;									   //�ŒZ�o�H
	std::vector<DirectX::SimpleMath::Vector3> m_patrolRootMap;			   //�X�e�[�W�̂��ׂĂ̏��񃋁[�g
	std::unordered_map<int, GameParameters::PatrolRoute> m_adjacencyList;  //�אڃ��X�g<�G�̔ԍ�,���_�ԍ�,�q�����Ă��钸�_�ԍ�>
	int m_wayPointIndex;												   // �E�F�C�|�C���g�C���f�b�N�X
	std::vector<GameParameters::MultiStageNode> m_multiStagePath; // �X�e�[�W�ԑΉ��̌o�H

	CollisionObject* m_collisionObject;	//�����蔻��̃p�����[�^

	// === ���E ===
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_detectionTexture;	// �����e�N�X�`��
	bool m_viewType;													    //���E�̃^�C�v

	// === ���� ===
	DirectX::SimpleMath::Vector3 m_targetDirection;	//�^�[�Q�b�g�܂ł̕����x�N�g��
	DirectX::SimpleMath::Vector3 m_enemyDirection;	//�G�̌����Ă������

	// === �ݒ�p�����[�^ ===
	int m_enemyNumber;			 //�G�ԍ�
	int m_initialVertexNumber;	 //�G�̏������_�ԍ�
	float m_nextAngle;			 //���̃t���[���Ō����p�x
	float m_searchTime;			 //�T���܂ł̎��Ԃ��Ǘ�
};
#endif // ENEMY_DEFINED
