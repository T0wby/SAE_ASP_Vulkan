#include "Device.h"

#include <set>
#include <stdexcept>
#include <vector>
#include "SwapChain.h"
#include "../../Utility/Utility.h"

const std::string NAME = "SAE_Tobi_Engine";
const std::string APPLICATION_NAME = "SAE_ASP_Engine";

CDevice::~CDevice()
{
	vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
	vkDestroyDevice(m_logicalDevice, nullptr);

	vkDestroySurfaceKHR(*m_vulkanInstance, m_surface, nullptr);
	vkDestroyInstance(*m_vulkanInstance, nullptr);
}

void CDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
	VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_logicalDevice, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(m_logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(m_logicalDevice, buffer, bufferMemory, 0);
}

void CDevice::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	const VkCommandBuffer commandBuffer = CUtility::BeginSingleTimeCommands(m_logicalDevice, m_commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	CUtility::EndSingleTimeCommands(commandBuffer,m_graphicsQueue ,m_commandPool ,m_logicalDevice);
}

uint32_t CDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	// query info about the available types of memory
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	// VkMemoryRequirements::memoryTypeBits is a bitfield that sets a bit for every memoryType that is
	// supported for the resource.Therefore we need to check if the bit at index i is set while also testing the
	// required memory property flags while iterating over the memory types.
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void CDevice::CreateVulkanInstance()
{
	VkApplicationInfo application_info = {};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pApplicationName = APPLICATION_NAME.c_str();
	application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	application_info.pEngineName = NAME.c_str();
	application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	application_info.apiVersion = VK_API_VERSION_1_2;

	uint32_t glfwExtensionCount = 0;

	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


#ifdef NDEBUG
	m_bEnableValidationLayers = false;
#else
	m_bEnableValidationLayers = true;
#endif

	if (m_bEnableValidationLayers && !CheckValidationLayerSupport(m_EnabledLayers))
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	// Configuration Structs are a common pattern in VK, later used to create the instance
	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	// Extra Application Info(Here for API version used)
	create_info.pApplicationInfo = &application_info;
	// Adding used Extensions
	create_info.enabledExtensionCount = glfwExtensionCount;
	create_info.ppEnabledExtensionNames = glfwExtensions;

	if (m_bEnableValidationLayers)
	{
		// Enable a Layer(validation)
		create_info.enabledLayerCount = static_cast<uint32_t>(m_EnabledLayers.size());
		create_info.ppEnabledLayerNames = m_EnabledLayers.data();
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}

	m_vulkanInstance = std::make_shared<VkInstance>();

	if (vkCreateInstance(&create_info, nullptr, m_vulkanInstance.get()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create VK instance!");
	}
}

void CDevice::CreateSurface()
{
	m_pWindow->CreateWindowSurface(*m_vulkanInstance, m_surface);
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

void CDevice::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = CSwapChain::FindQueueFamilies(m_physicalDevice, m_surface);

	/* There are two possible flags for command pools:
	* VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
	* VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
	*/

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(m_logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

bool CDevice::CheckValidationLayerSupport(const std::vector<const char*>& a_enabled_layers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check if we find the layers
	for (const char* layerName : a_enabled_layers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}
