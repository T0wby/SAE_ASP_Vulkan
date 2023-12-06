#ifndef UTILITY_H
#define UTILITY_H
#include <memory>
#include <vector>
#include <string>
#include <Vulkan/Include/vulkan/vulkan_core.h>
#include "../Core/System/Device.h"

class CUtility
{
public:
	static std::vector<char> ReadFile(const std::string& filename);
	static VkCommandBuffer BeginSingleTimeCommands(const VkDevice& a_logicalDevice, const VkCommandPool& a_commandPool);
	static void EndSingleTimeCommands(VkCommandBuffer a_commandBuffer, const VkQueue& a_graphicsQueue, const VkCommandPool& a_commandPool, const VkDevice& a_logicalDevice);
};

#endif