#include "Utility.h"
#include <fstream>

std::vector<char> CUtility::ReadFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) 
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkCommandBuffer CUtility::BeginSingleTimeCommands(const VkDevice& a_logicalDevice, const VkCommandPool& a_commandPool)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = a_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(a_logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void CUtility::EndSingleTimeCommands(const VkCommandBuffer& a_commandBuffer, const VkQueue& a_graphicsQueue, const VkCommandPool& a_commandPool, const VkDevice& a_logicalDevice)
{
    vkEndCommandBuffer(a_commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &a_commandBuffer;

    vkQueueSubmit(a_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(a_graphicsQueue);

    vkFreeCommandBuffers(a_logicalDevice, a_commandPool, 1, &a_commandBuffer);
}

stbi_uc* CUtility::LoadTextureFromFile(const std::string& a_filename, int& a_iTexWidth, int& a_iTexHeight,
    int& a_iTexChannels)
{
    return stbi_load(a_filename.c_str(), &a_iTexWidth, &a_iTexHeight, &a_iTexChannels, STBI_rgb_alpha);
}

