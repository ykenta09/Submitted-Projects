/*
	@file	Player.h
	@brief	�v���C���[�N���X
*/
#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include "Game/Character/Base/CharacterBase.h"
#include "Game/Interface/IState.h"

#include "Game/Character/Player/Stamina.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Character/Player/State/PlayerIdle.h"
#include "Game/Character/Player/State/PlayerMove.h"
#include "Game/Character/Player/State/PlayerSprint.h"
#include "Game/Character/Player/State/PlayerGoal.h"
#include "Game/Character/Player/State/PlayerTitle.h"
#include "Game/Character/Player/MouseControl.h"
#include "Game/Collision/CollisionManager.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/GameParameter.h"

// �O���錾
class CommonResources;

class Player : public CharacterBase
{
public:
   static constexpr DirectX::SimpleMath::Vector3 COLLISION_EXTENTS = { 0.5f, 1.2f, 0.5f };			//���E�{�b�N�X�̃T�C�Y
   static constexpr DirectX::SimpleMath::Vector3 COLLISION_OFFSET = { 0.0f, 0.8f, 0.0f };			//���E�{�b�N�X�̃T�C�Y
   static constexpr float MOVE_SPEED = { 6.50f };			    //�v���C���[�̈ړ����x
   static constexpr float AVOIDANCE_DISTANCE = { 20.0f };	    // ����s���̈ړ�����
   static constexpr float AVOIDANCE_DURATION = { 0.5f };	    //����s������
   static constexpr float STAMINA_GAUGE = { 1.0f };			    //�X�^�~�i�Q�[�W
   static constexpr float SPRINT_COOLTIME = { 0.5f };			//�X�v�����g�N�[���^�C��
public:
    //�e���擾����
	IComponent* GetParent() const { return m_parent; };	 	
	//�������W���擾����
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }	
	//���݂̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }		
	//���݂̍��W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_currentPosition = pos; }	 

	//���E�{�b�N�X���擾����
	const DirectX::BoundingBox& GetBoundingBox() { return m_collisionObject->GetCollisionParameter().collision.box; }	
	//�J�����p�̍s��𐶐�����
	DirectX::SimpleMath::Matrix GetCameraMatrix() const { return m_controlMouse->GetCameraMatrix(); }	
	//������]�p���擾����
	const float& GetInitialAngle() { return m_initialAngle; }			
	//�v���C���[�̉�]�p���擾����
	const float& GetAngle() { return m_currentAngle; }		
	//�v���C���[�̉�]�p���擾����
	const float& GetCameraAngle() { return m_cameraAngle; }		
	//�v���C���[�̉�]�p��ݒ肷��
	const void SetAngle(const float angle) { m_currentAngle = angle; }	
	//�J�����̃s�b�`��ݒ肷��
	void SetCameraPitch(const float& angle) { m_controlMouse->SetYaw(angle); }
	//�Փ˃t���O���擾����
	bool GetHit() { return m_isHit; }	

	//�ړ��������擾����
	const float& GetMoveAngle() { return m_nextMoveAngle; }	
	//�ړ�������ݒ肷��
	void SetMoveAngle(const float angle) { m_nextMoveAngle = angle; }	

	//���������Ă��邩
	bool HasKey() const { return m_hasKey; }				    

	//�X�^�[�g�C�x���g�t���O��ݒ肷��(���u���֐�)
	void SetStartEvent(bool flag) { m_isStartEvent = flag; }	
	//�S�[���������̃t���O��ݒ肷��i���u���֐��j
	void SetGoalEvent(bool flag) { m_isGoal = flag; }		    
	//�S�[���C�x���g���I���������ǂ����i���u���ϐ��j
	void SetEndGaolEvent(bool flag) { m_isGoalEvent = flag; }	
	//�S�[���C�x���g���I���������擾����(���u���ϐ�)
	bool GetEndGaolEvent() { return m_isGoalEvent; }		    

	Player* AsPlayer() { return this; }									//�v���C���[�̃|�C���^���擾����
	IState* GetCurrentState() const { return m_currentState; }			//�v���C���[�̌��݂̏�Ԃ��擾����
	PlayerIdle* GetPlayerIdle() { return m_playerIdle.get(); }			//�v���C���[�̃A�C�h����Ԃ��擾����
	PlayerMove* GetPlayerMove() { return m_playerMove.get(); }			//�v���C���[�̈ړ���Ԃ��擾����
	PlayerSprint* GetPlayerSprint() { return m_playerSprint.get(); }	//�v���C���[�̉����Ԃ��擾����
	PlayerGoal* GetPlayerGoal() { return m_playerGoal.get(); }			//�v���C���[�S�[����Ԃ��擾����
	PlayerTitle* GetPlayerTitle() { return m_playerTitle.get(); }		//�v���C���[�̃^�C�g����Ԃ��擾����
	MouseControl* GetMouseControl() { return m_controlMouse.get(); }	//�}�E�X�R���g���[�����擾����
	Stamina* GetStamina() const { return m_stamina.get(); }				//�X�^�~�i���擾����
