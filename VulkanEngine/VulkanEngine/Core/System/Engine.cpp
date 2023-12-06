#include "Engine.h"
#include "../../WindowGLFW/Window.h"
#include "../../Utility/Utility.h"
#include "SwapChain.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // GLM uses the OpenGL depth range of -1.0 to 1.0 by default, but Vulkan uses 0.0 to 1.0
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// System
std::shared_ptr<CWindow> pWindow = nullptr;

//Temp
//std::shared_ptr<CCube> pCube = nullptr;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;
const std::string VERT_SHADER = "Shader/vert.spv";
const std::string FRAG_SHADER = "Shader/frag.spv";
const std::string NAME = "SAE_Tobi_Engine";
const std::string APPLICATION_NAME = "SAE_ASP_Engine";
constexpr int MAX_FRAMES_IN_FLIGHT = 2;


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
	CreateInput();
	CreateGLFWSurface();

	m_pDevice = std::make_shared<CDevice>(m_vInstance, m_surface);
	m_pDevice->Initialize();
	// PickPhysicalDevice();
	// CreateLogicalDevice();
	//CreateCommandPool();

	m_pSwapChain = std::make_shared<CSwapChain>(*m_pDevice, pWindow, m_surface);
	//CreateSwapChain();
	//CreateImageViews();
	//CreateRenderPass();
	//CreateDepthResources();
	//CreateFrameBuffers();
	//CreateSyncObjects();

	CreateScenes();
	
	CreatePipeline();
	//CreateDescriptorSetLayout();
	//CreateGraphicsPipeline();
	
	m_pSwapChain->CreateTextures();
	//CreateTextureImage();
	//CreateTextureImageView();
	//CreateTextureSampler();
	//CreateVertexBuffer(); in Mesh now
	//CreateIndexBuffer(); in Mesh now
	CreateUniformBuffers();
	m_pSwapChain->CreateDescriptorPool();
	m_pSwapChain->CreateDescriptorSets(m_vUniformBuffers);
	CreateCommandBuffers();
}


void CEngine::CreateVulkanInstance(void)
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

	m_vInstance = std::make_shared<VkInstance>();

	const VkResult result = vkCreateInstance(&create_info, nullptr, m_vInstance.get());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create VK instance!");
	}
}

