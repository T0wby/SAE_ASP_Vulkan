#include "CDefaultScene.h"
#include <glm/glm/gtc/matrix_transform.hpp>

void CDefaultScene::Initialize()
{
	CScene::Initialize();
	

	m_pCube = std::make_shared<CCube>(m_pDevice);
	m_pCube->Initialize();
	m_vGameObjects.push_back(m_pCube);
	
	m_pCube2 = std::make_shared<CCube>(m_pDevice);
	m_pCube2->Initialize();
	m_pCube2->AddPosition(glm::vec3(1.0f, 1.0f,-2.0f));
	m_pCube2->SetRotation(glm::vec3(100.0f, 55.0f,128.0f));

	m_vGameObjects.push_back(m_pCube2);
}

void CDefaultScene::Initialize(VkCommandBuffer a_commandBuffer)
{
	CScene::Initialize(a_commandBuffer);
}

void CDefaultScene::Update(void)
{
	m_pCube2->SetRotation(glm::vec3(1.0f + static_cast<float>(glfwGetTime()),static_cast<float>(glfwGetTime()), 0.0f));
	CScene::Update();
}

void CDefaultScene::Draw(void)
{
	CScene::Draw();
}

void CDefaultScene::Draw(DrawInformation& a_drawInformation)
{
	CScene::Draw(a_drawInformation);
}

void CDefaultScene::Finalize(void)
{
	CScene::Finalize();
}
