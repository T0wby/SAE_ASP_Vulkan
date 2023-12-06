#include "Cube.h"

constexpr float F_ONE = 1.0f;
constexpr float F_ZERO = 0.0f;
constexpr float F_N_DOT_FIVE= -0.5f;
constexpr float F_P_DOT_FIVE= 0.5f;

const std::vector<Vertex> verticies = {
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Red(),  {F_ONE, F_ONE} }, // 0
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Green(),{F_ZERO, F_ONE} }, // 1
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), {F_ZERO, F_ZERO} }, // 2
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),{F_ONE, F_ZERO} }, // 3

		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  {F_ONE, F_ONE}},   // 4
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),{F_ZERO, F_ONE}},   // 5
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Blue(), {F_ZERO, F_ZERO}},   // 6
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::White(),{F_ONE, F_ZERO}},   // 7

		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  {F_ONE, F_ONE} }, // 8
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),{F_ZERO, F_ONE} }, // 9
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), {F_ZERO, F_ZERO} }, // 10
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),{F_ONE, F_ZERO} }, // 11

		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  {F_ONE, F_ONE} }, // 12
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),{F_ZERO, F_ONE} }, // 13
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), {F_ZERO, F_ZERO} }, // 14
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),{F_ONE, F_ZERO} }, // 15
		
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  {F_ONE, F_ONE} }, // 16
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Green(),{F_ZERO, F_ONE} }, // 17
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), {F_ZERO, F_ZERO} }, // 18
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::White(),{F_ONE, F_ZERO} }, // 19
		
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Red(),  {F_ONE, F_ONE} }, // 20
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),{F_ZERO, F_ONE} }, // 21
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Blue(), {F_ZERO, F_ZERO} }, // 22
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),{F_ONE, F_ZERO} }, // 23
		} ;

const std::vector<uint16_t> indices = { 
	2, 1, 0, 
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

CCube::~CCube()
{
}

void CCube::Initialize(void)
{
	m_pMesh = std::make_shared<CMesh>(m_pDevice, verticies, indices);

	AddComponent(m_pMesh);

	CGameObject::Initialize();
}

void CCube::Initialize(VkCommandBuffer a_commandBuffer)
{
	CGameObject::Initialize(a_commandBuffer);
}

void CCube::Update(void)
{
	CGameObject::Update();
}

void CCube::Draw(void)
{
	CGameObject::Draw();
}

void CCube::Draw(VkCommandBuffer a_commandBuffer)
{
	CGameObject::Draw(a_commandBuffer);
}

void CCube::Finalize()
{
	CGameObject::Finalize();
}

std::vector<Vertex>& CCube::GetMeshVertexData(void)
{
	m_vertices = m_pMesh->GetVertexData();
	auto pos = m_pTransform->GetPosition();
	for (auto& m_vertice : m_vertices)
	{
		m_vertice.position.x += pos.x;
		m_vertice.position.y += pos.y;
		m_vertice.position.z += pos.z;
	}

	return m_vertices;
}

std::vector<uint16_t>& CCube::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
