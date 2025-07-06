#include "pch.h"
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include "GraphScene.h"
#include "Model.h"
#include "Libraries/MyLib/Geometry.h"

 const float GraphScene::VERTEX_RADIUS  = 0.5f;

// �R���X�g���N�^
 GraphScene::GraphScene(CommonResources* resources)
	 :
	 m_commonResources(resources),
	 m_editMode(false),
	 m_graphics(nullptr),							// DirectXGraphics�N���X
	 m_device(nullptr),								// Device�N���X
	 m_context(nullptr),								// DeviceContext�N���X
	 m_keyboardState{},								// �L�[�{�[�h�X�e�[�g
	 m_mouseCursorPosition{},					// �}�E�X�J�[�\���ʒu
	 m_mouseState{},									// �}�E�X�X�e�[�g
	 m_mouseStateTracker{},						// �}�E�X�X�e�[�g�g���b�J�[
	 m_worldMatrix{},									// ���[���h
	 m_viewMatrix{},									// �r���[
	 m_projectionMatrix{},							// �v���W�F�N�V����
	 m_cameraRotation{},							// �J������]
	 m_cameraPosition{},							// �J�����ʒu
	 m_cameraFocus{},								// �J�����t�H�[�J�X
	 m_rotaionAngle(0.0f),							// �p�x
	 m_distance(10.0f),								// �����_���王�_�܂ł̋���
	 m_zoom(1.0f),										// �Y�[��
	 m_fov(DirectX::XM_PI / 4.0f),				// �t�B�[���h�I�u�r���[
	 m_nearPlane(0.1f),								// �j�A�v���[��
	 m_farPlane(0.0f),								// �t�@�[�v���[��
	 m_drive{},											// �h���C�u
	 m_path{},												// �p�X
	 m_fileName{},										// �t�@�C����
	 m_extension{},										// �g���q
	 m_scale(1.0f),										// �X�P�[��
	 m_position{},										// �ʒu
	 m_graph(nullptr),								// �O���t
	 m_spriteString3D(nullptr),					// �X�v���C�g�X�g�����O3D
	 m_enemyNumber(0),							//�G�ԍ�
	 m_enemyIndex(1),							//�G�̐�
	 m_initialEditMode(false)					//�������_�ԍ��ҏW���[�h
 {
	// Graphics�N���X�̃C���X�^���X���擾����
	m_graphics = yamadalib::Graphics::GetInstance();
	// �f�o�C�X���擾����
	m_device = yamadalib::Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾����
	m_context = yamadalib::Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
}

// �f�X�g���N�^
GraphScene::~GraphScene()
{
	// Graph�N���X�̃C���X�^���X�����Z�b�g����
	m_graph.reset();
}

// ����������
void GraphScene::Initialize()
{
	// Graph�N���X�̃C���X�^���X�𐶐�����
	m_graph = std::make_unique<Graph>();
	// SpriteString3D�N���X�̃C���X�^���X�𐶐�����
	m_spriteString3D = std::make_unique<SpriteString3D>();
	//�O���t�f�[�^��ǂݍ���
	LoadGraphData();
	m_enemyIndex = m_graph->GetEnemyIndex();
}

