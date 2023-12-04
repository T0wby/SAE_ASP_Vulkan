#include "CDefaultScene.h"

void CDefaultScene::Initialize()
{
	CScene::Initialize();
	m_cube = std::make_shared<CCube>();
	m_cube->Initialize();


	m_vGameObjects.push_back(m_cube);
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
