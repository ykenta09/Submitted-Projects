#include "pch.h"
#include "GraphEditor/Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Libraries/yamadaLib/Resources.h"

// �R���X�g���N�^
Graph::Graph()
    : 
    m_vertexList{},
    m_adjacencyList{},
    m_patrolRootMap{},
    m_patrolAdjacencyList{},
    m_initialVertexNumber(0)
{
}

// �f�X�g���N�^
Graph::~Graph()
{
    Clear();
}

//�O���t������������
void Graph::Clear()
{
    m_vertexList.clear();
    m_adjacencyList.clear();
    m_patrolRootMap.clear();
    m_patrolAdjacencyList.clear();
}

//���_�𐶐�����
bool Graph::CreateVertex(const int enemyIndex, const Vertex& vertex)
{
    //���_���X�g�ɒ��_��ǉ�����
    m_vertexList.emplace_back(vertex);
    //���݂���G���ׂĂ̗אڃ��X�g�ɒ��_��ǉ�����
    for (int number = 0; number <= enemyIndex; number++)
    {
        m_adjacencyList[number].adjacencyList.emplace_back(std::vector<int>(0, 0));
    }

    return true;
}

//���_���X�g�̒��_�ʒu���X�P�[�����O����
void Graph::ScaleVertexPositionOfVertexList(const float& scale)
{
    for (auto it = m_vertexList.begin(); it != m_vertexList.end(); it++)
    {
        it->SetPosition(it->GetPosition() * scale);
    }
}

// ���_���X�g���璸�_���폜����
bool Graph::RemoveVertexFromVertexList(const Vertex& vertex)
{
    // ���_���X�g���璸�_���폜����
    m_vertexList.erase(
        std::remove_if(
            m_vertexList.begin(),
            m_vertexList.end(),
            [&](Vertex const& vertexIndex) { return vertexIndex.GetPosition() == vertex.GetPosition(); }),
        m_vertexList.end()
    );
    // ���_�ԍ����Đݒ肷��
    ReNumber();
    // ����I��
    return true;
}

// ���_���X�g���璸�_���폜����
bool Graph::RemoveVertexFromVertexList(const int& vertexIndex)
{
    // ���_���X�g���璸�_���폜����
    m_vertexList.erase(m_vertexList.begin() + vertexIndex);
    // ���_�ԍ����Đݒ肷��
    ReNumber();
    // ����I��
    return true;
}


//�אڃ��X�g�ɃG�b�W�̒��_��ǉ�����
bool Graph::AddVertexOfEdgeToAdjacencyList(const int& enemyNumber, const int& vertexNumber, const int& vertexNumberOfEdge)
{
    //�אڃ��X�g�ɃG�b�W�̒��_�ԍ���ǉ�����
    if (m_adjacencyList[enemyNumber].adjacencyList.empty())
        NewAdjacencyList(enemyNumber);

    m_adjacencyList[enemyNumber].adjacencyList[vertexNumber].push_back(vertexNumberOfEdge);

    return false;
}


// �אڃ��X�g�̃G�b�W���_���폜����
bool Graph::RemoveVertexOfEdgeFromAdjacencyList(const int& enemyNumber, const int& vertexNumber)
{        
    // �אڃ��X�g���폜����
    m_adjacencyList[enemyNumber].adjacencyList[vertexNumber].clear();
    // �אڃ��X�g�ō폜���ׂ����_�ԍ������v�f���폜����
    for (int vertexIndex = 0; vertexIndex < (int)m_adjacencyList[enemyNumber].adjacencyList.size(); vertexIndex++)
    {
        // �אڃ��X�g���璸�_�C���f�b�N�X���폜����
        m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].erase(
            std::remove_if(
                m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].begin(),
                m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].end(),
                [&](int index) {return index == vertexNumber; }
            ),
            m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].end()
        );
    }
    // �ߗ׃��X�g�̃G�b�W�̒��_�C���f�b�N�X�𒲐�����
    for (int vertexIndex = 0; vertexIndex < (int)m_adjacencyList[enemyNumber].adjacencyList.size(); vertexIndex++)
    {
        for (int edgeIndex = 0; edgeIndex < (int)m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].size(); edgeIndex++)
        {
            // �אڃ��X�g�̒��_�C���f�b�N�X���폜���钸�_�C���f�b�N�X���傫���ꍇ
            if (m_adjacencyList[enemyNumber].adjacencyList[vertexIndex][edgeIndex] > vertexNumber)
            {
                // ���_�C���f�b�N�X���P���炷
                m_adjacencyList[enemyNumber].adjacencyList[vertexIndex][edgeIndex]--;
            }
        }
    }
    // �אڃ��X�g�̔z��̗v�f���폜����
    m_adjacencyList[enemyNumber].adjacencyList.erase(m_adjacencyList[enemyNumber].adjacencyList.begin() + vertexNumber);

    // ����I��
    return true;
}