// �X�V����
void GraphScene::Update(float timer)
{
	UNREFERENCED_PARAMETER(timer);
	// �L�[�{�[�h�̏�Ԃ��擾����
	m_keyboardState = DirectX::Keyboard::Get().GetState();
	//�L�[�{�[�h�̏�Ԃ��X�V����
	m_keyboardTracker.Update(m_keyboardState);
	// �}�E�X�̏�Ԃ��擾����
	m_mouseState = m_commonResources->GetInputManager()->GetMouse()->GetState();
	// �}�E�X�g���b�J�[���X�V����
	m_mouseStateTracker.Update(m_mouseState);

	//�ҏW���[�h��L�������ɂ���
	if (m_editMode == false && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::P))
	{
		m_editMode = true;
		m_commonResources->GetInputManager()->SetMouseMode(DirectX::Mouse::MODE_ABSOLUTE);
	}
	else if (m_editMode == true && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::P))
	{
		m_editMode = false;
		m_commonResources->GetInputManager()->SetMouseMode(DirectX::Mouse::MODE_RELATIVE);
	}


	if (!m_editMode) return;

	// �O���t�f�[�^���N���A����
	if (m_keyboardState.C)
	{
		ClearAll();
	}
	// �O���t�f�[�^��ǂݍ���
	if (m_keyboardState.L)
	{
		// �O���t�f�[�^��ǂݍ���
		LoadGraphData();
	}
	// �O���t�f�[�^��ۑ�����
	if (m_keyboardState.K)
	{
		//  �O���t�f�[�^��ۑ�����
		SaveGraphData();
	}
	//�������_�ԍ����쐬����
	if (m_initialEditMode == false && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::I))
	{
		m_initialEditMode = true;
	}
	else if (m_initialEditMode == true && m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::I))
	{
		m_initialEditMode = false;
	}

	//�V�����o�H��ǉ�����
	if (m_keyboardState.N)
	{
		NewEnemyAdjacencyList();
	}
	//�o�H���폜����
	if (m_keyboardState.Delete)
	{
		ClearEnemyAdjacencyList();
	}
	//�G�ԍ���؂�ւ���
	if (m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Left))
	{
		ChangeEnemyAdjacencyList(false);
	}
	if (m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Right))
	{
		ChangeEnemyAdjacencyList(true);
	}

	// ���ʂ�����������
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);
	// �}�E�X�J�[�\���̃X�N���[���ʒu���擾����
	m_mouseCursorPosition = DirectX::SimpleMath::Vector2(roundf((float)m_mouseState.x), roundf((float)m_mouseState.y));

	// �X�N���[���ʒu��������𐶐�����
	DirectX::SimpleMath::Ray ray = CreateRayFromScreenPoint(m_mouseCursorPosition);
	// �����ƕ��ʂƂ̌����_���擾����
	IntersectRayPlane(ray, plane, &m_vertex);

	//�������_�ԍ���ݒ肷��
	if (m_initialEditMode == true && m_mouseStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED && m_mouseState.leftButton == true)
	{
		InitalVertexNumber();
		return;
	}

	// ���}�E�X�{�^�����N���b�N�����ꍇ���_��ǉ�����
	if (m_mouseStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED && m_mouseState.leftButton == true)
	{
		// �u���_�v�܂��́u�G�b�W�v��ǉ�����
		AddVertexOrEdge();
	}

	// �E�}�E�X�{�^�����N���b�N�����ʒu�ɒ��_���폜����
	if (m_mouseStateTracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
	{
		// ���_�ƒ��_�ɗאڂ���G�b�W���폜����
		RemoveVertexOrEdge();
	}
}

// �}�E�X�J�[�\���ʒu����X�N���[���ւ̌����𐶐�����
DirectX::SimpleMath::Ray GraphScene::CreateRayFromScreenPoint(const DirectX::SimpleMath::Vector2& mousePosition)
{
	// �r���[�|�[�g���擾����
	DirectX::SimpleMath::Viewport viewport(m_graphics->GetDeviceResources()->GetScreenViewport());
	// �X�N���[���̃}�E�X�J�[�\���ʒu��Vector3�ɐݒ肷��
	DirectX::SimpleMath::Vector3 screenPosition(mousePosition.x, mousePosition.y, 0.0f);
	// �X�N���[�����W��3�������W�ɕϊ�����
	DirectX::SimpleMath::Vector3 worldPoint = viewport.Unproject(
		screenPosition,
		m_graphics->GetProjectionMatrix(),
		m_graphics->GetViewMatrix(),
		DirectX::SimpleMath::Matrix::Identity
	);

	// �r���[�s�񂩂�t�s����擾����
	DirectX::SimpleMath::Matrix matrix = m_graphics->GetViewMatrix().Invert();
	// �J�����ʒu���擾����
	DirectX::SimpleMath::Vector3 cameraPosition(matrix._41, matrix._42, matrix._43);
	// �J�����̈ʒu���烏�[���h��Ԃ̃}�E�X�J�[�\���ʒu�ւ̕������擾����
	DirectX::SimpleMath::Vector3 direction = worldPoint - cameraPosition;
	// ���K������
	direction.Normalize();
	// �����𐶐�����
	DirectX::SimpleMath::Ray ray(cameraPosition, direction);
	// ������Ԃ�
	return ray;
}

