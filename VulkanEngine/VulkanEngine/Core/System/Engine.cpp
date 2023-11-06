#include "Engine.h"
#include "../../WindowGLFW/Window.h"
#include "../../Utility/Utility.h"
#include <stdexcept>
#include <optional>
#include <iostream>
#include <set>
#include <limits>
#include <algorithm>

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
	CreateGLFWSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateImageViews();
	CreateGraphicsPipeline();
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

bool CEngine::CheckDeviceExtensionSupport(VkPhysicalDevice a_device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(a_device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtentions(extensionCount);
	vkEnumerateDeviceExtensionProperties(a_device, nullptr, &extensionCount, availableExtentions.data());

	// Create unique container out of the extentions that we want to use
	std::set<std::string> requiredExtensions(m_EnabledExtensions.begin(), m_EnabledExtensions.end());

	// Erase the required extension if it matches with an available one
	for (const auto& extension : availableExtentions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
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
	vkDestroySwapchainKHR(m_logicelDevice, m_swapChain, nullptr);
	vkDestroySurfaceKHR(m_vInstance, m_surface, nullptr);
	DestroyImageViews();
	vkDestroyDevice(m_logicelDevice, nullptr);
	vkDestroyInstance(m_vInstance, nullptr);
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

	std::cout << "Device name: " << deviceProperties.deviceName << std::endl;
	std::cout << "Device type: " << deviceProperties.deviceType << std::endl;
	std::cout << "API version: " << deviceProperties.apiVersion << std::endl;

	// Optional features like texture compression, 64 bit floats and multi viewport rendering
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(a_device, &deviceFeatures);

	// Check supported extensions
	bool extensionsSupported = CheckDeviceExtensionSupport(a_device); 
	bool swapChainAdequate = false;

	if (extensionsSupported)
	{
		// Check if the swapchain has at least one supported image format and one supported presentation mode
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(a_device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	// GPU needs to support geometry shaders, a certain queue family and certain Extensions
	// (Could implement a score system for certain features instead of just picking one)
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && 
		deviceFeatures.geometryShader && 
		indices.IsComplete() && 
		extensionsSupported &&
		swapChainAdequate;
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
	for (const auto& queueFamily : queueFamilies) 
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		// Check if the queue family has the capability of presenting to our window surface
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(a_device, i, m_surface, &presentSupport);
		if (presentSupport)
			indices.presentFamily = i;

		if (indices.IsComplete())
			break;

		i++;
	}

	return indices;
}

SwapChainSupportDetails CEngine::QuerySwapChainSupport(VkPhysicalDevice a_device)
{
	SwapChainSupportDetails scsDetails;

	// Get the surface capabilities associated to the current physical device and created surface
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a_device, m_surface, &scsDetails.capabilities);

	// Get the supported surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(a_device, m_surface, &formatCount, nullptr);

	if (formatCount != 0) 
	{
		scsDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(a_device, m_surface, &formatCount, scsDetails.formats.data());
	}

	// Get the supported presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(a_device, m_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) 
	{
		scsDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(a_device, m_surface, &presentModeCount, scsDetails.presentModes.data());
	}

	return scsDetails;
}

VkSurfaceFormatKHR CEngine::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	// See if the optimal Format is available otherwise we just take the first one
	for (const auto& availableFormat : availableFormats) 
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR CEngine::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentationModes)
{
	/*
	* VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by your application are transferred to the screen right away, which may result in tearing.
	* 
	* VK_PRESENT_MODE_FIFO_KHR: The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed 
	* and the program inserts rendered images at the back of the queue. If the queue is full then the program has to wait. This is most similar to vertical sync
	* as found in modern games. The moment that the display is refreshed is known as "vertical blank".
	* 
	* VK_PRESENT_MODE_FIFO_RELAXED_KHR: This mode only differs from the previous one if the application is late and the queue was empty at the last vertical blank. 
	* Instead of waiting for the next vertical blank, the image is transferred right away when it finally arrives. This may result in visible tearing.
	* 
	* VK_PRESENT_MODE_MAILBOX_KHR: This is another variation of the second mode. Instead of blocking the application when the queue is full, the images that are 
	* already queued are simply replaced with the newer ones. This mode can be used to render frames as fast as possible while still avoiding tearing, resulting in 
	* fewer latency issues than standard vertical sync. This is commonly known as "triple buffering", although the existence of three buffers alone does not necessarily 
	* mean that the framerate is unlocked.
	*/

	for (const auto& availablePresentMode : availablePresentationModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D CEngine::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		pWindow->GetWindowFrameBufferSize(width, height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void CEngine::CreateLogicalDevice(void)
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);
	VkPhysicalDeviceFeatures deviceFeatures{};


	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
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


	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicelDevice) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create logical device!");
	}

	// Get handle to interface with the queue later
	vkGetDeviceQueue(m_logicelDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicelDevice, indices.presentFamily.value(), 0, &m_presentationQueue);
}

void CEngine::CreateGLFWSurface(void)
{
	if (glfwCreateWindowSurface(m_vInstance, pWindow->GetWindow(), nullptr, &m_surface) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void CEngine::CreateSwapChain(void)
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	m_swapChainExtent = ChooseSwapExtent(swapChainSupport.capabilities);

	m_swapChainImageFormat = surfaceFormat.format;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = m_swapChainExtent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //  specifies what kind of operations we'll use the images in the swap chain for

	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE; // Swapchain can get invalid during runtime for example because the window was resized(Solution done later)

	if (vkCreateSwapchainKHR(m_logicelDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	///////////////// Retrieving the swap chain images //////////////////////////
	
	vkGetSwapchainImagesKHR(m_logicelDevice, m_swapChain, &imageCount, nullptr); // query the final number of images
	m_vSwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_logicelDevice, m_swapChain, &imageCount, m_vSwapChainImages.data());
}

void CEngine::CreateImageViews(void)
{
	m_vSwapChainImageViews.resize(m_vSwapChainImages.size());
	for (size_t i = 0; i < m_vSwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = m_vSwapChainImages[i];
		// The viewType parameter allows you to treat images as 1D textures, 2D textures, 3D textures and cube maps
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = m_swapChainImageFormat;
		// Default component mapping
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		// Purpose of the image and which part of the image should be accessed
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_logicelDevice, &createInfo, nullptr, &m_vSwapChainImageViews[i]) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create image views!");
		}
	}


}

void CEngine::DestroyImageViews(void)
{
	for (auto imageView : m_vSwapChainImageViews)
	{
		vkDestroyImageView(m_logicelDevice, imageView, nullptr);
	}
}

void CEngine::CreateGraphicsPipeline(void)
{
	auto vertShaderCode = CUtility::ReadFile("Shader/vert.spv");
	auto fragShaderCode = CUtility::ReadFile("Shader/frag.spv");

	// Wrapper for  SPIR-V bytecode
	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	// Shader Stage
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Destroy wrapper after Pipeline is created since they are not needed anymore
	vkDestroyShaderModule(m_logicelDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(m_logicelDevice, vertShaderModule, nullptr);
}

/// <summary>
/// Takes a buffer with the bytecode as parameter and create a VkShaderModule from it.
/// </summary>
/// <param name="code">buffer with the bytecode</param>
/// <returns>Shader module</returns>
VkShaderModule CEngine::CreateShaderModule(const std::vector<char>& a_vBytecode)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = a_vBytecode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(a_vBytecode.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(m_logicelDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}
