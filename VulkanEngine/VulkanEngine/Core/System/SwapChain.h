#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../Core/System/CoreSystemStructs.h"

class CSwapChain
{
public:

	static bool IsDeviceSuitable(VkPhysicalDevice a_device, VkSurfaceKHR a_surface, const std::vector<const char*> a_enabledExtensions);
	static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_device, VkSurfaceKHR a_surface);
	static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice a_device, VkSurfaceKHR a_surface);
	static bool CheckDeviceExtensionSupport(VkPhysicalDevice a_device, const std::vector<const char*> a_EnabledExtensions);

private:

};

#endif