// �`�悷��
void GraphScene::Render()
{
	const DirectX::XMVECTORF32 xaxis = { 100.f, 0.0f, 0.0f };
	const DirectX::XMVECTORF32 yaxis = { 0.0f, 0.0f, 100.f };
	//�ҏW���[�h�̂ݕ`�悷��
	if (m_editMode)
	{
		// �O���b�h��`�悷��
		//DrawGrid(xaxis, yaxis, DirectX::g_XMZero, 50, 50, DirectX::Colors::LightGray);
		auto debugString = m_commonResources->GetDebugString();
		debugString->AddString("EnemyNumber, %d", m_enemyNumber);
		debugString->AddString("EnemyIndex, %d", m_enemyIndex);
		debugString->AddString("EnemyInitailVertex, %d", m_graph->GetInitailVertex(m_enemyNumber));
		// �O���t��`�悷��
		DrawGraph();
	}

#ifdef DEBUG

#endif // DEBUG


}

// �㏈�����s��
void GraphScene::Finalize()
{
	// Graph�N���X�̃C���X�^���X�����Z�b�g����
	m_graph.reset();
}

//�O���t�f�[�^��ǂݍ���
void GraphScene::LoadGraphData()
{
   // �O���t�f�[�^���N���A����
   m_graph->Clear();
   // �ő咸�_�ԍ����[�����Z�b�g����
   Vertex::ResetMaxNumber(0);
   //// ���ׂĂ̕����z����폜����
   m_spriteString3D->RemoveAllString();
   // JSON�`���̃f�[�^��ǂݍ���
   m_graph->LoadGraphDataByJSON(m_commonResources->GetBlackBoard()->GetStageNumber());
   // �ǂݍ���JSON�`���̃f�[�^���I�u�W�F�N�g�ɕϊ�����
   m_graph->ParseJSON();
   // SpriteString3D�N���X�̃C���X�^���X�ɂ��ׂĂ̒��_�ԍ���ǉ�����
   AddAllVertexNumberToSpriteString3D();
}

//�X�e�[�W�f�[�^����G�̐����擾����
int GraphScene::GetMaxEnemyIndex()
{
	// �O���t�f�[�^���N���A����
	m_graph->Clear();
	// �ő咸�_�ԍ����[�����Z�b�g����
	Vertex::ResetMaxNumber(0);
	//// ���ׂĂ̕����z����폜����
	m_spriteString3D->RemoveAllString();
	// JSON�`���̃f�[�^��ǂݍ���
	m_graph->LoadGraphDataByJSON(m_commonResources->GetBlackBoard()->GetStageNumber());
	// �ǂݍ���JSON�`���̃f�[�^���I�u�W�F�N�g�ɕϊ�����
	m_graph->ParseJSON();
	// SpriteString3D�N���X�̃C���X�^���X�ɂ��ׂĂ̒��_�ԍ���ǉ�����
	AddAllVertexNumberToSpriteString3D();

	return m_graph->GetEnemyIndex();
}

 //�O���t�f�[�^��ۑ�����
void GraphScene::SaveGraphData()
{
	if (MessageBox(NULL, L"���_����ۑ����܂����H", L"���_���̕ۑ�", MB_YESNO) == IDYES)
	{
		// �I�u�W�F�N�g��JSON�`���̃f�[�^�Ƃ��ĕۑ�����
		m_graph->SaveGraphDataByJSON(m_enemyIndex);
	}
}

// SpriteString3D�N���X�̃C���X�^���X�ɂ��ׂĂ̒��_�ԍ���ǉ�����
void GraphScene::AddAllVertexNumberToSpriteString3D()
{
	// SpriteString3D�N���X�̃C���X�^���X�ɒ��_�ԍ�(������)��ǉ�����
	for (int index = 0; index < m_graph->GetVertexList().size(); index++)
	{
		wchar_t buffer[10];
		// ���_�ԍ����t�H�[�}�b�g������
		swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%3d", m_graph->GetVertex(index).GetVertexNumber());
		// SpriteString3D�N���X�̃C���X�^���X�ɕ������ǉ�����
		m_spriteString3D->AddString(
			buffer, 
			DirectX::SimpleMath::Vector3(
				m_graph->GetVertex(index).GetPosition().x + 1, 
				3.0f, 
				m_graph->GetVertex(index).GetPosition().z
			),
		   DirectX::Colors::Blue
		);
	}
}

