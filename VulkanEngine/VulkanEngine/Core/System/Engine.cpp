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


CEngine::~CEngine()
{
	Cleanup();
}

void CEngine::Run(void)
{
	InitializeWindow();
	InitializeVulkan();
	MainLoop();
}

void CEngine::InitializeVulkan(void)
{
	CreateInput();
	m_pDevice = std::make_shared<CDevice>(m_pWindow);
	CreateScenes();
	m_pRenderer = std::make_shared<CRenderer>(m_pDevice, m_pWindow, m_firstScene);
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
	CSimpleRenderSystem simpleRenderSystem{m_pDevice, m_pRenderer->GetSwapChainRenderPass(), m_pRenderer->GetDescriptorSetLayout()};

	
	while (!m_pWindow->GetWindowShouldClose()) 
	{
		m_pWindow->Update();
		m_dCurrentFrame = glfwGetTime();
		m_dDeltaTime = m_dCurrentFrame - m_dLastFrame;
		m_dLastFrame = m_dCurrentFrame;
		if (const auto commandBuffer = m_pRenderer->BeginFrame())
		{
			DrawInformation drawInfo{commandBuffer, simpleRenderSystem.GetLayout()};
			m_pRenderer->BeginSwapChainRenderPass(drawInfo);
			m_firstScene->Update(m_dDeltaTime);
			simpleRenderSystem.RenderGameObjects(drawInfo, m_firstScene);
			m_pRenderer->EndSwapChainRenderPass(drawInfo);
			m_pRenderer->EndFrame();
		}
	}

	vkDeviceWaitIdle(m_pDevice->GetLogicalDevice());
}

void CEngine::Cleanup(void)
{
	m_firstScene->Finalize();
	m_pWindow->Finalize();
}

