/*
	@file	CharacterBase.h
	@brief	�L�����N�^�[�x�[�X�N���X
*/
#pragma once
#ifndef CHARACTER_BASE_DEFINED
#define CHARACTER_BASE_DEFINED

#include "Game/Interface/IComposite.h"
#include "Game/Interface/IState.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Shadow/Shadow.h"

// �O���錾
class CommonResources;

class CharacterBase : public IComposite
{
public:
   // �p�[�c�z�u�p�萔
   static constexpr DirectX::SimpleMath::Vector3 BODY_POSITION = { 0.0f, 0.75f,0.0f };			   //���̂̍��W
   static constexpr DirectX::SimpleMath::Vector3 HEAD_POSITION = { 0.0f, 0.75f,0.0f };			   //���̍��W
   static constexpr DirectX::SimpleMath::Vector3 LEFT_ARM_POSITION = { -0.3f, 1.1f,0.0f };		   //���r�̍��W
   static constexpr DirectX::SimpleMath::Vector3 RIGHT_ARM_POSITION = { 0.3f, 1.1f,0.0f };		   //�E�r�̍��W
   static constexpr DirectX::SimpleMath::Vector3 LEFT_FOOT_POSITION = { -0.25f, 0.5f,0.0f };	   //�����̍��W
   static constexpr DirectX::SimpleMath::Vector3 RIGHT_FOOT_POSITION = { 0.25f, 0.5f,0.0f };	   //�E���̍��W
   static constexpr float PARTS_ROT_DEG = DirectX::XMConvertToRadians(6.0f);   // �p�[�c������]�p�x�i�x�j
   static constexpr float ROTATE_LERP_SPEED = 10.0f; // ��]��ԑ��x
   static constexpr float SHADOW_Y = -0.4f;  // �e�̕`��Y���W
   static constexpr float GRAVITY_SCALE = 0.01f;  // �d�͂̃X�P�[��
public:
    //�L�����N�^�[�^�C�v
    enum class CharacterType
    {
	   Player = 0,
	   Fellow,
	   Enemy
	};
	//�p�[�cID
	enum PartID : int
	{
		Character = -1,
		HEAD = 0,
		BODY = 1,
		LEFT_ARM = 2,
		RIGHT_ARM = 3,
		LEFT_FOOT = 4,
		RIGHT_FOOT = 5
	};
	//�p�[�c���f��
	struct CharacterResources
	{
	   DirectX::Model* head;									  //���̃��f��
	   DirectX::Model* body;									  //���̂̃��f��  
	   DirectX::Model* arm;										  //�r�̃��f��
	   DirectX::Model* foot;									  //���̃��f��
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> headDDS;  //���̃e�N�X�`��
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bodyDDS;  //���̂̃e�N�X�`��
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> armDDS;	  //�r�̃e�N�X�`��
	   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> footDDS;  //���̃e�N�X�`��
	};
	
public:
   //�m�[�h�J�E���g�A�b�v������m�[�h�J�E���g���擾����
   static int GetNodeCountAfterCountUp() { return s_nodeCount++; }
   //�m�[�h�J�E���g���擾����
   static int GetNodeCount() { return s_nodeCount; }

   // �p�[�c�ԍ������Z�b�g����(-1����J�n����)
   static void ResetPartsNumber() { s_partsNumber = -1; }
   // �v���C���[�p�[�c�ԍ����擾����
   static int GetPartsNumber() { return s_partsNumber; }
   // �v���C���[�p�[�c�ԍ����₷
   static void IncrementPartsNumber() { s_partsNumber++; }
public:
	//�e���擾����
	IComponent* GetParent() const { return m_parent; };
	//�m�[�h�ԍ����擾����
	int GetNodeNumber() const { return m_nodeNumber; }
	//�p�[�cID���擾����
	int GetPartID() const { return m_partID; }
	//�p�[�c�ԍ����擾����
	int GetPartNumber() const { return m_partNumber; }
	DirectX::SimpleMath::Vector3 GetInitialPosition() const { return m_initialPosition; }
	//���݂̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPosition() const  { return m_currentPosition; }
	//���݂̍��W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)  { m_currentPosition = pos; }
	//���݂̉�]�p���擾����
	float GetAngle() const { return m_currentAngle; };
	//���݂̉�]�p��ݒ肷��
	void SetAngle(const float& angle) { m_currentAngle = angle; }
	IComponent* GetLastAttachedNode() const { return nullptr; }