// �O���b�h��`�悷��
void GraphScene::DrawGrid(
	DirectX::FXMVECTOR xAxis,
	DirectX::FXMVECTOR yAxis,
	DirectX::FXMVECTOR origin,
	size_t xdivs,
	size_t ydivs,
	DirectX::GXMVECTOR color
)
{
	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	// �v���~�e�B�u�`����J�n����
	m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
		[this, xdivs, ydivs, &xAxis, &origin, &yAxis, &color](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
		{
			for (int index = 0; index <= xdivs; ++index)
			{
				float percent = float(index) / float(xdivs);
				percent = (percent * 2.0f) - 1.0f;
				// �X�P�[�����v�Z����
				DirectX::XMVECTOR scale = DirectX::XMVectorScale(xAxis, percent);
				scale = DirectX::XMVectorAdd(scale, origin);
				// ���_1��ݒ肷��
				DirectX::VertexPositionColor v1(DirectX::XMVectorSubtract(scale, yAxis), color);
				// ���_2��ݒ肷��
				DirectX::VertexPositionColor v2(DirectX::XMVectorAdd(scale, yAxis), color);
				// ������`�悷��
				m_graphics->GetPrimitiveBatch()->DrawLine(v1, v2);
			}

			for (int index = 0; index <= ydivs; index++)
			{
				float percent = float(index) / float(ydivs);
				percent = (percent * 2.0f) - 1.0f;
				// �X�P�[�����v�Z����
				DirectX::XMVECTOR scale = DirectX::XMVectorScale(yAxis, percent);
				scale = DirectX::XMVectorAdd(scale, origin);
				// ���_1��ݒ肷��
				DirectX::VertexPositionColor v1(DirectX::XMVectorSubtract(scale, xAxis), color);
				// ���_2��ݒ肷��
				DirectX::VertexPositionColor v2(DirectX::XMVectorAdd(scale, xAxis), color);
				// ������`�悷��
				m_graphics->GetPrimitiveBatch()->DrawLine(v1, v2);
			}
		});
}

// ���_�Ɨאڃ��X�g��`�悷��
void GraphScene::DrawGraph()
{
	// ���_���X�g���擾����
	const std::vector<Vertex>& vertexList = m_graph->GetVertexList();
	// �אڃ��X�g���擾����
	const std::unordered_map<int, Graph::PatrolRoute>& adjacencyList = m_graph->GetAdjacencyList();
	// ���_���X�g�܂��͗אڃ��X�g�̃T�C�Y��0�̏ꍇ
	if (vertexList.size() == 0 || adjacencyList.size() == 0)
		return;

	// �v���~�e�B�u�`����J�n����
	m_graphics->DrawPrimitivesColor(*m_graphics->GetCommonStates(), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix(),
		[this, vertexList, adjacencyList](DirectX::PrimitiveBatch<DirectX::VertexPositionColor>&)
		{
			for (int index = 0; index < vertexList.size(); index++)
			{
				DirectX::XMVECTOR edgeColor = DirectX::Colors::Blue;
				for (int edgeIndex = 0; edgeIndex < adjacencyList.at(m_enemyNumber).adjacencyList.at(index).size(); edgeIndex++)
				{
					// �אڃ��X�g�̒��_�Ԃɒ�����`�悷��
					m_graphics->DrawLine(
						m_graph->GetVertex(index).GetPosition(),
						m_graph->GetVertex(adjacencyList.at(m_enemyNumber).adjacencyList.at(index)[edgeIndex]).GetPosition() - m_graph->GetVertex(index).GetPosition(),
						edgeColor
					);
				}
				// ���_�̊���F��ݒ肷��
				DirectX::XMVECTOR vertexColor = DirectX::Colors::Blue;
				if (m_edgeMode == ON && (int)index == m_vertexStartIndex)
				{
					// ���_��I�������Ƃ��̐F��ݒ肷��
					vertexColor = DirectX::Colors::Red;
				}
				// ���_��`�悷��
				m_graphics->DrawCircle(
				   DirectX::SimpleMath::Vector2(vertexList[index].GetPosition().x,vertexList[index].GetPosition().z),
				   0.5f,
				   vertexColor
				);
			}
		});
	// SpriteString�N���X�̕������`�悷��
	m_spriteString3D->Render();
}

