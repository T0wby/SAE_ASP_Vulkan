#include "Cube.h"

const std::vector<uint16_t> indices = { 
	2, 1, 1, 
	0, 3, 2,

	6, 5, 4,
	4, 7, 6,

	10, 9, 8,
	8, 11, 10,

	14, 13, 12,
	12, 15, 14, 

	18, 17, 16,
	16, 19, 18,

	22, 21, 20,
	20, 23, 22
};

void CCube::Initialize(void)
{
	m_pMesh = std::make_shared<CMesh>();

	// Triangle
	m_pMesh->SetVertexData({
		Vertex{Vertex::Position{-0.5f, -0.5f, -0.5f}, Vertex::Color::Red(),   {1.0f, 1.0f} }, // 0
		Vertex{Vertex::Position{0.5f, -0.5f, -0.5f},  Vertex::Color::Green(), {1.0f, 0.0f} }, // 1
		Vertex{Vertex::Position{0.5f, 0.5f, -0.5f},   Vertex::Color::Blue(),  {0.0f, 0.0f} }, // 2
		Vertex{Vertex::Position{-0.5f, 0.5f, -0.5f},  Vertex::Color::White(), {0.0f, 1.0f} }, // 3

		Vertex{Vertex::Position{0.5f, -0.5f, 0.5f}, Vertex::Color::Red(),  {1.0f, 1.0f}},   // 4
		Vertex{Vertex::Position{-0.5f, -0.5f, 0.5f},  Vertex::Color::Green(),{1.0f, 0.0f}},   // 5
		Vertex{Vertex::Position{-0.5f, 0.5f, 0.5f},   Vertex::Color::Blue(), {0.0f, 0.0f}},   // 6
		Vertex{Vertex::Position{0.5f, 0.5f, 0.5f},  Vertex::Color::White(),{0.0f, 1.0f}},   // 7

		Vertex{Vertex::Position{-0.5f, -0.5f, 0.5f}, Vertex::Color::Red(),   {1.0f, 1.0f} }, // 8
		Vertex{Vertex::Position{0.5f, -0.5f, 0.5f},  Vertex::Color::Green(), {1.0f, 0.0f} }, // 9
		Vertex{Vertex::Position{0.5f, -0.5f, -0.5f},   Vertex::Color::Blue(),  {0.0f, 0.0f} }, // 10
		Vertex{Vertex::Position{-0.5f, -0.5f, -0.5f},  Vertex::Color::White(), {0.0f, 1.0f} }, // 11

		Vertex{Vertex::Position{0.5f, 0.5f, 0.5f}, Vertex::Color::Red(),   {1.0f, 1.0f} }, // 12
		Vertex{Vertex::Position{-0.5f, 0.5f, 0.5f},  Vertex::Color::Green(), {1.0f, 0.0f} }, // 13
		Vertex{Vertex::Position{-0.5f, 0.5f, -0.5f},   Vertex::Color::Blue(),  {0.0f, 0.0f} }, // 14
		Vertex{Vertex::Position{0.5f, 0.5f, -0.5f},  Vertex::Color::White(), {0.0f, 1.0f} }, // 15

		Vertex{Vertex::Position{-0.5f, -0.5f, 0.5f}, Vertex::Color::Red(),   {1.0f, 1.0f} }, // 16
		Vertex{Vertex::Position{-0.5f, -0.5f, -0.5f},  Vertex::Color::Green(), {1.0f, 0.0f} }, // 17
		Vertex{Vertex::Position{-0.5f, 0.5f, -0.5f},   Vertex::Color::Blue(),  {0.0f, 0.0f} }, // 18
		Vertex{Vertex::Position{-0.5f, 0.5f, 0.5f},  Vertex::Color::White(), {0.0f, 1.0f} }, // 19

		Vertex{Vertex::Position{0.5f, -0.5f, -0.5f}, Vertex::Color::Red(),   {1.0f, 1.0f} }, // 20
		Vertex{Vertex::Position{0.5f, -0.5f, 0.5f},  Vertex::Color::Green(), {1.0f, 0.0f} }, // 21
		Vertex{Vertex::Position{0.5f, 0.5f, 0.5f},   Vertex::Color::Blue(),  {0.0f, 0.0f} }, // 22
		Vertex{Vertex::Position{0.5f, 0.5f, -0.5f},  Vertex::Color::White(), {0.0f, 1.0f} }, // 23
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
	m_vertices = m_pMesh->GetVertexData();
	auto pos = m_pTransform->GetPosition();
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].position.x += pos.x;
		m_vertices[i].position.y += pos.y;
		m_vertices[i].position.z += pos.z;
	}

	return m_vertices;
}

std::vector<uint16_t>& CCube::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
