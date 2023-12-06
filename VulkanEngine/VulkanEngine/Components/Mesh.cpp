#include "Mesh.h"

#include <stdexcept>

#include "../Utility/Utility.h"

CMesh::~CMesh()
{
}

int CMesh::Initialize(void)
{
    return 0;
}

int CMesh::Initialize(VkCommandBuffer a_commandBuffer)
{
    const VkBuffer vertexBuffers[] = { m_vertexBuffer };
    constexpr VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(a_commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(a_commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    return 0;
}

int CMesh::Update(void)
{
    return 0;
}

void CMesh::Draw(void)
{
}

void CMesh::Draw(VkCommandBuffer a_commandBuffer)
{
    vkCmdDrawIndexed(a_commandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
}

void CMesh::Finalize(void)
{
    vkDestroyBuffer(m_pDevice->GetLogicalDevice(), m_indexBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDevice(), m_indexBufferMemory, nullptr);
    vkDestroyBuffer(m_pDevice->GetLogicalDevice(), m_vertexBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDevice(), m_vertexBufferMemory, nullptr);
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

    // Added staging buffer (vertex data is now being loaded from high performance memory)
    // One staging buffer in CPU accessible memory to upload the data from the vertex array to, and the final vertex buffer in device local memory(GPU).
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    // Map memory and copy vertices.data() to it(other possibility would be explicit flushing)
    vkMapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, a_vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory);

    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

    m_pDevice->CopyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    vkDestroyBuffer(m_pDevice->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void CMesh::CreateIndexBuffer(const std::vector<uint16_t>& a_indices)
{
    const VkDeviceSize bufferSize = sizeof(uint16_t) * a_indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, a_indices.data(), (size_t)bufferSize);
    vkUnmapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory);

    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

    m_pDevice->CopyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    vkDestroyBuffer(m_pDevice->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, nullptr);
}
