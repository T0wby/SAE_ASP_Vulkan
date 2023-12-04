#include "CDefaultScene.h"
#include <chrono>
#include <glm/glm/gtc/matrix_transform.hpp>

void CDefaultScene::Initialize()
{
	CScene::Initialize();
	

	m_cube = std::make_shared<CCube>();
	m_cube->Initialize();

	m_vGameObjects.push_back(m_cube);


	//m_cube2 = std::make_shared<CCube>();
	//m_cube2->Initialize();
	//m_cube2->SetPosition(glm::vec3(0.0f, 0.1f, -2.5f));

	//m_vGameObjects.push_back(m_cube2);
}

void CDefaultScene::Update(void)
{
	CScene::Update();
}

void CDefaultScene::Draw(void)
{
	CScene::Draw();
}

void CDefaultScene::Finalize(void)
{
	CScene::Finalize();
}