public:
	//�R���X�g���N�^
	Player(IComponent* parent, CommonResources* resources, const GameParameters::CharacterData data, const int& stageNumber);			 
	~Player();	//�f�X�g���N�^
	void Initialize();	//����������
	//�X�V����
	void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle,	const DirectX::SimpleMath::Vector3& target);
	//�`�悷��
	inline void Render();
	//�㏈������
	void Finalize();	   

	//�����蔻��̏�����
	void InitializeCollision();	
	//�Q�[���f�[�^��ݒ肷��
	void SetGameData();		    
	//�Փˎ��̏���
	void HandleCollision(const CollisionObject* other);	
	//�L�[�{�[�h�̒ʒm������
	void OnKeyPressed(const DirectX::Keyboard::Keys& keys);	
	//�Փ˂����Ƃ��ɉ����o������
	void PushPosition(const DirectX::SimpleMath::Vector3& position);	
	//���݂̏�Ԃ�ύX����
	void ChangeState(IState* newState);	
	//�������Ă��錮���g�p����
	void UseKey();		 
private:
   //���f������K�p����
   void ApplyModelAndTexture();
private:
    CommonResources* m_commonResources;					 // ���ʃ��\�[�X
	Player* m_player;									 //�v���C���[���g
	IState* m_currentState;							     //�v���C���[�̏��

	//�Փ˔���̃p�����[�^
	CollisionObject* m_collisionObject;

	std::unique_ptr<PlayerIdle> m_playerIdle;			 //�v���C���[�A�C�h�����
	std::unique_ptr<PlayerMove> m_playerMove;			 // �v���C���[�ړ����
	std::unique_ptr<PlayerSprint> m_playerSprint;		 // �v���C���[������
	std::unique_ptr<PlayerTitle> m_playerTitle;			 //�v���C���[�^�C�g�����(�����������̂��߃S�[����ԂƓ�������)
	std::unique_ptr<PlayerGoal> m_playerGoal;			 //�v���C���[�S�[���ړ�����(�����������̂��߃^�C�g����ԂƓ�������)
	std::unique_ptr<Stamina> m_stamina;					 //�X�^�~�i
	std::unique_ptr<MouseControl> m_controlMouse;		 //�}�E�X�̃R���g���[��

	DirectX::SimpleMath::Quaternion m_rotationPlayerAngle;		  // ��]�p
	DirectX::SimpleMath::Quaternion m_rotationPlayerMoveAngle;	  // �ړ������̉�]�p

	float m_nextMoveAngle;				   //�ړ����̉�]�p
	float m_cameraAngle;			   //�J�����̉�]�p
	DirectX::SimpleMath::Vector3 m_playerVelocity;	//�v���C���[�̃x�N�g��
	DirectX::SimpleMath::Vector3 m_previousVector;	//�v���C���[�̑O��̈ړ����̃x�N�g��
	
	float m_sprintCoolTime;	//�X�v�����g�N�[���^�C��

	bool m_isHit;			  //�G�Ƃ̏Փ˃t���O

	bool m_hasKey;			  //���������Ă��邩
	int m_keyNum;			  //�������Ă��錮�̐�

	bool m_isStartEvent;	  //�X�^�[�g�C�x���g�����ǂ���
	bool m_isGoal;			  //�S�[��������
	bool m_isGoalEvent;		  //�S�[���C�x���g���I������
};
#endif // PLAYER_DEFINED