// ���_���X�g����C���f�b�N�X�Ԗڂ̒��_���폜����
void Graph::RemoveVertexFromVertexListByIndex(const int& index)
{
    // ���_���X�g�̔z��̗v�f���폜����
    m_vertexList.erase(m_vertexList.begin() + index);
}

// �אڃ��X�g����C���f�b�N�X�Ԗڂ̃G�b�W�̒��_���폜����
void Graph::RemoveVertexOfEdgeFromAdjacencyListByIndex(const int& enemyNumber, const int& index)
{
    // �אڃ��X�g�̔z��̗v�f���폜����
    m_adjacencyList[enemyNumber].adjacencyList.erase(m_adjacencyList[enemyNumber].adjacencyList.begin() + index);
}


// �אڃ��X�g����C���f�b�N�X�Ԗڂ̒��_�̃G�b�W�C���f�b�N�X�Ԗڂ̃G�b�W���폜����
void Graph::RemoveEgdeFromAdjacencyListByIndex(const int& enemyNumber, const int& index, const int& edgeIndex)
{
    // �אڃ��X�g����C���f�b�N�X�Ԗڂ̒��_�̃G�b�W�C���f�b�N�X�Ԗڂ̃G�b�W���폜����
    m_adjacencyList[enemyNumber].adjacencyList[index].erase(m_adjacencyList[enemyNumber].adjacencyList[index].begin() + edgeIndex);
}

// ���_�ԍ����Đݒ肷��
void Graph::ReNumber()
{
    for (int index = 0; index < (int)m_vertexList.size(); index++)
    {
        // ���_�ԍ����Đݒ肷��
        m_vertexList[index].SetVertexNumber(index);
    }
    // �ő咸�_�ԍ����Đݒ肷��
    Vertex::ResetMaxNumber((int)m_vertexList.size());
}

