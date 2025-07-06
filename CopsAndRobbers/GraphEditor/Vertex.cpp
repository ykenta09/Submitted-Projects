#include "pch.h"
#include "GraphEditor/Vertex.h"

int Vertex::s_maxVertexNumber = 0;

// コンストラクタ
Vertex::Vertex(const DirectX::SimpleMath::Vector3& position)
    : 
    m_vertexNumber(0),
    m_position(position)
{
    m_vertexNumber = s_maxVertexNumber++;
    m_position = position;
}

// デストラクタ
Vertex::~Vertex()
{
}
