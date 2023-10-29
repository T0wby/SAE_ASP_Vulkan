#include "Engine.h"
#include "../../WindowGLFW/Window.h"
#include <stdexcept>
#include <optional>

// System
std::shared_ptr<CWindow> pWindow = nullptr;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const std::string NAME = "SAE_Tobi_Engine";

void CEngine::Run(void)
{
	InitializeWindow();
	InitializeVulkan();
	MainLoop();
	Cleanup();
}

void CEngine::InitializeVulkan(void)
{
	CreateVulkanInstance();
}


void CEngine::CreateVulkanInstance(void)
{
	VkApplicationInfo application_info = {};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pApplicationName = "Hello Triangle";
	application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	application_info.pEngineName = NAME.c_str();
	application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	application_info.apiVersion = VK_API_VERSION_1_2;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);


#ifdef NDEBUG
	m_bEnableValidationLayers = false;
#else
	m_bEnableValidationLayers = true;
#endif

	if (m_bEnableValidationLayers && !CheckValidationLayerSupport(m_EnabledLayers)) {
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

	

	VkResult result = vkCreateInstance(&create_info, nullptr, &m_vInstance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create VK instance!");
	}
}

bool CEngine::CheckValidationLayerSupport(const std::vector<const char*> a_enabled_layers)
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

void CEngine::InitializeWindow(void)
{
	// Create GLFW window
	pWindow = std::make_shared<CWindow>(WIDTH, HEIGHT, NAME);
	pWindow->Initialize();
}

void CEngine::MainLoop(void)
{
	while (!pWindow->GetWindowShouldClose()) 
	{
		pWindow->Update();
	}
}

void CEngine::Cleanup(void)
{
	pWindow->Finalize();
	vkDestroyDevice(m_logicelDevice, nullptr);
}

void CEngine::PickPhysicalDevice(void)
{
	// Get number of available physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_vInstance, &deviceCount, nullptr);

	if (deviceCount == 0) 
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	// Add the physical devices found to devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_vInstance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			m_physicalDevice = device;
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE) 
		throw std::runtime_error("failed to find a suitable GPU!");
}

bool CEngine::IsDeviceSuitable(VkPhysicalDevice a_device)
{
	QueueFamilyIndices indices = FindQueueFamilies(a_device);

	// Basic device properties(name, type and supported Vulkan version, etc)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(a_device, &deviceProperties);

	// Optional features like texture compression, 64 bit floats and multi viewport rendering
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(a_device, &deviceFeatures);

	// GPU needs to support geometry shaders and a certain queue family (Could implement a score system for certain features instead of just picking one)
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && indices.IsComplete();
}

QueueFamilyIndices CEngine::FindQueueFamilies(VkPhysicalDevice a_device)
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
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

void CEngine::CreateLogicalDevice(void)
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);
	VkPhysicalDeviceFeatures deviceFeatures{};

	const char* enabled_extensions[1] = { "VK_KHR_swapchain" };
	float priority = 1.0f;

	// This structure describes the number of queues we want for a single queue family
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &priority;

	// Logical Device
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.enabledExtensionCount = 1;
	deviceCreateInfo.ppEnabledExtensionNames = enabled_extensions;
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


	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicelDevice) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create logical device!");
	}

	// Get handle to interface with the queue later
	vkGetDeviceQueue(m_logicelDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
}
