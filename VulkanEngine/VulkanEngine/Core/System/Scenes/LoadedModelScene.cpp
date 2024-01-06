#include "LoadedModelScene.h"

void CLoadedModelScene::Initialize()
{
	CScene::Initialize();
	InitGameObjects();
}

void CLoadedModelScene::InitGameObjects()
{
	auto light = CGameObject::CreateGameObject(m_pDevice);
	m_pLightObject = std::make_shared<CGameObject>(std::move(light));
	m_pLightObject->SetPosition(glm::vec3(2.0f,3.0f,0.0f));
	m_vGameObjects.push_back(std::move(m_pLightObject));
	
	auto loaded = CLoadedCube::CreateGameObject(m_pDevice);
	m_pVaseLoad = std::make_shared<CLoadedCube>(std::move(loaded));
	m_pVaseLoad->Initialize();
	m_pVaseLoad->SetPosition(glm::vec3(0.0f, 0.5f,0.0f));
	m_pVaseLoad->SetRotation(glm::vec3(110.0f, 0.0f,0.0f));
	m_pVaseLoad->SetScale(glm::vec3(2.0f, 2.0f,2.0f));
	m_vGameObjects.push_back(std::move(m_pVaseLoad));

}

void CLoadedModelScene::Initialize(VkCommandBuffer a_commandBuffer)
{
	CScene::Initialize(a_commandBuffer);
}

void CLoadedModelScene::Update(const double& a_dDeltaTime)
{
	//m_vGameObjects[1]->SetRotation(glm::vec3(1.0f + static_cast<float>(glfwGetTime()),static_cast<float>(glfwGetTime()), 0.0f));
	CScene::Update(a_dDeltaTime);
}

void CLoadedModelScene::Draw(void)
{
	CScene::Draw();
}

void CLoadedModelScene::Draw(const DrawInformation& a_drawInformation)
{
	CScene::Draw(a_drawInformation);
}

void CLoadedModelScene::Finalize(void)
{
	CScene::Finalize();
}

UniformBufferObject& CLoadedModelScene::CreateUniformBuffer()
{
	UniformBufferObject ubo{};
	ubo.model = glm::mat4(1.0f);
	ubo.view = m_pCamera->GetViewMatrix(m_pCameraObject->GetPosition());
	ubo.proj = m_pCamera->GetProjectionMatrix();
	ubo.proj[1][1] *= -1;
	ubo.lightPosition = m_vGameObjects[0]->GetPosition();
	ubo.lightColor = glm::vec4(1.0f);

	return ubo;
}

