#ifndef UTILITY_H
#define UTILITY_H
#include <stb_image.h>
#include <vector>
#include <string>
#include <Vulkan/Include/vulkan/vulkan_core.h>

class CUtility
{
public:
	static std::vector<char> ReadFile(const std::string& filename);
	static VkCommandBuffer BeginSingleTimeCommands(const VkDevice& a_logicalDevice, const VkCommandPool& a_commandPool);
	static void EndSingleTimeCommands(const VkCommandBuffer& a_commandBuffer, const VkQueue& a_graphicsQueue, const VkCommandPool& a_commandPool, const VkDevice& a_logicalDevice);
	static stbi_uc* LoadTextureFromFile(const std::string& a_filename, int& a_iTexWidth, int& a_iTexHeight, int& a_iTexChannels);
};

#endif