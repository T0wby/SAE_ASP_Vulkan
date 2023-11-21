#include "Cube.h"

void CCube::Initialize(void)
{
	// Triangle
	m_pMesh->SetVertexData({
		Vertex{Vertex::Position{0.0f, -0.5f},Vertex::Color::Red()   }, // 1
		Vertex{Vertex::Position{0.5f, 0.5f}, Vertex::Color::Green() }, // 2
		Vertex{Vertex::Position{-0.5f, 0.5f}, Vertex::Color::Blue() }, // 3
		//Vertex{Vertex::Position{0.5f, 0.0f},  Vertex::Color::White()}  // 4
		});

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
