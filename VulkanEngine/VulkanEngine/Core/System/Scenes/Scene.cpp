#include "Scene.h"
#include <chrono>

void CScene::Initialize(void)
{
    CreateGameObjects();
    SetupPhysxScene();
    SetupSceneInput();
    for (const auto& m_vGameObject : m_vGameObjects)
    {
        m_vGameObject->Initialize();
    }
}

// Initializes all components on all gameobjects in the scene wit a commandbuffer(Used in the mesh atm)
void CScene::Initialize(VkCommandBuffer a_commandBuffer)
{
    for (const auto& m_vGameObject : m_vGameObjects)
    {
        m_vGameObject->Initialize(a_commandBuffer);
    }
}

void CScene::Update(const double& a_dDeltaTime)
{
    m_pPlayerController->Update(a_dDeltaTime);

    for (const auto& m_vGameObject : m_vGameObjects)
    {
        m_vGameObject->Update(a_dDeltaTime);
    }
}

void CScene::Draw(void)
{
    for (const auto& m_vGameObject : m_vGameObjects)
    {
        m_vGameObject->Draw();
    }
}

void CScene::Draw(const DrawInformation& a_drawInformation)
{
    for (const auto& m_vGameObject : m_vGameObjects)
    {
        m_vGameObject->Draw(a_drawInformation);
    }
}

void CScene::Finalize(void)
{
    for (const auto& m_vGameObject : m_vGameObjects)
    {
        m_vGameObject->Finalize();
    }
}

void CScene::CreateGameObjects(void)
{
    if (m_pCameraObject != nullptr)return;
    auto camObj = CGameObject::CreateGameObject(m_pDevice);
    m_pCameraObject = std::make_shared<CGameObject>(std::move(camObj));
    m_pCamera = std::make_shared<CCamera>(static_cast<float>(m_fWidth), static_cast<float>(m_fHeight), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -0.5f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    m_pCameraObject->AddComponent(m_pCamera);
    m_pCameraObject->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
}

void CScene::SetupSceneInput(void)
{
    // Default Input is set via the Initialize method
    m_pPlayerController->Initialize(m_pWindow, m_pCameraObject, 0.0f);
}

void CScene::SetupPhysxScene(void)
{
    const auto tolScale = m_pPhysics->getTolerancesScale();
    const physx::PxSceneDesc sceneDesc{tolScale};
    m_pxScene = m_pPhysics->createScene(sceneDesc);
}

void CScene::AddGameObject(std::shared_ptr<CGameObject>& a_gameObject)
{
	if (a_gameObject == nullptr) return;

    m_vGameObjects.push_back(std::move(a_gameObject));
}

void CScene::RemoveGameObject(const std::shared_ptr<CGameObject>& a_gameObject)
{
    if (a_gameObject == nullptr) return;


    for (int i = 0; i < m_vGameObjects.size(); i++)
    {
        if (a_gameObject == m_vGameObjects[i])
        {
            m_vGameObjects.erase(m_vGameObjects.begin() + i);
            break;
        }
    }
}

std::shared_ptr<CGameObject> CScene::GetGameObject(const int& a_iIndex)
{
    if (m_vGameObjects.size() < a_iIndex || a_iIndex < 0) return nullptr;

    return m_vGameObjects[a_iIndex];
}

UniformBufferObject& CScene::CreateUniformBuffer(void)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.view = m_pCamera->GetViewMatrix(m_pCameraObject->GetPosition());
    ubo.proj = m_pCamera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;
    ubo.lightPosition = glm::vec3(1.0f,3.0f,-1.0f);
    ubo.lightColor = glm::vec4(1.0f);

    return ubo;
}

void CScene::UpdateSizeValues(const int& a_iWidth, const int& a_iHeight)
{
    m_fWidth = static_cast<float>(a_iWidth);
    m_fHeight = static_cast<float>(a_iHeight);
    m_pCamera->UpdateSizeValues(a_iWidth, a_iHeight);
}