// �v���W�F�N�V�����𐶐�����
void GraphScene::CreateProjection()
{
	// �E�B���h�E�T�C�Y���擾����
	auto size = m_graphics->GetDeviceResources()->GetOutputSize();
	// �v���W�F�N�V�����𐶐�����
	m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(size.right) / float(size.bottom), m_nearPlane, m_farPlane);
}

// ���_�܂��̓G�b�W��ǉ�����
void GraphScene::AddVertexOrEdge()
{
	// �}�E�X�|�C���^�̈ʒu�𒸓_�x�N�g���ɐݒ肷��
   DirectX::SimpleMath::Vector3 vertexVector = DirectX::SimpleMath::Vector3(
	  std::round(m_vertex.x * 100) / 100,
	  std::round(m_vertex.y * 100) / 100,
	  std::round(m_vertex.z * 100) / 100);
   // ���_���X�g�ɒ��_�x�N�g�������݂��邩�ǂ����𒲂ׂ�
	int vertexIndex = ExistingVertexInVertexList(vertexVector);

	// ���_���X�g�Ƀ}�E�X�|�C���^�[�̈ʒu�����݂��Ă��Ȃ��ꍇ
	if (vertexIndex == ERROR_INDEX)
	{
		// �G�b�W�ҏW���[�h��ON�̏ꍇ
		if (m_edgeMode == ON)
		{
			// �G�b�W�ҏW���[�h��OFF�ɂ���
			m_edgeMode = OFF;
			return;
		}
		// ���_���X�g�ɒ��_��ǉ�����
		m_graph->CreateVertex(m_enemyIndex, Vertex(vertexVector));
		// ���_�ԍ���ǉ�����
		AddVertexNumberToSpriteString3D((int)m_graph->GetVertexList().size());
	}

	// ���_���X�g�Ƀ}�E�X�|�C���^�[�̈ʒu�����݂��Ă���ꍇ
	if (vertexIndex > ERROR_INDEX)
	{
		// �G�b�W�ҏW���[�h��ON�̏ꍇ
		if (m_edgeMode == ON)
		{
			// ���_�I�_�C���f�b�N�X��ݒ肷��
			m_vertexEndIndex = vertexIndex;
			if (m_vertexStartIndex != m_vertexEndIndex)
			{
				// �אڃ��X�g�ɃG�b�W�̒��_��ǉ�����(�o�����ɓo�^����)
				m_graph->AddVertexOfEdgeToAdjacencyList(m_enemyNumber, m_vertexStartIndex, m_vertexEndIndex);
				m_graph->AddVertexOfEdgeToAdjacencyList(m_enemyNumber, m_vertexEndIndex, m_vertexStartIndex);
			}
			// �G�b�W�ҏW���[�h��OFF�ɂ���
			m_edgeMode = OFF;
		}
		else
		{
			// ���_�N�_�C���f�b�N�X��ݒ肷��
			m_vertexStartIndex = vertexIndex;
			// �G�b�W�ҏW���[�h��ON�ɂ���
			m_edgeMode = ON;
		}
	}
}

/// ���_���X�g�Ɏw�肳�ꂽ���_�x�N�g�������݂��邩�ǂ����𒲂ׂ�
int GraphScene::ExistingVertexInVertexList(const DirectX::SimpleMath::Vector3& vertexVector)
{
	// ���_�C���f�b�N�X�����Z�b�g����
	int vertexIndex = ERROR_INDEX;
	for (const Vertex& vertex : m_graph->GetVertexList())
	{
		// ���_�C���f�b�N�X���C���N�������g����
		vertexIndex++;
		// �E�N���b�N�����}�E�X�|�C���^�[�̈ʒu�������̒��_���X�g�ɑ��݂��邩�ǂ���
		if (InsideCircle(2, vertex.GetPosition(), vertexVector))
		{
			// ���_�C���f�b�N�X��ݒ肷��
			return vertexIndex;
		}
	}
	// ���_�͑��݂��Ȃ�
	return ERROR_INDEX;
}

