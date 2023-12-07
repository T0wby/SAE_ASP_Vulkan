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


void CEngine::Run(void)
{
	InitializeWindow();
	InitializeVulkan();
	MainLoop();
	Cleanup();
}
/* TODO: Check order of execution
*createInstance(); CreateVulkanInstance
*createSurface(); CreateGLFWSurface
*pickPhysicalDevice(); m_pDevice->Initialize()
*createLogicalDevice(); m_pDevice->Initialize()
*createSwapChain(); std::make_shared<CSwapChain>
*createImageViews(); std::make_shared<CSwapChain>
*createRenderPass(); std::make_shared<CSwapChain>
*createDescriptorSetLayout(); CreatePipeline()
*createGraphicsPipeline(); CreatePipeline()
*createCommandPool(); m_pDevice->Initialize()
*createDepthResources(); std::make_shared<CSwapChain>
*createFramebuffers(); std::make_shared<CSwapChain>
*createTextureImage(); m_pSwapChain->CreateTextures()
*createTextureImageView(); m_pSwapChain->CreateTextures()
*createTextureSampler(); m_pSwapChain->CreateTextures()
*createVertexBuffer(); m_firstScene->Initialize() / CreateScenes()
*createIndexBuffer(); m_firstScene->Initialize() / CreateScenes()
*createUniformBuffers(); CreateUniformBuffers()
*createDescriptorPool(); m_pSwapChain->CreateDescriptorPool()
*createDescriptorSets(); m_pSwapChain->CreateDescriptorSets(m_vUniformBuffers)
*createCommandBuffers(); CreateCommandBuffers
*createSyncObjects(); std::make_shared<CSwapChain>
 */
void CEngine::InitializeVulkan(void)
{
	CreateVulkanInstance();
	CreateInput();
	CreateGLFWSurface();
	m_pDevice = std::make_shared<CDevice>(m_vInstance, m_surface);
	m_pDevice->Initialize();
	m_pSwapChain = std::make_unique<CSwapChain>(*m_pDevice, pWindow, m_surface);
	CreateScenes();
	CreatePipeline();
	m_pSwapChain->CreateTextures();
	m_pSwapChain->CreateUniformBuffers();
	m_pSwapChain->CreateDescriptorPool();
	m_pSwapChain->CreateDescriptorSets();
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
	PipelineConfigInfo defaultPipelineConfigInfo{};
	CPipeline::DefaultPipelineConfigInfo(defaultPipelineConfigInfo);
	defaultPipelineConfigInfo.renderPass = m_pSwapChain->GetRenderPass();
	m_pSwapChain->CreateDescriptorSetLayout();
	auto layout = m_pSwapChain->GetDescriptorSetLayout();
	m_pPipeline = std::make_shared<CPipeline>(*m_pDevice, &defaultPipelineConfigInfo, VERT_SHADER, FRAG_SHADER, layout);
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

void CEngine::RecreateSwapChain()
{
	//auto extent = pWindow->GetExtent();
	pWindow->CheckIfWindowMinimized();
	vkDeviceWaitIdle(m_pDevice->GetLogicalDevice());
	if (m_pSwapChain == nullptr)
	{
		// might not be needed
		m_pSwapChain->Finalize();
		m_pSwapChain = std::make_unique<CSwapChain>(*m_pDevice, pWindow, m_surface);
		m_pSwapChain->CreateTextures();
		m_pSwapChain->CreateUniformBuffers();
		m_pSwapChain->CreateDescriptorPool();
		m_pSwapChain->CreateDescriptorSetLayout();
		m_pSwapChain->CreateDescriptorSets();
	}
	else
	{
		m_pSwapChain = std::make_unique<CSwapChain>(*m_pDevice, pWindow, m_surface, std::move(m_pSwapChain));
		m_pSwapChain->CreateTextures();
		m_pSwapChain->CreateUniformBuffers();
		m_pSwapChain->CreateDescriptorPool();
		m_pSwapChain->CreateDescriptorSetLayout();
		m_pSwapChain->CreateDescriptorSets();
		if (m_pSwapChain->GetImageCount() != m_vCommandBuffers.size())
		{
			FreeCommandBuffers();
			CreateCommandBuffers();
		}
	}
	
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
	VkResult result = m_pSwapChain->AquireNextImage(imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || pWindow->IsFrameBufferResized())
	{
		pWindow->SetIsFrameBufferResized(false);
		RecreateSwapChain();
		if (m_firstScene != nullptr)
			m_firstScene->UpdateSizeValues(static_cast<uint32_t>(m_pSwapChain->GetWidth()), static_cast<uint32_t>(m_pSwapChain->GetHeight()));
		return;
	}
	result = m_pSwapChain->SubmitCommandBuffers(m_vCommandBuffers[m_pSwapChain->GetCurrentFrame()], imageIndex, m_firstScene, m_pPipeline, m_pipelineLayout);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||  pWindow->IsFrameBufferResized())
	{
		pWindow->SetIsFrameBufferResized(false);
		RecreateSwapChain();
		if (m_firstScene != nullptr)
			m_firstScene->UpdateSizeValues(static_cast<uint32_t>(m_pSwapChain->GetWidth()), static_cast<uint32_t>(m_pSwapChain->GetHeight()));
		return;
	}
	if (result != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}
}

void CEngine::Cleanup(void)
{
	m_pSwapChain->Finalize();
	m_firstScene->Finalize();
	m_pPipeline->Finalize();
	vkDestroySurfaceKHR(*m_vInstance, m_surface, nullptr);
	m_pDevice->Finalize();
	vkDestroyInstance(*m_vInstance, nullptr);
	pWindow->Finalize();
}


void CEngine::CreateGLFWSurface(void)
{
	pWindow->CreateWindowSurface(*m_vInstance, m_surface);
}

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

void CEngine::FreeCommandBuffers()
{
	vkFreeCommandBuffers(m_pDevice->GetLogicalDevice(), m_pDevice->GetCommandPool(), m_vCommandBuffers.size(), m_vCommandBuffers.data());
	m_vCommandBuffers.clear();
}

bool CEngine::HasStencilComponent(VkFormat a_format)
{
	return a_format == VK_FORMAT_D32_SFLOAT_S8_UINT || a_format == VK_FORMAT_D24_UNORM_S8_UINT;
}