// JSON�`���ŃO���t�f�[�^��ۑ�����
bool Graph::SaveGraphDataByJSON(int index)
{
    // �o�̓t�@�C���X�g���[����錾����
    std::ofstream out;

    // �o�̓t�@�C���X�g���[�����I�[�v������
    out.open(L"Resources\\EnemyRouteData\\StagePatrolRoot.json", std::ios::out);
    if (!out.is_open())
        return false;

    std::string text;
    out << "{" << std::endl;
    //�G�̑������o�͂���
    out << "\t\"enemy_index\": " << index << ", " << std::endl;

    // ���_���X�g�̕�������o�͂���
    out << "\t\"rootList\": [" << std::endl;
    // ���_���X�g���o�͂���
    for (int vertexIndex = 0; vertexIndex < (int)m_vertexList.size(); vertexIndex++)

    {
        // ���_�ԍ��̕�������o�͂���
        out << "\t\t{\"root_number\": ";
        // ���_�ԍ����o�͂���
        out << m_vertexList[vertexIndex].GetVertexNumber() << ", ";
        // �ʒu�̕�������o�͂���
        out << "\"position\": ";
        out << "{";
        // X���W���o�͂���
        out << "\"x\": " << m_vertexList[vertexIndex].GetPosition().x << ", ";
        // Y���W���o�͂���
        out << "\"y\": " << m_vertexList[vertexIndex].GetPosition().y << ", ";
        // Z���W���o�͂���
        out << "\"z\": " << m_vertexList[vertexIndex].GetPosition().z;

        out << "}";
        // ���_���X�g�̃f�[�^���p�����Ă��邩�ǂ����𒲂ׂ�
        if (vertexIndex < m_vertexList.size() - 1)
            // �u���b�N�ƃR���}���o�͂���
            out << "}," << std::endl;
        else
            // �u���b�N���o�͂���
            out << "}" << std::endl;
    }
    out << "\t]," << std::endl;
    

    // �אڃ��X�g�̕�������o�͂���
    out << "\t\"adjacency_list\": [" << std::endl;
    for (int enemyNumber = 0; enemyNumber <= index; enemyNumber++)
    {
        out << "\t\t{" << std::endl;
        // �G�ԍ����o�͂���
        out << "\t\t\t\"enemy_number\": " << enemyNumber << ", " << std::endl;

        // �G�̏������_�ԍ����o�͂���
        out << "\t\t\t\"intial_number\": " << m_initialVertexNumber[enemyNumber][0] << ", " << std::endl;

        //�אڂ��Ă��钸�_���o��
        out << "\t\t\t\"connected_vertices\": [" << std::endl;
        for (int vertexIndex = 0; vertexIndex < (int)m_adjacencyList[enemyNumber].adjacencyList.size(); vertexIndex++)
        {
            out << "\t\t\t\t[";
            for (int edgeIndex = 0; edgeIndex < (int)m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].size(); edgeIndex++)
            {
                // �אڃ��X�g�̒��_�f�[�^��\������
                out << m_adjacencyList[enemyNumber].adjacencyList[vertexIndex][edgeIndex];
                // ���_���X�g�̒��_�f�[�^���p�����Ă��邩�ǂ����𒲂ׂ�
                if (edgeIndex < m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].size() - 1)
                {
                    // ���_�f�[�^���p�����Ă���
                    out << ", ";
                }
            }
            // ���_���אڃ��X�g�̍Ōォ�ǂ����𒲂ׂ�
            if (vertexIndex < m_adjacencyList[enemyNumber].adjacencyList.size() - 1)
                // ���_�f�[�^�͌p�����Ă���
                out << "]," << std::endl;
            else
                // ���_�f�[�^�͍Ō�̃f�[�^
                out << "]" << std::endl;
        }
        out << "\t\t\t]" << std::endl;
        if (enemyNumber < index)
            out << "\t\t}," << std::endl;
        else
            out << "\t\t}" << std::endl;
    }  
    out << "\t]" << std::endl;
    out << "}" << std::endl;


    // �o�̓t�@�C���X�g���[�����N���[�Y����
    out.close();
    // ����I��
    return true;
}

// JSON�`���ŃO���t�f�[�^��ǂݍ���
bool Graph::LoadGraphDataByJSON(const int stageNumber)
{
    std::stringstream ss;
    std::string graphData;

    // ���̓t�@�C���X�g���[��
    std::ifstream in(L"Resources\\EnemyRouteData\\Stage" + std::to_wstring(stageNumber) + L"PatrolRoot.json", std::ifstream::in);
    // �o�̓t�@�C���X�g���[�����I�[�v������
    in.open(L"Resources\\EnemyRouteData\\Stage" + std::to_wstring(stageNumber) + L"PatrolRoot", std::ifstream::in);
    if (!in.is_open())
        return false;

    // �X�g�����O�X�g���[��
    ss << in.rdbuf();
    // �z����N���A����
    m_graphData.clear();
    // ���s�܂ł̈�s���̕�������擾����
    while (std::getline(ss, graphData))
    {
        // �O���t�f�[�^����s�v�ȕ�������������
        graphData.erase(std::remove_if(
            graphData.begin(),
            graphData.end(),
            [](char c) {
                return (
                    c == '\r' || c == '\t' || c == ' ' || c == '\n' || c == '\"' ||
                    c == '[' || c == ']' || c == '{' || c == '}'
                    );
            }),
            graphData.end()
        );
        if (!graphData.empty())
        {
            // �z��Ƀ��[�h��ǉ�����
            m_graphData.push_back(graphData);
        }
    }
    // ���̓t�@�C���X�g���[�����N���[�Y����
    in.close();
    // ����I��
    return true;
}

