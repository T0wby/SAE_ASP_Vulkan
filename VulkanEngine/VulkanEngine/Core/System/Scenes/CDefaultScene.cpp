#include "CDefaultScene.h"
#include <glm/glm/gtc/matrix_transform.hpp>

void CDefaultScene::Initialize()
{
	CScene::Initialize();
	InitGameObjects();
}

void CDefaultScene::InitGameObjects()
{
	auto cube = CCube::CreateGameObject(m_pDevice);
	m_pCube = std::make_shared<CCube>(std::move(cube));
	m_pCube->Initialize();
	m_vGameObjects.push_back(std::move(m_pCube));

	cube = CCube::CreateGameObject(m_pDevice);
	m_pCube2 = std::make_shared<CCube>(std::move(cube));
	m_pCube2->Initialize();
	m_pCube2->SetPosition(glm::vec3(1.0f, 1.0f,-2.0f));
	m_pCube2->SetRotation(glm::vec3(100.0f, 55.0f,128.0f));
	m_vGameObjects.push_back(std::move(m_pCube2));
	
	cube = CCube::CreateGameObject(m_pDevice);
	m_pCube3 = std::make_shared<CCube>(std::move(cube));
	m_pCube3->Initialize();
	m_pCube3->SetPosition(glm::vec3(-0.5f, 2.0f,-1.0f));
	m_pCube3->SetRotation(glm::vec3(90.0f, 55.0f,255.0f));
	m_pCube3->SetScale(glm::vec3(0.5f, 0.5f,0.5f));
	m_vGameObjects.push_back(std::move(m_pCube3));

	auto floor = CQuad::CreateGameObject(m_pDevice);
	m_pFloor = std::make_shared<CQuad>(std::move(floor));
	m_pFloor->Initialize();
	m_pFloor->SetPosition(glm::vec3(0.0f, -0.5f,0.0f));
	m_pFloor->SetScale(glm::vec3(5.0f, 0.0f,5.0f));
	//m_pFloor->SetRotation(glm::vec3(62.0f, 0.0f,0.0f));
	m_vGameObjects.push_back(std::move(m_pFloor));

	auto light = CGameObject::CreateGameObject(m_pDevice);
	m_pLightObject = std::make_shared<CGameObject>(std::move(light));
	m_pLightObject->SetPosition(glm::vec3(1.0f,2.0f,-1.0f));
	m_vGameObjects.push_back(std::move(m_pLightObject));
	
	//auto loaded = CLoadedCube::CreateGameObject(m_pDevice);
	//m_pCubeLoad = std::make_shared<CLoadedCube>(std::move(loaded));
	//m_pCubeLoad->Initialize();
	//m_pCubeLoad->SetPosition(glm::vec3(-1.5f, 2.0f,1.0f));
	//m_pCubeLoad->SetScale(glm::vec3(0.7f, 0.7f,0.7f));
	//m_vGameObjects.push_back(std::move(m_pCubeLoad));

}

void CDefaultScene::Initialize(VkCommandBuffer a_commandBuffer)
{
	CScene::Initialize(a_commandBuffer);
}

void CDefaultScene::Update(const double& a_dDeltaTime)
{
	m_vGameObjects[1]->SetRotation(glm::vec3(1.0f + static_cast<float>(glfwGetTime()),static_cast<float>(glfwGetTime()), 0.0f));
	CScene::Update(a_dDeltaTime);
}

void CDefaultScene::Draw(void)
{
	CScene::Draw();
}

void CDefaultScene::Draw(const DrawInformation& a_drawInformation)
{
	CScene::Draw(a_drawInformation);
}

void CDefaultScene::Finalize(void)
{
	CScene::Finalize();
}

UniformBufferObject& CDefaultScene::CreateUniformBuffer()
{
	UniformBufferObject ubo{};
	ubo.model = glm::mat4(1.0f);
	ubo.view = m_pCamera->GetViewMatrix(m_pCameraObject->GetPosition());
	ubo.proj = m_pCamera->GetProjectionMatrix();
	ubo.proj[1][1] *= -1;
	ubo.lightPosition = m_vGameObjects[4]->GetPosition();
	ubo.lightColor = glm::vec4(1.0f);

	return ubo;
}

