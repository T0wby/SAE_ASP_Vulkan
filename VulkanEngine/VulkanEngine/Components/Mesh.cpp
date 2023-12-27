#include "Mesh.h"
#include "../Utility/Utility.h"
#include <iostream>

CMesh::~CMesh(){}

std::unique_ptr<CMesh> CMesh::CreateMeshFromFile(const std::shared_ptr<CDevice>& a_pDevice,
    const std::string& a_filePath, MeshData& a_meshData)
{
    const auto assimpScene = MeshData::LoadMesh(a_filePath);
    MeshData::ProcessNode(assimpScene->mRootNode, assimpScene, a_meshData);

    std::cout << a_meshData.vertices.size() << "\n";
    return std::make_unique<CMesh>(a_pDevice, a_meshData);
}

int CMesh::Initialize(void)
{
    return 0;
}

int CMesh::Initialize(const VkCommandBuffer& a_commandBuffer)
{
    Bind(a_commandBuffer);
    
    return 0;
}

int CMesh::Update(const double& a_dDeltaTime)
{
    return 0;
}

void CMesh::Draw(void)
{
}

void CMesh::Draw(const DrawInformation& a_drawInformation)
{
    Bind(a_drawInformation.commandBuffer);
    vkCmdDrawIndexed(a_drawInformation.commandBuffer, m_iIndexCount, 1, 0, 0, 0);
}

void CMesh::Finalize(void)
{
}

void CMesh::SetVertexData(const std::vector<Vertex>& a_vertices)
{
    m_vertices = a_vertices;
}

std::vector<Vertex>& CMesh::GetVertexData(void)
{
    return m_vertices;
}

void CMesh::SetIndiceData(const std::vector<uint16_t>& a_indices)
{
    m_indices = a_indices;
}

std::vector<uint16_t>& CMesh::GetIndiceData(void)
{
    return m_indices;
}

void CMesh::CreateVertexBuffer(const std::vector<Vertex>& a_vertices)
{
    const VkDeviceSize bufferSize = sizeof(Vertex) * a_vertices.size();
    
    uint32_t vertexSize = sizeof(Vertex);
    uint32_t vertexCount = a_vertices.size();

    CBuffer stagingBuffer{
        m_pDevice,
        vertexSize,
        vertexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    stagingBuffer.Map();
    stagingBuffer.WriteToBuffer((void*)a_vertices.data());

    
    m_pVertexBuffer = std::make_unique<CBuffer>(
        m_pDevice,
        vertexSize,
        vertexCount,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

    m_pDevice->CopyBuffer(stagingBuffer.GetBuffer(), m_pVertexBuffer->GetBuffer(), bufferSize);
}

void CMesh::CreateIndexBuffer(const std::vector<uint16_t>& a_indices)
{
    m_iIndexCount = static_cast<uint32_t>(a_indices.size());

    m_bHasIndexBuffer = m_iIndexCount > 0;
    if (!m_bHasIndexBuffer) return;
    
    uint32_t indexSize = sizeof(a_indices[0]);
    const VkDeviceSize bufferSize = indexSize * m_iIndexCount;
    
    CBuffer stagingBuffer{
        m_pDevice,
        indexSize,
        m_iIndexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    stagingBuffer.Map();
    stagingBuffer.WriteToBuffer((void*)a_indices.data());

    m_pIndexBuffer = std::make_unique<CBuffer>(
            m_pDevice,
            indexSize,
            m_iIndexCount,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

    m_pDevice->CopyBuffer(stagingBuffer.GetBuffer(), m_pIndexBuffer->GetBuffer(), bufferSize);
}

void CMesh::Bind(const VkCommandBuffer& a_commandBuffer) const
{
    const VkBuffer vertexBuffers[] = { m_pVertexBuffer->GetBuffer() };
    constexpr VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(a_commandBuffer, 0, 1, vertexBuffers, offsets);

    if (m_bHasIndexBuffer)
        vkCmdBindIndexBuffer(a_commandBuffer, m_pIndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
}
