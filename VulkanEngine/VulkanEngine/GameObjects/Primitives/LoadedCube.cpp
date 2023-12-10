#include "LoadedCube.h"

constexpr float F_ONE = 1.0f;
constexpr float F_ZERO = 0.0f;
constexpr float F_N_DOT_FIVE= -0.5f;
constexpr float F_P_DOT_FIVE= 0.5f;


CLoadedCube::~CLoadedCube()
{
}

void CLoadedCube::Initialize(void)
{
	MeshData data{};
	m_pMesh = CMesh::CreateMeshFromFile(m_pDevice, "Models/colored_cube.obj", data);

	AddComponent(m_pMesh);

	CGameObject::Initialize();
}

void CLoadedCube::Initialize(VkCommandBuffer a_commandBuffer)
{
	CGameObject::Initialize(a_commandBuffer);
}

void CLoadedCube::Update(const double& a_dDeltaTime)
{
	CGameObject::Update(a_dDeltaTime);
}

void CLoadedCube::Draw(void)
{
	CGameObject::Draw();
}

void CLoadedCube::Draw(const DrawInformation& a_drawInformation)
{
	CGameObject::Draw(a_drawInformation);
}

void CLoadedCube::Finalize()
{
	CGameObject::Finalize();
}

std::vector<Vertex>& CLoadedCube::GetMeshVertexData(void)
{
	return m_pMesh->GetVertexData();
}

std::vector<uint16_t>& CLoadedCube::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
