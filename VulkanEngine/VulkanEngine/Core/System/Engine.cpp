#include "Engine.h"
#include "../../WindowGLFW/Window.h"
#include "../../Utility/Utility.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // GLM uses the OpenGL depth range of -1.0 to 1.0 by default, but Vulkan uses 0.0 to 1.0
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "RenderSystems/SimpleRenderSystem.h"

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

void CEngine::InitializeVulkan(void)
{
	CreateVulkanInstance();
	CreateInput();
	CreateGLFWSurface();
	m_pDevice = std::make_shared<CDevice>(m_vInstance, m_pWindow->GetSurface());
	m_pDevice->Initialize();
	CreateScenes();
	m_pRenderer = std::make_shared<CRenderer>(m_pDevice, m_pWindow, m_firstScene);
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

bool CEngine::CheckValidationLayerSupport(const std::vector<const char*>& a_enabled_layers)
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
	m_pWindow = std::make_shared<CWindow>(WIDTH, HEIGHT, NAME);
	m_pWindow->Initialize();
}

void CEngine::CreateInput(void)
{
	m_playerController = std::make_shared<CPlayerController>();
}

void CEngine::CreateScenes(void)
{
	m_firstScene = std::make_shared<CDefaultScene>(m_playerController,
		m_pWindow,
		m_pDevice,
		WIDTH,
		HEIGHT);
	m_firstScene->Initialize();
}

void CEngine::MainLoop(void)
{
	while (!m_pWindow->GetWindowShouldClose()) 
	{
		CSimpleRenderSystem simpleRenderSystem{m_pDevice, m_pRenderer->GetSwapChainRenderPass(), m_pRenderer->GetDescriptorSetLayout()};
		m_pWindow->Update();
		m_firstScene->Update();
		if (const auto commandBuffer = m_pRenderer->BeginFrame())
		{
			DrawInformation drawInfo{commandBuffer, simpleRenderSystem.GetLayout()};
			m_pRenderer->BeginSwapChainRenderPass(drawInfo);
			simpleRenderSystem.RenderGameObjects(drawInfo, m_firstScene);
			m_pRenderer->EndSwapChainRenderPass(drawInfo);
			m_pRenderer->EndFrame();
			simpleRenderSystem.Finalize();
		}
	}

	vkDeviceWaitIdle(m_pDevice->GetLogicalDevice());
}

void CEngine::Cleanup(void)
{
	m_firstScene->Finalize();
	m_pRenderer->Finalize();
	vkDestroySurfaceKHR(*m_vInstance, m_pWindow->GetSurface(), nullptr);
	m_pDevice->Finalize();
	vkDestroyInstance(*m_vInstance, nullptr);
	m_pWindow->Finalize();
}

void CEngine::CreateGLFWSurface(void)
{
	m_pWindow->CreateWindowSurface(*m_vInstance);
}

