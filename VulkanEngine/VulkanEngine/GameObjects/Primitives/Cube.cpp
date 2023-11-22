#include "Cube.h"

const std::vector<uint16_t> indices = { 
	0, 1, 2, 
	2, 3, 0, 
	0, 4, 1 };

void CCube::Initialize(void)
{
	m_pMesh = std::make_shared<CMesh>();

	// Triangle
	m_pMesh->SetVertexData({
		Vertex{Vertex::Position{-0.5f, -0.5f},Vertex::Color::Red()   }, // 0
		Vertex{Vertex::Position{0.5f, -0.5f}, Vertex::Color::Green() }, // 1
		Vertex{Vertex::Position{0.5f, 0.5f}, Vertex::Color::Blue() }, // 2
		Vertex{Vertex::Position{-0.5f, 0.5f},  Vertex::Color::Red()},  // 3
		Vertex{Vertex::Position{0.0f, -1.0f},  Vertex::Color::White()}  // 4
		});

	m_pMesh->SetIndiceData(indices);

	AddComponent(m_pMesh);

	CGameObject::Initialize();
}

void CCube::Update(void)
{
	CGameObject::Update();
}

void CCube::Draw(void)
{
	CGameObject::Draw();
}

std::vector<Vertex>& CCube::GetMeshVertexData(void)
{
	return m_pMesh->GetVertexData();
}

std::vector<uint16_t>& CCube::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
