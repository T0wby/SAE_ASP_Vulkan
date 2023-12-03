#include "Scene.h"
#include <stdexcept>
#include <chrono>
#include <glm/glm/gtc/matrix_transform.hpp>

void CScene::Initialize(void)
{
    CreateGameObjects();
}

void CScene::Update(void)
{
}

void CScene::Draw(void)
{
}

void CScene::Finalize(void)
{
}

void CScene::CreateGameObjects(void)
{
    m_cube = std::make_shared<CCube>();
    m_cube->Initialize();
    m_vGameObjects.push_back(m_cube);

    m_camera = std::make_shared<CCamera>(static_cast<float>(m_fWidth), static_cast<float>(m_fHeight), glm::vec3(-1.0f, 0.1f, 2.5f), glm::vec3(0.0f, 0.0f, -0.5f),
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void CScene::AddGameObject(std::shared_ptr<CGameObject> a_gameObject)
{
	if (a_gameObject == nullptr) return;

    m_vGameObjects.push_back(a_gameObject);
}

void CScene::RemoveGameObject(std::shared_ptr<CGameObject> a_gameObject)
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

auto CScene::GetSceneVertexCount(void) const -> const int
{
    std::vector<std::vector<Vertex>> vertices{};

    for (int i = 0; i < m_vGameObjects.size(); i++)
    {
        vertices.push_back(m_vGameObjects[i]->GetMeshVertexData());
    }

    int vertexCount{ 0 };
    for (int i = 0; i < vertices.size(); i++)
    {
        vertexCount += vertices[i].size();
    }

    return vertexCount;
}

auto CScene::GetSceneFirstVertex(void) const -> const std::vector<Vertex>
{
    if (m_vGameObjects.size() < 1)
    {
        throw std::runtime_error("Scene doesn't have any gameobjects");
    }

    return m_vGameObjects[0]->GetMeshVertexData();
}

auto CScene::GetSceneIndicesCount(void) const -> const int
{
    std::vector<std::vector<uint16_t>> indices{};

    for (int i = 0; i < m_vGameObjects.size(); i++)
    {
        indices.push_back(m_vGameObjects[i]->GetMeshIndiceData());
    }

    int indicesCount{ 0 };
    for (int i = 0; i < indices.size(); i++)
    {
        indicesCount += indices[i].size();
    }

    return indicesCount;
}

auto CScene::GetSceneFirstIndice(void) const -> const std::vector<uint16_t>
{
    if (m_vGameObjects.size() < 1)
    {
        throw std::runtime_error("Scene doesn't have any gameobjects");
    }

    return m_vGameObjects[0]->GetMeshIndiceData();
}

UniformBufferObject& CScene::CreateUniformBuffer(void)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};

    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));;
    ubo.view = m_camera->GetViewMatrix();
    ubo.proj = m_camera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    return ubo;
}