// SpriteString3D�N���X�̃C���X�^���X�ɒ��_�ԍ���ǉ�����
void GraphScene::AddVertexNumberToSpriteString3D(const int& vertexNumber)
{
	using namespace DirectX::SimpleMath;

	// SpriteString3D�N���X�̃C���X�^���X�ɒ��_�ԍ�(������)��ǉ�����
	wchar_t buffer[10];
	// ���_�ԍ����t�H�[�}�b�g������
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%3d", vertexNumber - 1);
	// SpriteString3D�N���X�̃C���X�^���X�ɕ������ǉ�����
	m_spriteString3D->AddString(buffer, 
		Vector3(m_graph->GetVertex(vertexNumber - 1).GetPosition().x + 1, 
			0.0f, 
			m_graph->GetVertex(vertexNumber - 1).GetPosition().y)
	);
}

// ���_�܂��̓G�b�W���폜����
void GraphScene::RemoveVertexOrEdge()
{
	// ���ʂ�����������
	DirectX::SimpleMath::Plane plane(0.0f, 1.0f, 0.0f, 0.0f);

	// �}�E�X�J�[�\���̃X�N���[���ʒu���擾����
	m_mouseCursorPosition = DirectX::SimpleMath::Vector2((float)m_mouseState.x, (float)m_mouseState.y);
	// �X�N���[���ʒu���烌�C�ɕϊ�����
	DirectX::SimpleMath::Ray ray = CreateRayFromScreenPoint(m_mouseCursorPosition);
	// ���C�ƕ��ʂƂ̌����_���擾����
	IntersectRayPlane(ray, plane, &m_vertex);
	// 3D��Ԃ̒��_���擾����
	DirectX::SimpleMath::Vector3 vertexVector = m_vertex;

	// ���_���X�g�Ɏw�肳�ꂽ���_�x�N�g�������݂��邩�ǂ����𒲂ׂ�
	int vertexIndex = ExistingVertexInVertexList(vertexVector);
	// ���_���X�g�Ƀ}�E�X�|�C���^�[�̈ʒu�����݂��Ă��Ȃ��ꍇ�G�b�W���폜����
	if (vertexIndex == ERROR_INDEX)
	{
		// �P�ƂŃG�b�W���폜����
		RemoveEdge(m_enemyNumber, vertexVector);
	}
	// ���_��Ƀ}�E�X�|�C���^�[�����݂��Ă���ꍇ���_����ђ��_�ɗאڂ���G�b�W���폜����
	else if (vertexIndex > ERROR_INDEX)
	{
		// ���_���X�g���璸�_���擾����
		Vertex vertex = m_graph->GetVertex(vertexIndex);
		// ���_���w�肵�אڃ��X�g���폜����
		m_graph->RemoveVertexOfEdgeFromAdjacencyList(m_enemyNumber, vertexIndex);
		// ���_���X�g�Ɏw�肳�ꂽ���_�x�N�g�������݂��邩�ǂ����𒲂ׂ�
		vertexIndex = ExistingVertexInVertexList(vertexVector);
		// ���_���X�g���璸�_���폜����
		m_graph->RemoveVertexFromVertexList(vertexIndex);
		// SpriteString3D�N���X�̕�����z�񂩂炷�ׂĂ̔z��̗v�f���폜����
		m_spriteString3D->RemoveAllString();
		// SpriteString3D�N���X�̕�����z��ɂ��ׂĂ̒��_�ԍ���ǉ�����
		AddAllVertexNumberToSpriteString3D();
		// ���_�ԍ����Đݒ肷��
		m_graph->ReNumber();
	}
}

