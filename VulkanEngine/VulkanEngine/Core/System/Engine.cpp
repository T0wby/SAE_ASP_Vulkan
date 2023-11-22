#include "Engine.h"
#include "../../WindowGLFW/Window.h"
#include "../../Utility/Utility.h"
#include "../../Utility/Variables.h"
#include "../../GameObjects/Primitives/Cube.h"
#include <stdexcept>
#include <optional>
#include <iostream>
#include <set>
#include <limits>
#include <algorithm>

// System
std::shared_ptr<CWindow> pWindow = nullptr;

//Temp
std::shared_ptr<CCube> pCube = nullptr;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const std::string NAME = "SAE_Tobi_Engine";
const int MAX_FRAMES_IN_FLIGHT = 2;

//const std::vector<uint16_t> indices = {	0, 1, 2, 2, 3, 0};


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
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFrameBuffers();
	CreateCommandPool();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateCommandBuffers();
	CreateSyncObjects();
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
		DrawFrame();
	}

	vkDeviceWaitIdle(m_logicelDevice);
}

void CEngine::DrawFrame(void)
{
	vkWaitForFences(m_logicelDevice, 1, &m_vInFlightFences[m_iCurrentFrame], VK_TRUE, UINT64_MAX);
	

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(m_logicelDevice, m_swapChain, UINT64_MAX, m_vImageAvailableSemaphores[m_iCurrentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || pWindow->IsFrameBufferResized())
	{
		pWindow->SetIsFrameBufferResized(false);
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	// Reset fence to unsigned
	vkResetFences(m_logicelDevice, 1, &m_vInFlightFences[m_iCurrentFrame]);

	vkResetCommandBuffer(m_vCommandBuffers[m_iCurrentFrame], 0);
	RecordCommandBuffer(m_vCommandBuffers[m_iCurrentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_vImageAvailableSemaphores[m_iCurrentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_vCommandBuffers[m_iCurrentFrame];
	VkSemaphore signalSemaphores[] = { m_vImageAvailableSemaphores[m_iCurrentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_vInFlightFences[m_iCurrentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(m_presentationQueue, &presentInfo);

	// Next frame
	m_iCurrentFrame = (m_iCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void CEngine::Cleanup(void)
{
	vkDestroySurfaceKHR(m_vInstance, m_surface, nullptr);
	CleanupSwapChain();
	//
	vkDestroyBuffer(m_logicelDevice, m_indexBuffer, nullptr);
	vkFreeMemory(m_logicelDevice, m_indexBufferMemory, nullptr);
	vkDestroyBuffer(m_logicelDevice, m_vertexBuffer, nullptr);
	vkFreeMemory(m_logicelDevice, m_vertexBufferMemory, nullptr);
	//
	vkDestroyPipeline(m_logicelDevice, m_graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_logicelDevice, m_pipelineLayout, nullptr);
	vkDestroyRenderPass(m_logicelDevice, m_renderPass, nullptr);
	vkDestroyCommandPool(m_logicelDevice, m_commandPool, nullptr);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(m_logicelDevice, m_vImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(m_logicelDevice, m_vRenderFinishedSemaphores[i], nullptr);
		vkDestroyFence(m_logicelDevice, m_vInFlightFences[i], nullptr);
	}
	vkDestroyDevice(m_logicelDevice, nullptr);
	vkDestroyInstance(m_vInstance, nullptr);
	pWindow->Finalize();
}

void CEngine::CleanupFrameBuffer(void)
{
	for (auto framebuffer : m_vSwapChainFramebuffers) {
		vkDestroyFramebuffer(m_logicelDevice, framebuffer, nullptr);
	}
}

void CEngine::CleanupSwapChain(void)
{
	CleanupFrameBuffer();
	DestroyImageViews();

	vkDestroySwapchainKHR(m_logicelDevice, m_swapChain, nullptr);
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

	// Dynamic state
	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	// This will cause the configuration of these values to be ignored and we need to specify the data at drawing time.
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();


	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptionPos = Vertex::GetAttributeDescriptionPos();
	auto attributeDescriptionCol = Vertex::GetAttributeDescriptionCol();
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = { {attributeDescriptionPos, attributeDescriptionCol} };

	// Vertex Input(Data hard coded inside of the shader atm)
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

	// Input assembly
	// The VkPipelineInputAssemblyStateCreateInfo struct describes two things: what kind of geometry will be drawn from the vertices and if primitive restart should be enabled.
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapChainExtent.width;
	viewport.height = (float)m_swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	// Scissor(scissor rectangles define in which regions pixels will actually be stored. Any pixels outside the scissor rectangles will be discarded by the rasterizer.)
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	// Rasterizer (vertex data to fragments)
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE; // VK_TRUE, then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them.
	rasterizer.rasterizerDiscardEnable = VK_FALSE; // VK_TRUE, then geometry never passes through the rasterizer stage. This basically disables any output to the framebuffer.
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	// Multisampling (Disabled for now)
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	// Depth and stencil testing (Disabled for now)

	// Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	// Pipeline Layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(m_logicelDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	// Creating Pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(m_logicelDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	// Destroy wrapper after Pipeline is created since they are not needed anymore
	vkDestroyShaderModule(m_logicelDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(m_logicelDevice, vertShaderModule, nullptr);
}

void CEngine::CreateRenderPass(void)
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Subpasses and attachment references
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(m_logicelDevice, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create render pass!");
	}
}

void CEngine::CreateFrameBuffers(void)
{
	m_vSwapChainFramebuffers.resize(m_vSwapChainImageViews.size());

	for (size_t i = 0; i < m_vSwapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			m_vSwapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = m_swapChainExtent.width;
		framebufferInfo.height = m_swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(m_logicelDevice, &framebufferInfo, nullptr, &m_vSwapChainFramebuffers[i]) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void CEngine::CreateCommandPool(void)
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_physicalDevice);

	/* There are two possible flags for command pools:
	* VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
	* VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
	*/

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(m_logicelDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void CEngine::CreateVertexBuffer(void)
{
	pCube = std::make_shared<CCube>();
	pCube->Initialize();
	m_vSceneObjects.push_back(pCube);

	auto vertices = pCube->GetMeshVertexData();
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	// Added staging buffer (vertex data is now being loaded from high performance memory)
	// One staging buffer in CPU accessible memory to upload the data from the vertex array to, and the final vertex buffer in device local memory(GPU).
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	// Map memory and copy vertices.data() to it(other possibility would be explicit flushing)
	vkMapMemory(m_logicelDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_logicelDevice, stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

	CopyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

	vkDestroyBuffer(m_logicelDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_logicelDevice, stagingBufferMemory, nullptr);
}

void CEngine::CreateIndexBuffer(void)
{
	//Temp
	auto cube = m_vSceneObjects[0];
	auto indices = cube->GetMeshIndiceData();

	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_logicelDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_logicelDevice, stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

	CopyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

	vkDestroyBuffer(m_logicelDevice, stagingBuffer, nullptr);
	vkFreeMemory(m_logicelDevice, stagingBufferMemory, nullptr);
}

void CEngine::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_logicelDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_logicelDevice, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(m_logicelDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(m_logicelDevice, buffer, bufferMemory, 0);
}

void CEngine::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_logicelDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);
	/*
	* We could use a fence and wait with vkWaitForFences, or simply wait for the transfer queue to become idle with vkQueueWaitIdle. 
	* A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete, instead of executing one at a time. 
	* That may give the driver more opportunities to optimize.
	*/

	vkFreeCommandBuffers(m_logicelDevice, m_commandPool, 1, &commandBuffer);
}

void CEngine::CreateCommandBuffers(void)
{
	/* The level parameter specifies if the allocated command buffers are primary or secondary command buffers.
	* VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers.
	* VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.
	*/

	m_vCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_vCommandBuffers.size();;

	if (vkAllocateCommandBuffers(m_logicelDevice, &allocInfo, m_vCommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void CEngine::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	/* The flags parameter specifies how we're going to use the command buffer. The following values are available:
	* VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.
	* VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: This is a secondary command buffer that will be entirely within a single render pass.
	* VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: The command buffer can be resubmitted while it is also already pending execution.
	*/

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Only relevant for secondary command buffers

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	// Starting a render pass
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_renderPass;
	renderPassInfo.framebuffer = m_vSwapChainFramebuffers[imageIndex];
	// size of the render area
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_swapChainExtent;
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	/* The final parameter controls how the drawing commands within the render pass will be provided. It can have one of two values:
	* VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
	* VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
	*/

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Basic drawing commands

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

	VkBuffer vertexBuffers[] = { m_vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(m_swapChainExtent.width);
	viewport.height = static_cast<float>(m_swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	auto vertices = m_vSceneObjects[0]->GetMeshVertexData();
	auto indices = m_vSceneObjects[0]->GetMeshIndiceData();

	/*
	* vertexCount: Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
	* instanceCount: Used for instanced rendering, use 1 if you're not doing that.
	* firstVertex: Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
	* firstInstance: Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.
	*/
	//vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void CEngine::CreateSyncObjects(void)
{
	m_vImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_vRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_vInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // We set it signaled to skip the first vkWaitForFences which otherwise would block us indefinitely

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(m_logicelDevice, &semaphoreInfo, nullptr, &m_vImageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(m_logicelDevice, &semaphoreInfo, nullptr, &m_vRenderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(m_logicelDevice, &fenceInfo, nullptr, &m_vInFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create semaphores!");
		}
	}

	
}

void CEngine::RecreateSwapChain(void)
{
	pWindow->CheckIfWindowMinimized();
	vkDeviceWaitIdle(m_logicelDevice);

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateFrameBuffers();
}

uint32_t CEngine::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
