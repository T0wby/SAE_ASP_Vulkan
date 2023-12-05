#include "Device.h"

#include <set>
#include <stdexcept>
#include <vector>

void CDevice::Initialize()
{
	PickPhysicalDevice();
	CreateLogicalDevice();
}

void CDevice::PickPhysicalDevice(void)
{
	// Get number of available physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(*m_vulkanInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	// Add the physical devices found to devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(*m_vulkanInstance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (CSwapChain::IsDeviceSuitable(device, m_surface, m_EnabledExtensions))
		{
			m_physicalDevice = device;
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("failed to find a suitable GPU!");
}

void CDevice::CreateLogicalDevice()
{
	QueueFamilyIndices indices = CSwapChain::FindQueueFamilies(m_physicalDevice, m_surface);
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;


	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float priority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		// This structure describes the number of queues we want for a single queue family
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &priority; 
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Logical Device
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_EnabledExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = m_EnabledExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	// enabledLayerCount and ppEnabledLayerNames fields of VkDeviceCreateInfo are ignored by up-to-date implementations
	if (m_bEnableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_EnabledLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = m_EnabledLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}


	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create logical device!");
	}

	// Get handle to interface with the queue later
	vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentationQueue);
}
