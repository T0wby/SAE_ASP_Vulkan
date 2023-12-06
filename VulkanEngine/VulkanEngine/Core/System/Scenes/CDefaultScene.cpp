#include "CDefaultScene.h"
#include <chrono>
#include <glm/glm/gtc/matrix_transform.hpp>

void CDefaultScene::Initialize()
{
	CScene::Initialize();
	

	m_pCube = std::make_shared<CCube>(m_pDevice);
	m_pCube->Initialize();

	m_vGameObjects.push_back(m_pCube);


	//m_cube2 = std::make_shared<CCube>();
	//m_cube2->Initialize();
	//m_cube2->SetPosition(glm::vec3(0.0f, 0.1f, -2.5f));

	//m_vGameObjects.push_back(m_cube2);
}

void CDefaultScene::Initialize(VkCommandBuffer a_commandBuffer)
{
	CScene::Initialize(a_commandBuffer);
}

void CDefaultScene::Update(void)
{
	CScene::Update();
}

void CDefaultScene::Draw(void)
{
	CScene::Draw();
}

void CDefaultScene::Draw(VkCommandBuffer a_commandBuffer)
{
	CScene::Draw(a_commandBuffer);
}

void CDefaultScene::Finalize(void)
{
	CScene::Finalize();
}