/// JSON�`���̃f�[�^���p�[�X���I�u�W�F�N�g�ɕϊ�����
void Graph::ParseJSON()
{
    int position;

    //�G�̑���
    int enemyIndex = 0;
    for (int index = 0; index < (int)m_graphData.size(); index++)
    {
        //�G�̑����̕��������������
        position = (int)m_graphData[index].find("enemy_index");
        //�����񃊃X�g��������Ȃ������ꍇ
        if (position == std::string::npos)
            continue;
        std::stringstream ss(m_graphData[index].c_str());
        //enemy_index���폜����
        m_graphData[index].replace(position, strlen("enemy_index:"), "");
        ss.clear();
        ss.str(m_graphData[index]);
        ss >> enemyIndex;
        m_enemyIndex = enemyIndex;
        break;
    }

    // JSON�f�[�^���p�[�X����
    for (int index = 0; index < (int)m_graphData.size(); index++)
    {
        int rootNumber;
        float x;
        float y;
        float z;
        // �����Froot_number:0,position:x:-40,y:20,
        std::stringstream ss(m_graphData[index].c_str());
        //�G�̔ԍ����������������
        position = (int)m_graphData[index].find("root_number:");
        // �����񂪌�����Ȃ��ꍇ
        if (position == std::string::npos)
            continue;
        // "root_number:"���폜����
        m_graphData[index].replace(position, strlen("root_number:"), "");
        // ",position:x:"����������
        position = (int)m_graphData[index].find(",position:x:");
        // ",position:x:"���󕶎��ɒu��������
        m_graphData[index].replace(position, strlen(",position:x:"), " ");
        // ",y:"��T������
        position = (int)m_graphData[index].find(",y:");
        // ",y:"���󕶎��ɒu��������
        m_graphData[index].replace(position, strlen(",y:"), " ");
        // ",z:"��T������
        position = (int)m_graphData[index].find(",z:");
        // ",z:"���󕶎��ɒu��������
        m_graphData[index].replace(position, strlen(",z:"), " ");

        ss.clear();
        ss.str(m_graphData[index]);
        // ���_�̍��W���擾���� rootNumber�o�H�ԍ�,���W
        ss >> rootNumber >> x >> y >> z;
        //���_����z��ɕۑ�����
        m_patrolRootMap.push_back(DirectX::SimpleMath::Vector3(x, y, z));
        // ���_�𐶐�����(VertexList�ɓo�^����)
        CreateVertex(m_enemyIndex, Vertex(DirectX::SimpleMath::Vector3(roundf(x), roundf(y), roundf(z))));
    }

    //�אڃ��X�g�f�[�^�̒��o
    bool found = false;
    int enemyNumber = 0;
    int initialNumber = 0;
    int vertexIndex = 0;
    for (int index = 0; index < (int)m_graphData.size(); index++)
    {
        //��������
        if (!found)
        {
            position = (int)m_graphData[index].find("enemy_number:");
            if (position == std::string::npos)
                continue;
            found = true;
        }
        //�G�ԍ��������Aconnected_vertices�̏I�������m
        position = (int)m_graphData[index].find("enemy_number:");
        if (position != std::string::npos)
        {
            //�G�ԍ��̕������������      
            m_graphData[index].replace(position, strlen("enemy_number:"), "");
            std::stringstream ss(m_graphData[index].c_str());
            ss >> enemyNumber;
            // index��size�͈͓��Ȃ���Z
            if (index < (int)m_graphData.size() - 1)
                index++;
            else
                continue;
            //�������_�ԍ��̕������������
            m_graphData[index].replace(position, strlen("intial_number:"), "");
            std::stringstream ssInital(m_graphData[index].c_str());
            ssInital >> initialNumber;
            m_initialVertexNumber.resize(enemyNumber + 1);
            m_initialVertexNumber[enemyNumber].resize(1);
            m_initialVertexNumber[enemyNumber][0] = initialNumber;
            // index��size�͈͓��Ȃ���Z
            if (index < (int)m_graphData.size() - 1)
                index++;
            else
                continue;

            position = (int)m_graphData[index].find("connected_vertices:");
            m_graphData[index].replace(position, strlen("connected_vertices:"), "");
            vertexIndex = 0;
            // index��size�͈͓��Ȃ���Z
            if (index < (int)m_graphData.size() - 1)
                index++;
            else
                continue;
        }       

        // �אڃ��X�g�̍s���̃R���}�����݂���ꍇ�͍폜����
        if (m_graphData[index].at((m_graphData[index].size() - 1)) == ',')
            m_graphData[index].erase(--m_graphData[index].end());

        // �X�g�����O�X�g���[���𐶐�����
        std::stringstream ssVertices(m_graphData[index].c_str());
        std::string vertexNumber;

        while (std::getline(ssVertices, vertexNumber, ','))
        {
            int vertexIndexOfEdge = std::stoi(vertexNumber);
            AddVertexOfEdgeToAdjacencyList(enemyNumber, vertexIndex, vertexIndexOfEdge);
        }
        vertexIndex++;
    }
}

