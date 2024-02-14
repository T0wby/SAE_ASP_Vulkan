#include "Quad.h"
constexpr float F_ONE = 1.0f;
constexpr float F_ZERO = 0.0f;
constexpr float F_N_DOT_FIVE= -0.5f;
constexpr float F_P_DOT_FIVE= 0.5f;

const std::vector<Vertex> verticies = {
		// Front
		Vertex{Vertex::Position{-F_ONE, F_ZERO, -F_ONE}, Vertex::Color::Red(),  Vertex::Normal{F_ZERO, -F_ONE, F_ZERO}, {F_ONE, F_ONE} }, // 0
		Vertex{Vertex::Position{-F_ONE, F_ZERO, F_ONE}, Vertex::Color::Green(),Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ZERO, F_ONE} }, // 1
		Vertex{Vertex::Position{F_ONE, F_ZERO, F_ONE}, Vertex::Color::Blue(), Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ZERO, F_ZERO} }, // 2
		Vertex{Vertex::Position{F_ONE, F_ZERO, -F_ONE}, Vertex::Color::White(),Vertex::Normal{F_ZERO, -F_ONE, F_ZERO},{F_ONE, F_ZERO} }, // 3
		} ;

const std::vector<uint16_t> indices = { 
	0, 1, 2, 
	2, 3, 0
};

CQuad::~CQuad()
{
}

void CQuad::Initialize(void)
{
	MeshData data{verticies, indices };
	m_pMesh = std::make_shared<CMesh>(m_pDevice, data);

	AddComponent(m_pMesh);

	CGameObject::Initialize();
}

void CQuad::Initialize(VkCommandBuffer a_commandBuffer)
{
	CGameObject::Initialize(a_commandBuffer);
}

void CQuad::Update(const double& a_dDeltaTime)
{
	CGameObject::Update(a_dDeltaTime);
}

void CQuad::Draw(void)
{
	CGameObject::Draw();
}

void CQuad::Draw(const DrawInformation& a_drawInformation)
{
	CGameObject::Draw(a_drawInformation);
}

void CQuad::Finalize()
{
	CGameObject::Finalize();
}

std::vector<Vertex>& CQuad::GetMeshVertexData(void)
{
	return m_pMesh->GetVertexData();
}

std::vector<uint16_t>& CQuad::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
