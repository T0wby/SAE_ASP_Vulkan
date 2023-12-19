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

	m_uboBuffers.resize(CSwapChain::MAX_FRAMES_IN_FLIGHT);
	
	for (auto& m_uboBuffer : m_uboBuffers)
	{
		m_uboBuffer = std::make_unique<CBuffer>(
			m_pDevice,
			sizeof(UniformBufferObject),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		m_uboBuffer->Map();
	}
	
	m_pGlobalPool = CDescriptorPool::Builder(m_pDevice)
		.SetMaxSets(CSwapChain::MAX_FRAMES_IN_FLIGHT)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, CSwapChain::MAX_FRAMES_IN_FLIGHT)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, CSwapChain::MAX_FRAMES_IN_FLIGHT)
		.Build();

	m_pDescriptorSetLayout = CDescriptorSetLayout::Builder(m_pDevice)
		.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
		.AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();

	m_vGlobalDescriptorSets.resize(CSwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < m_vGlobalDescriptorSets.size(); ++i)
	{
		auto bufferInfo = m_uboBuffers[i]->DescriptorInfo();
		auto imageInfo = m_pRenderer->GetDescriptorImageInfo();
		CDescriptorWriter(*m_pDescriptorSetLayout, *m_pGlobalPool)
			.WriteBuffer(0, &bufferInfo)
			.WriteImage(1, &imageInfo)
			.Build(m_vGlobalDescriptorSets[i]);
	}
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
	CSimpleRenderSystem simpleRenderSystem{m_pDevice, m_pRenderer->GetSwapChainRenderPass(), m_pDescriptorSetLayout->GetDescriptorSetLayout()};
	
	while (!m_pWindow->GetWindowShouldClose()) 
	{
		m_pWindow->Update();
		m_dCurrentFrame = glfwGetTime();
		m_dDeltaTime = m_dCurrentFrame - m_dLastFrame;
		m_dLastFrame = m_dCurrentFrame;
		if (const auto commandBuffer = m_pRenderer->BeginFrame())
		{
			DrawInformation drawInfo{commandBuffer, simpleRenderSystem.GetLayout(), m_vGlobalDescriptorSets[m_pRenderer->GetFrameIndex()]};
			m_pRenderer->BeginSwapChainRenderPass(drawInfo);
			m_firstScene->Update(m_dDeltaTime);
			simpleRenderSystem.RenderGameObjects(drawInfo, m_firstScene);
			m_pRenderer->EndSwapChainRenderPass(drawInfo);

			// Update uniform buffers
			UniformBufferObject ubo = m_firstScene->CreateUniformBuffer();
			m_uboBuffers[m_pRenderer->GetFrameIndex()]->WriteToIndex(&ubo, m_pRenderer->GetFrameIndex());
			
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

