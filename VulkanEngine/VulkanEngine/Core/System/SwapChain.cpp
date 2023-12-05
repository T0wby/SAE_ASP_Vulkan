#include "SwapChain.h"
#include <iostream>
#include <set>

bool CSwapChain::IsDeviceSuitable(VkPhysicalDevice a_device, VkSurfaceKHR a_surface, const std::vector<const char*> a_enabledExtensions)
{
	QueueFamilyIndices indices = FindQueueFamilies(a_device, a_surface);

	// Basic device properties(name, type and supported Vulkan version, etc)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(a_device, &deviceProperties);

	std::cout << "Device name: " << deviceProperties.deviceName << std::endl;
	std::cout << "Device type: " << deviceProperties.deviceType << std::endl;
	std::cout << "API version: " << deviceProperties.apiVersion << std::endl;

	// Optional features like texture compression, 64 bit floats and multi viewport rendering
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(a_device, &deviceFeatures);

	// Check supported extensions
	bool extensionsSupported = CheckDeviceExtensionSupport(a_device, a_enabledExtensions);
	bool swapChainAdequate = false;

	if (extensionsSupported)
	{
		// Check if the swapchain has at least one supported image format and one supported presentation mode
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(a_device, a_surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	// GPU needs to support geometry shaders, a certain queue family and certain Extensions
	// (Could implement a score system for certain features instead of just picking one)
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader &&
		indices.IsComplete() &&
		extensionsSupported &&
		swapChainAdequate &&
		deviceFeatures.samplerAnisotropy;
}

QueueFamilyIndices CSwapChain::FindQueueFamilies(VkPhysicalDevice a_device, VkSurfaceKHR a_surface)
{
	QueueFamilyIndices indices{};

	// Get amount of available queue families on the physical device
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(a_device, &queueFamilyCount, nullptr);



	// The VkQueueFamilyProperties struct contains some details about the queue family, 
	// including the type of operations that are supported and the number of queues that can be created based on that family.
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(a_device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		// Check if the queue family has the capability of presenting to our window surface
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(a_device, i, a_surface, &presentSupport);
		if (presentSupport)
			indices.presentFamily = i;

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

SwapChainSupportDetails CSwapChain::QuerySwapChainSupport(VkPhysicalDevice a_device, VkSurfaceKHR a_surface)
{
	SwapChainSupportDetails scsDetails;

	// Get the surface capabilities associated to the current physical device and created surface
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a_device, a_surface, &scsDetails.capabilities);

	// Get the supported surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(a_device, a_surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		scsDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(a_device, a_surface, &formatCount, scsDetails.formats.data());
	}

	// Get the supported presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(a_device, a_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		scsDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(a_device, a_surface, &presentModeCount, scsDetails.presentModes.data());
	}

	return scsDetails;
}

bool CSwapChain::CheckDeviceExtensionSupport(VkPhysicalDevice a_device, const std::vector<const char*> a_EnabledExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(a_device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtentions(extensionCount);
	vkEnumerateDeviceExtensionProperties(a_device, nullptr, &extensionCount, availableExtentions.data());

	// Create unique container out of the extentions that we want to use
	std::set<std::string> requiredExtensions(a_EnabledExtensions.begin(), a_EnabledExtensions.end());

	// Erase the required extension if it matches with an available one
	for (const auto& extension : availableExtentions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
