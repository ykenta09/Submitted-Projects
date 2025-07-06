#pragma once
#ifndef GRAPH_SCENE_DEFINED
#define GRAPH_SCENE_DEFINED
#include "DeviceResources.h"
//#include "ArcBall.h"
#include "Game.h"
#include "Libraries/MyLib/SpriteString3D.h"
#include "Graph.h"	

class CommonResources;

class GraphScene
{
	// �G���[�C���f�b�N�X
	static const int ERROR_INDEX = -1;
	static const float VERTEX_RADIUS;
	const bool ON = true;		
	const bool OFF = false;		

public:
	bool GetEditMode() { return m_editMode; }	
public:
	// �R���X�g���N�^
	GraphScene(CommonResources* resources);
	// �f�X�g���N�^
	~GraphScene();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float timer);
	// �`�悷��
	void Render();
	// �㏈�����s��
	void Finalize();
	//�O���t��ǂݍ���(�ҏW���[�h�p)
	void LoadGraphData();
	//���_���X�g���擾����
	const std::vector<Vertex>& GetVertexList() { return m_graph->GetVertexList(); }
	//���񃋁[�g���擾����
	const std::vector<DirectX::SimpleMath::Vector3>& GetPatrolRootMap() { return m_graph->GetPatrolRootMap(); }
	//�אڃ��X�g���擾����
	const std::unordered_map<int , Graph::PatrolRoute>& GetAdjacencyList() { return m_graph->GetAdjacencyList(); };
	//�X�e�[�W�̓G�̍ő吔���擾����(�g���ǂ��낪�Ȃ���΍폜����֐�)
	int GetMaxEnemyIndex();
	//�X�e�[�W�̓G�̍ő吔���擾����
	int GetEnemyIndex() { return m_enemyIndex; }
	//�G�̏������_�ԍ����擾����
	int GetInitalVertexNumbeer(const int& number) { return m_graph->GetInitailVertex(number); }
private:
	// �O���b�h��`�悷��
	void DrawGrid(
		DirectX::FXMVECTOR xAxis, 
		DirectX::FXMVECTOR yAxis, 
		DirectX::FXMVECTOR origin, 
		size_t xdivs, 
		size_t ydivs, 
		DirectX::GXMVECTOR m_color
	);
	// �v���W�F�N�V�����𐶐�����
	void CreateProjection();

	//�O���t��ۑ�����
	void SaveGraphData();

	// SpriteString3D�N���X�̃C���X�^���X�ɂ��ׂĂ̒��_�ԍ���ǉ�����
	void AddAllVertexNumberToSpriteString3D();
	// �O���t��`�悷��
	void DrawGraph();

	// ���_�܂��̓G�b�W��ǉ�����
	void AddVertexOrEdge();
	// ���_���X�g�Ɏw�肳�ꂽ���_�x�N�g�������݂��邩�ǂ����𒲂ׂ�
	int ExistingVertexInVertexList(const DirectX::SimpleMath::Vector3& vertexVector);
	// SpriteString3D�N���X�̃C���X�^���X�ɒ��_�ԍ���ǉ�����
	void AddVertexNumberToSpriteString3D(const int& vertexNumber);
	// �}�E�X�J�[�\���ʒu����X�N���[���ւ̌������v�Z����
	DirectX::SimpleMath::Ray CreateRayFromScreenPoint(const DirectX::SimpleMath::Vector2& mousePosition);

	// ���_�܂��̓G�b�W���폜����
	void RemoveVertexOrEdge();
	// �G�b�W���폜����
	void RemoveEdge(int number, const DirectX::SimpleMath::Vector3& vertexVector);

	// �p�X���𕪗�����(20231120)
	void SplitPath(wchar_t* modelName);
	//���ׂăN���A����
	void ClearAll();

	//�G�̗אڃ��X�g��؂�ւ���
	void ChangeEnemyAdjacencyList(bool increaseNumbers);
	//���݂̓G�ԍ��̗אڃ��X�g���폜����
	void ClearEnemyAdjacencyList();
	//�V�����G�̌o�H�ǉ�����
	void NewEnemyAdjacencyList();
	//�G�̏������_�ԍ����쐬����
	void InitalVertexNumber();
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// DirectXGraphics�N���X�̃C���X�^���X�ւ̃|�C���^
	yamadalib::Graphics* m_graphics;
	// Device�N���X�̃C���X�^���X�ւ̃|�C���^
	ID3D11Device* m_device;
	// DeviceContext�N���X�̃C���X�^���X�ւ̃|�C���^
	ID3D11DeviceContext* m_context;
	// �}�E�X�J�[�\���ʒu
	DirectX::SimpleMath::Vector2 m_mouseCursorPosition;
	// �L�[�{�[�h�X�e�[�g
	DirectX::Keyboard::State m_keyboardState;
	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	// �}�E�X�X�e�[�g
	DirectX::Mouse::Mouse::State m_mouseState;
	// �}�E�X�X�e�[�g�g���b�J�[
	DirectX::Mouse::ButtonStateTracker m_mouseStateTracker;

	// ���[���h
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �r���[
	DirectX::SimpleMath::Matrix m_viewMatrix;
	// �v���W�F�N�V����
	DirectX::SimpleMath::Matrix m_projectionMatrix;
	// �J������]
	DirectX::SimpleMath::Quaternion m_cameraRotation;
	// �J�����ʒu
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// �J�����t�H�[�J�X
	DirectX::SimpleMath::Vector3 m_cameraFocus;
	// �p�x
	float m_rotaionAngle;
	// �����_���王�_�܂ł̋���
	float m_distance;
	// �Y�[��
	float m_zoom;
	// �t�B�[���h�I�u�r���[
	float m_fov;
	// �j�A�v���[��
	float m_nearPlane;
	// �t�@�[�v���[��
	float m_farPlane;

	// �O���t
	std::unique_ptr<Graph> m_graph;
	// �X�v���C�g�X�g�����O3D
	std::unique_ptr<SpriteString3D> m_spriteString3D;

	// ���_
	DirectX::SimpleMath::Vector3 m_vertex;
	// ���_�I�_�C���f�b�N�X
	int m_vertexEndIndex;
	// ���_�n�_�C���f�b�N�X
	int m_vertexStartIndex;
	// �G�b�W�ҏW���[�h
	bool m_edgeMode;

	// �h���C�u��
	wchar_t m_drive[_MAX_DRIVE];
	// �p�X��
	wchar_t m_path[MAX_PATH];
	// �t�@�C����
	wchar_t m_fileName[_MAX_FNAME];
	// �g���q
	wchar_t m_extension[_MAX_EXT];
	// �X�P�[��
	float m_scale;
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	//�ҏW���[�h
	bool m_editMode;
	//�������_�ԍ��쐬���[�h
	bool m_initialEditMode;
	//�X�e�[�W�̓G�̐�
	int m_enemyIndex;
	//�G�̔ԍ�
	int m_enemyNumber;
};

#endif		// GRAPH_SCENE_DEFINED