void CEngine::CreatePipeline()
{
	auto defaultPipelineConfigInfo = CPipeline::DefaultPipelineConfigInfo(m_pSwapChain->GetWidth(), m_pSwapChain->GetHeight());
	defaultPipelineConfigInfo.renderPass = m_pSwapChain->GetRenderPass();
	auto layout = m_pSwapChain->GetDescriptorSetLayout();
	m_pPipeline = std::make_shared<CPipeline>(*m_pDevice, defaultPipelineConfigInfo, VERT_SHADER, FRAG_SHADER, layout);
	m_pipelineLayout = defaultPipelineConfigInfo.pipelineLayout;
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

void CEngine::CreateInput(void)
{
	m_playerController = std::make_shared<CPlayerController>();
}

void CEngine::CreateScenes(void)
{
	m_firstScene = std::make_shared<CDefaultScene>(m_playerController,
		pWindow,
		m_pDevice,
		m_pSwapChain->GetSwapChainExtent().width,
		m_pSwapChain->GetSwapChainExtent().height);
	m_firstScene->Initialize();
}

void CEngine::MainLoop(void)
{
	while (!pWindow->GetWindowShouldClose()) 
	{
		pWindow->Update();
		m_firstScene->Update();
		DrawFrame();
	}

	vkDeviceWaitIdle(m_pDevice->GetLogicalDevice());
}

void CEngine::DrawFrame(void)
{
	//vkWaitForFences(m_pDevice->GetLogicalDevice(), 1, &m_vInFlightFences[m_iCurrentFrame], VK_TRUE, UINT64_MAX);
	

	uint32_t imageIndex;
	const VkResult result = m_pSwapChain->AquireNextImage(imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || pWindow->IsFrameBufferResized())
	{
		pWindow->SetIsFrameBufferResized(false);
		m_pSwapChain->RecreateSwapChain();
		if (m_firstScene != nullptr)
			m_firstScene->UpdateSizeValues(m_pSwapChain->GetWidth(), m_pSwapChain->GetHeight());
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	m_pSwapChain->SubmitCommandBuffers(m_vCommandBuffers[m_pSwapChain->GetCurrentFrame()], imageIndex, m_firstScene, m_pPipeline, m_pipelineLayout);
}

void CEngine::Cleanup(void)
{
	m_pSwapChain->Finalize();
	//CleanupSwapChain();
	//
	// Finalize for Scene GO?
	m_firstScene->Finalize();
	//vkDestroyBuffer(m_pDevice->GetLogicalDevice(), m_indexBuffer, nullptr);
	//vkFreeMemory(m_pDevice->GetLogicalDevice(), m_indexBufferMemory, nullptr);
	//vkDestroyBuffer(m_pDevice->GetLogicalDevice(), m_vertexBuffer, nullptr);
	//vkFreeMemory(m_pDevice->GetLogicalDevice(), m_vertexBufferMemory, nullptr);
	//
	vkDestroySampler(m_pDevice->GetLogicalDevice(), m_textureSampler, nullptr);
	vkDestroyImageView(m_pDevice->GetLogicalDevice(), m_textureImageView, nullptr);
	vkDestroyImage(m_pDevice->GetLogicalDevice(), m_textureImage, nullptr);
	vkFreeMemory(m_pDevice->GetLogicalDevice(), m_textureImageMemory, nullptr);
	CleanupUniformBuffers();
	vkDestroyDescriptorPool(m_pDevice->GetLogicalDevice(), m_descriptorPool, nullptr);
	//vkDestroyDescriptorSetLayout(m_pDevice->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
	//vkDestroyPipeline(m_pDevice->GetLogicalDevice(), m_graphicsPipeline, nullptr);
	//vkDestroyPipelineLayout(m_pDevice->GetLogicalDevice(), m_pipelineLayout, nullptr);
	//vkDestroyRenderPass(m_pDevice->GetLogicalDevice(), m_renderPass, nullptr);
	//vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
	m_pPipeline->Finalize();
	//for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	//{
	//	vkDestroySemaphore(m_pDevice->GetLogicalDevice(), m_vImageAvailableSemaphores[i], nullptr);
	//	vkDestroySemaphore(m_pDevice->GetLogicalDevice(), m_vRenderFinishedSemaphores[i], nullptr);
	//	vkDestroyFence(m_pDevice->GetLogicalDevice(), m_vInFlightFences[i], nullptr);
	//}
	//vkDestroyDevice(m_device->GetLogicalDevice(), nullptr);
	vkDestroySurfaceKHR(*m_vInstance, m_surface, nullptr);
	m_pDevice->Finalize();
	vkDestroyInstance(*m_vInstance, nullptr);
	pWindow->Finalize();
}


void CEngine::CleanupUniformBuffers(void)
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vkDestroyBuffer(m_pDevice->GetLogicalDevice(), m_vUniformBuffers[i], nullptr);
		vkFreeMemory(m_pDevice->GetLogicalDevice(), m_vUniformBuffersMemory[i], nullptr);
	}
}


