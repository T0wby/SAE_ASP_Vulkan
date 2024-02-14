#include "LoadedVase.h"

constexpr float F_ONE = 1.0f;
constexpr float F_ZERO = 0.0f;
constexpr float F_N_DOT_FIVE= -0.5f;
constexpr float F_P_DOT_FIVE= 0.5f;


CLoadedVase::~CLoadedVase()
{
}

void CLoadedVase::Initialize(void)
{
	MeshData data{};
	m_pMesh = CMesh::CreateMeshFromFile(m_pDevice, "Models/smooth_vase.obj", data);

	AddComponent(m_pMesh);

	CGameObject::Initialize();
}

void CLoadedVase::Initialize(VkCommandBuffer a_commandBuffer)
{
	CGameObject::Initialize(a_commandBuffer);
}

void CLoadedVase::Update(const double& a_dDeltaTime)
{
	CGameObject::Update(a_dDeltaTime);
}

void CLoadedVase::Draw(void)
{
	CGameObject::Draw();
}

void CLoadedVase::Draw(const DrawInformation& a_drawInformation)
{
	CGameObject::Draw(a_drawInformation);
}

void CLoadedVase::Finalize()
{
	CGameObject::Finalize();
}

std::vector<Vertex>& CLoadedVase::GetMeshVertexData(void)
{
	return m_pMesh->GetVertexData();
}

std::vector<uint16_t>& CLoadedVase::GetMeshIndiceData(void)
{
	return m_pMesh->GetIndiceData();
}