	//�I�u�W�F�N�g�̃X�e�[�W�ԍ����擾����
	int GetObjectStageNumber() const { return m_objectStageNumber; }
	void SetObjectStageNumber(const int& number) { m_objectStageNumber = number; }

	//�L�����N�^�[�̃|�C���^���擾����
	Player* AsPlayer() override { return nullptr; }
	Enemy* AsEnemy() override { return nullptr; }
	Fellow* AsFellow() override { return nullptr; }

	//�X�P�[���擾����
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	//���f�����擾����
	DirectX::Model* GetModel() const { return NULL; }
	//���f����ݒ肷��
	void SetPartModels(const CharacterResources& resources);

	//�v���C���[�̉�]�p���擾����
	const float& GetAngle()  { return m_angleY; }
	//�v���C���[�̉�]�p��ݒ肷��
	const void SetAngle(const float angle) { m_angleY = angle; }
	const float& GetMoveAngle() { return m_moveAngle; }

	DirectX::SimpleMath::Vector3 GetInitialPosition() { return m_initialPosition; }
	//�ړ��������󂯎��
	void SetMoveAngle(const float angle) { m_moveAngle = angle; }

	//�L�����N�^�[�^�C�v���擾����
	CharacterType GetCharacterType() const { return m_characterType; }
	//�L�����N�^�[�^�C�v��ݒ肷��
	void SetCharacterType(CharacterType type) { m_characterType = type; }
public:
	//�R���X�g���N�^
	CharacterBase(
		IComponent* parent,
		CommonResources* resources,
		const DirectX::SimpleMath::Vector3& initialPosition, 
		const float& initialAngle,
		const DirectX::SimpleMath::Vector3& scale);
	//�f�X�g���N�^
	~CharacterBase();
	//����������
	void Initialize();
	//�X�V����
	void Update(const float& elapsedTime,
		const DirectX::SimpleMath::Vector3& position,
		const float& angle,
		const DirectX::SimpleMath::Vector3& target);
	//�v���C���[�̃p�[�c��ǉ�����
	void Attach(std::unique_ptr<IComponent> playerParts) ;
	//�v���C���[�̃p�[�c���폜����
	void Detach(std::unique_ptr<IComponent> playerParts) ;
	//�`�悷��
	inline void Render() ;
	//�㏈������
	void Finalize() ;

	//�L�[�{�[�h�̒ʒm������
	inline void OnKeyPressed(const DirectX::Keyboard::Keys& keys) ;
	//�Փ˂����Ƃ��ɉ����o������
	void PushPosition(const DirectX::SimpleMath::Vector3& position);
	//������]������
	void LostInitialRotation();
private:
	   static int s_nodeCount;		 //�m�[�h��
	   static int s_partsNumber;	//�p�[�c��
protected:
   yamadalib::Graphics* m_graphics;	   //�O���t�B�b�N�X
   IComponent* m_parent;			   //�e
   CommonResources* m_commonResources; // ���ʃ��\�[�X

   std::vector<std::unique_ptr<IComponent>> m_characterParts;	//�p�[�c�z��
   std::unique_ptr<Shadow> m_shadow;	//�e
   CharacterResources m_modelResources;	//���f�����\�[�X
   CharacterType m_characterType;   //�L�����N�^�[�^�C�v

   DirectX::SimpleMath::Vector3 m_initialPosition;	  //�������W
   DirectX::SimpleMath::Vector3 m_currentPosition;	  //���݂̍��W
   float m_initialAngle;							  //������]�p
   float m_currentAngle;							  //���݂̉�]
   DirectX::SimpleMath::Vector3 m_scale;   //�X�P�[��

   DirectX::SimpleMath::Vector3 m_targetPosition;   //�^�[�Q�b�g�̍��W

   bool m_applyInitialRotation;   //������]��K�p���邩�̃t���O

   int m_nodeNumber;	//�m�[�h�ԍ�
   int m_partNumber;	//�p�[�c�ԍ�
   int m_partID;		//�p�[�cID

   float m_angleY;		// ��]�p
   float m_moveAngle;	//�ړ����̉�]�p

   float m_time;		//�o�ߎ���
   int m_objectStageNumber;	 //�I�u�W�F�N�g�����݂���X�e�[�W�ԍ�
};
#endif // CHARACTER_BASE_DEFINED