/**
// void CEngine::CreateLogicalDevice(void)
// {
// 	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);
// 	VkPhysicalDeviceFeatures deviceFeatures{};
// 	deviceFeatures.samplerAnisotropy = VK_TRUE;
//
//
// 	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
// 	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
//
// 	float priority = 1.0f;
//
// 	for (uint32_t queueFamily : uniqueQueueFamilies)
// 	{
// 		// This structure describes the number of queues we want for a single queue family
// 		VkDeviceQueueCreateInfo queueCreateInfo{};
// 		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
// 		queueCreateInfo.queueFamilyIndex = queueFamily;
// 		queueCreateInfo.queueCount = 1;
// 		queueCreateInfo.pQueuePriorities = &priority; 
// 		queueCreateInfos.push_back(queueCreateInfo);
// 	}
//
// 	// Logical Device
// 	VkDeviceCreateInfo deviceCreateInfo = {};
// 	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
// 	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
// 	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
// 	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_EnabledExtensions.size());
// 	deviceCreateInfo.ppEnabledExtensionNames = m_EnabledExtensions.data();
// 	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
// 	// enabledLayerCount and ppEnabledLayerNames fields of VkDeviceCreateInfo are ignored by up-to-date implementations
// 	if (m_bEnableValidationLayers)
// 	{
// 		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_EnabledLayers.size());
// 		deviceCreateInfo.ppEnabledLayerNames = m_EnabledLayers.data();
// 	}
// 	else
// 	{
// 		deviceCreateInfo.enabledLayerCount = 0;
// 	}
//
//
// 	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicelDevice) != VK_SUCCESS) 
// 	{
// 		throw std::runtime_error("failed to create logical device!");
// 	}
//
// 	// Get handle to interface with the queue later
// 	vkGetDeviceQueue(m_logicelDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
// 	vkGetDeviceQueue(m_logicelDevice, indices.presentFamily.value(), 0, &m_presentationQueue);
// }
**/

void CEngine::CreateGLFWSurface(void)
{
	pWindow->CreateWindowSurface(*m_vInstance, m_surface);
}


/*
void CEngine::CreateVertexBuffer(void)
{
	VkDeviceSize bufferSize = sizeof(Vertex) * m_firstScene->GetSceneVertexCount();

	// Added staging buffer (vertex data is now being loaded from high performance memory)
	// One staging buffer in CPU accessible memory to upload the data from the vertex array to, and the final vertex buffer in device local memory(GPU).
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	// Map memory and copy vertices.data() to it(other possibility would be explicit flushing)
	vkMapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_firstScene->GetSceneFirstVertex().data(), (size_t)bufferSize);
	vkUnmapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

	CopyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

	vkDestroyBuffer(m_pDevice->GetLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void CEngine::CreateIndexBuffer(void)
{
	VkDeviceSize bufferSize = sizeof(uint16_t) * m_firstScene->GetSceneIndicesCount();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_firstScene->GetSceneFirstIndice().data(), (size_t)bufferSize);
	vkUnmapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory);

	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

	CopyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

	vkDestroyBuffer(m_pDevice->GetLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void CEngine::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_pDevice->GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetLogicalDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(m_pDevice->GetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(m_pDevice->GetLogicalDevice(), buffer, bufferMemory, 0);
}

void CEngine::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}
*/

void CEngine::CreateCommandBuffers(void)
{
	/* The level parameter specifies if the allocated command buffers are primary or secondary command buffers.
	* VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers.
	* VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.
	*/

	m_vCommandBuffers.resize(m_pSwapChain->GetImageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_pDevice->GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_vCommandBuffers.size());

	if (vkAllocateCommandBuffers(m_pDevice->GetLogicalDevice(), &allocInfo, m_vCommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void CEngine::CreateUniformBuffers(void)
{
	const VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_vUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	m_vUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	m_vUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		// staging buffer not used due to uniform buffers being changed every frame
		m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vUniformBuffers[i], m_vUniformBuffersMemory[i]);

		// persistent mapping
		vkMapMemory(m_pDevice->GetLogicalDevice(), m_vUniformBuffersMemory[i], 0, bufferSize, 0, &m_vUniformBuffersMapped[i]);
	}
}


void CEngine::CreateTextureSampler(void)
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;

	// Might be a good idea to save those in a variable
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(m_pDevice->GetPhysicalDevice(), &properties);

	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(m_pDevice->GetLogicalDevice(), &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

bool CEngine::HasStencilComponent(VkFormat a_format)
{
	return a_format == VK_FORMAT_D32_SFLOAT_S8_UINT || a_format == VK_FORMAT_D24_UNORM_S8_UINT;
}