// �G�b�W���폜����
void GraphScene::RemoveEdge(int number, const DirectX::SimpleMath::Vector3& vertexVector)
{
	// ���_���X�g���擾����
	const std::vector<Vertex>& vertexList = m_graph->GetVertexList();
	// �אڃ��X�g���擾����
	const std::unordered_map<int, Graph::PatrolRoute>& adjacencyList = m_graph->GetAdjacencyList();
	// �אڃ��X�g�ꗗ�̃��[�v���\������
	for (int index = 0; index < (int)adjacencyList.at(number).adjacencyList.size(); index++)
	{
		// �אڃ��X�g����G�b�W�C���f�b�N�X�̃��[�v���\������
		for (int edgeIndex = 0; edgeIndex < (int)adjacencyList.at(m_enemyNumber).adjacencyList.at(index).size(); edgeIndex++)
		{
			// �����Ɖ~�̌���������s���A�������Ă���ꍇ���̃G�b�W���폜����
			if (IntersectCircleLine(
			    vertexVector,
				VERTEX_RADIUS,
			    vertexList[index].GetPosition(),
				vertexList[adjacencyList.at(m_enemyNumber).adjacencyList.at(index)[edgeIndex]].GetPosition()))
			{
				// �אڃ��X�g����index�Ԗڂ̒��_��edgeIndex�Ԗڂ̃G�b�W���폜����
				m_graph->Graph::RemoveEgdeFromAdjacencyListByIndex(m_enemyNumber, index, edgeIndex);
			}
		}
	}

}

// �p�X���𕪗�����
void GraphScene::SplitPath(wchar_t* modelName)
{
	// �p�X���𕪊�����
	_wsplitpath_s(modelName, m_drive, _MAX_DRIVE, m_path, MAX_PATH, m_fileName, _MAX_FNAME, m_extension, _MAX_EXT);
}

//���ׂăN���A����
void GraphScene::ClearAll()
{
	//�@���f�������Z�b�g����
	if (m_model != nullptr) m_model.reset();
	//�A�O���t���N���A����
	m_graph->Clear();
	//�B�ő咸�_�ԍ����[�����Z�b�g����
	Vertex::ResetMaxNumber(0);
	//�C���ׂĂ�3D������z����폜����
	m_spriteString3D->RemoveAllString();
}


//�G�̗אڃ��X�g��؂�ւ���
void GraphScene::ChangeEnemyAdjacencyList(bool increaseNumbers)
{
	if (increaseNumbers)
	{
		if (m_enemyNumber < m_enemyIndex)
			m_enemyNumber++;
	}
	else
	{
		if(m_enemyNumber > 0)
		m_enemyNumber--;
	}
	m_graph->NewAdjacencyList(m_enemyNumber);
}

//���݂̓G�ԍ��̗אڃ��X�g���폜����
void GraphScene::ClearEnemyAdjacencyList()
{
	if (MessageBox(NULL, L"���݂̔ԍ��̃f�[�^���폜���܂����H", L"�f�[�^���폜����", MB_YESNO) == IDYES)
	{
		m_graph->ClearNumberAdjacenyList(m_enemyNumber);
		//�G�̑��������炷
		if (m_enemyIndex > 0)
			m_enemyIndex--;
		//�G�������ԍ��������Ă����牺����
		if (m_enemyNumber > m_enemyIndex)
			m_enemyNumber--;
	}
}

//�V�����G�̗אڃ��X�g��ǉ�����
void GraphScene::NewEnemyAdjacencyList()
{
	if (MessageBox(NULL, L"�V�����G�̌o�H���쐬���܂����H", L"�o�H��ǉ�", MB_YESNO) == IDYES)
	{
		m_enemyIndex++;
		m_graph->CreateInitailVertex(m_enemyIndex, m_graph->GetVertexList().front().GetVertexNumber());
	}
}

//�G�̏������_���W���쐬����
void GraphScene::InitalVertexNumber()
{
	// �}�E�X�|�C���^�̈ʒu�𒸓_�x�N�g���ɐݒ肷��
	DirectX::SimpleMath::Vector3 vertexVector = DirectX::SimpleMath::Vector3(roundf(m_vertex.x), 0.0f, roundf(m_vertex.z));
	// ���_���X�g�ɒ��_�x�N�g�������݂��邩�ǂ����𒲂ׂ�
	int vertexIndex = ExistingVertexInVertexList(vertexVector);

	// ���_���X�g�Ƀ}�E�X�|�C���^�[�̈ʒu�����݂��Ă���ꍇ
	if (vertexIndex > ERROR_INDEX)
	{
		m_graph->CreateInitailVertex(m_enemyNumber, vertexIndex);
	}
	else
	{
		MessageBox(NULL, L"�G�̏������_�ԍ��ҏW���ł�", L"�x���I", MB_YESNO);
	}
}