void Graph::NewAdjacencyList(const int& enemyNumber)
{
    // �G�ԍ��ɑΉ�����אڃ��X�g�����݂��Ȃ��ꍇ�A���_�̐����������X�g�𐶐�
    if (m_adjacencyList[enemyNumber].adjacencyList.empty())
    {
        for (int i = 0; i < m_vertexList.size(); i++)
        {
             m_adjacencyList[enemyNumber].adjacencyList.emplace_back(std::vector<int>(0, 0));
        }
    }
    else
    {
        for (int i = 0; i < m_vertexList.size(); i++)
        {
            if (m_adjacencyList[enemyNumber].adjacencyList.size() < m_vertexList.size())
                m_adjacencyList[enemyNumber].adjacencyList.emplace_back(std::vector<int>(0, 0));
        }
    }
}

//�G�ԍ��̗אڃ��X�g���폜����
void Graph::ClearNumberAdjacenyList(const int& enemyNumber)
{
    //�G�ԍ��̗אڃ��X�g���폜����
    m_adjacencyList.erase(enemyNumber);
    //�G�̐������炷
    m_enemyIndex--;
    std::unordered_map<int, PatrolRoute> updateList;

    for (auto it = m_adjacencyList.begin(); it != m_adjacencyList.end(); it++)
    {
        int currentEnemyNumber = it->first;
        if (currentEnemyNumber > enemyNumber)
        {
            updateList[currentEnemyNumber - 1] = it->second;
        }
        else
        {
            updateList[currentEnemyNumber] = it->second;
        }
    }
    //�אڃ��X�g���X�V����
   m_adjacencyList = updateList;
   if (m_adjacencyList.empty())
   {
       NewAdjacencyList(enemyNumber);
   }
   //�G�ԍ��ɑΉ������������_�ԍ����폜����
   m_initialVertexNumber.erase(m_initialVertexNumber.begin() + enemyNumber);
   if (m_initialVertexNumber.empty())
   {
       CreateInitailVertex(enemyNumber, m_vertexList.front().GetVertexNumber());
   }
}

//�G�̏������_�ԍ����쐬
void Graph::CreateInitailVertex(const int& enemyNumber, const int vertexNumber)
{
    // �G�ԍ��ɑΉ�����z�񂪑��݂��Ȃ��ꍇ�́A�T�C�Y�𒲐�
    if (m_initialVertexNumber.size() <= enemyNumber)
    {
        m_initialVertexNumber.resize(enemyNumber + 1);
        m_initialVertexNumber[enemyNumber].resize(1);
    }   
    m_initialVertexNumber[enemyNumber][0] = vertexNumber;
}
