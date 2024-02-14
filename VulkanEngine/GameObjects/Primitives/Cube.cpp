#include "Cube.h"

constexpr float F_ONE = 1.0f;
constexpr float F_ZERO = 0.0f;
constexpr float F_N_DOT_FIVE= -0.5f;
constexpr float F_P_DOT_FIVE= 0.5f;

const std::vector<Vertex> verticies = {
		// Front
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Red(),  Vertex::Normal{F_ZERO, F_ZERO, -F_ONE}, {F_ONE, F_ONE} }, // 0
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Green(),Vertex::Normal{F_ZERO, F_ZERO, -F_ONE},{F_ZERO, F_ONE} }, // 1
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), Vertex::Normal{F_ZERO, F_ZERO, -F_ONE},{F_ZERO, F_ZERO} }, // 2
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),Vertex::Normal{F_ZERO, F_ZERO, -F_ONE},{F_ONE, F_ZERO} }, // 3

		// Back
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  Vertex::Normal{F_ZERO, F_ZERO, F_ONE},{F_ONE, F_ONE}},   // 4
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),Vertex::Normal{F_ZERO, F_ZERO, F_ONE},{F_ZERO, F_ONE}},   // 5
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Blue(), Vertex::Normal{F_ZERO, F_ZERO, F_ONE},{F_ZERO, F_ZERO}},   // 6
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::White(),Vertex::Normal{F_ZERO, F_ZERO, F_ONE},{F_ONE, F_ZERO}},   // 7

		// Top
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ONE, F_ONE} }, // 8
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ZERO, F_ONE} }, // 9
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ZERO, F_ZERO} }, // 10
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ONE, F_ZERO} }, // 11

		// Bottom
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  Vertex::Normal{F_ZERO, F_ONE, F_ZERO},{F_ONE, F_ONE} }, // 12
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),Vertex::Normal{F_ZERO, F_ONE, F_ZERO},{F_ZERO, F_ONE} }, // 13
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), Vertex::Normal{F_ZERO, F_ONE, F_ZERO},{F_ZERO, F_ZERO} }, // 14
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),Vertex::Normal{F_ZERO, F_ONE, F_ZERO},{F_ONE, F_ZERO} }, // 15

		// Right
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Red(),  Vertex::Normal{-F_ONE, F_ZERO, F_ZERO},{F_ONE, F_ONE} }, // 16
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Green(),Vertex::Normal{-F_ONE, F_ZERO, F_ZERO},{F_ZERO, F_ONE} }, // 17
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Blue(), Vertex::Normal{-F_ONE, F_ZERO, F_ZERO},{F_ZERO, F_ZERO} }, // 18
		Vertex{Vertex::Position{F_N_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::White(),Vertex::Normal{-F_ONE, F_ZERO, F_ZERO},{F_ONE, F_ZERO} }, // 19

		// Left
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::Red(),  Vertex::Normal{F_ONE, F_ZERO, F_ZERO},{F_ONE, F_ONE} }, // 20
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_N_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Green(),Vertex::Normal{F_ONE, F_ZERO, F_ZERO},{F_ZERO, F_ONE} }, // 21
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_P_DOT_FIVE}, Vertex::Color::Blue(), Vertex::Normal{F_ONE, F_ZERO, F_ZERO},{F_ZERO, F_ZERO} }, // 22
		Vertex{Vertex::Position{F_P_DOT_FIVE, F_P_DOT_FIVE, F_N_DOT_FIVE}, Vertex::Color::White(),Vertex::Normal{F_ONE, F_ZERO, F_ZERO},{F_ONE, F_ZERO} }, // 23
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
	MeshData data{verticies, indices };
	m_pMesh = std::make_shared<CMesh>(m_pDevice, data);

	AddComponent(m_pMesh);

	CGameObject::Initialize();
}

void CCube::Initialize(VkCommandBuffer a_commandBuffer)
{
	CGameObject::Initialize(a_commandBuffer);
}

void CCube::Update(const double& a_dDeltaTime)
{
	CGameObject::Update(a_dDeltaTime);
}

void CCube::Draw(void)
{
	CGameObject::Draw();
}

void CCube::Draw(const DrawInformation& a_drawInformation)
{
	CGameObject::Draw(a_drawInformation);
}

void CCube::Finalize()
{
	CGameObject::Finalize();
}

std::vector<Vertex>& CCube::GetMeshVertexData(void)
{
	return m_pMesh->GetVertexData();
}

std::vector<uint16_t>& CCube::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
