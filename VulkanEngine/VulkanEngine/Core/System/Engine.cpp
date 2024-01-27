#include "Engine.h"
#include "../../WindowGLFW/Window.h"
#include "../../Utility/Utility.h"
#include <stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // GLM uses the OpenGL depth range of -1.0 to 1.0 by default, but Vulkan uses 0.0 to 1.0
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "RenderSystems/SimpleRenderSystem.h"
#include "RenderSystems/PointLightSystem.h"
#include "Scenes/DefaultScene.h"
#include "Scenes/LoadedModelScene.h"

constexpr uint32_t WIDTH = 1200;
constexpr uint32_t HEIGHT = 1000;
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
	InitializePhysx();
	MainLoop();
}

void CEngine::InitializeVulkan(void)
{
	CreateInput();
	m_pDevice = std::make_shared<CDevice>(m_pWindow);
	CreateScenes();
	EngineSetup();
}

void CEngine::EngineSetup()
{
	if (m_pRenderer == nullptr)
		m_pRenderer = std::make_shared<CRenderer>(m_pDevice, m_pWindow, m_pCurrScene);
	else
		m_pRenderer->RecreateSwapChain();

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
		.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
		.AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();

	m_vGlobalDescriptorSets.resize(CSwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < m_vGlobalDescriptorSets.size(); ++i)
	{
		auto bufferInfo = m_uboBuffers[i]->DescriptorInfo(sizeof(UniformBufferObject));
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

void CEngine::InitializePhysx(void)
{
	// Create foundation
	//m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	//m_ToleranceScale.length = 100;        // typical length of an object
	//m_ToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice

	//// Create physics
	//m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, m_ToleranceScale);

}

void CEngine::CreateInput(void)
{
	m_playerController = std::make_shared<CPlayerController>();

	m_playerController->SetSceneSwitch(([this]()
	{
		m_bSwitchScenes = true;
	}));
	m_playerController2 = std::make_shared<CPlayerController>();

	m_playerController2->SetSceneSwitch(([this]()
	{
		m_bSwitchScenes = true;
	}));
}

void CEngine::CreateScenes(void)
{
	const auto scene = std::make_shared<CDefaultScene>(m_playerController,
	                                                   m_pWindow,
	                                                   m_pDevice,
	                                                   WIDTH,
	                                                   HEIGHT);
	scene->Initialize();
	m_vScenes.push_back(scene);
	m_pCurrScene = m_vScenes[m_iCurrSceneNum];

	const auto scene2 = std::make_shared<CLoadedModelScene>(m_playerController2,
													   m_pWindow,
													   m_pDevice,
													   WIDTH,
													   HEIGHT);
	m_vScenes.push_back(scene2);
}

void CEngine::MainLoop(void)
{
	CSimpleRenderSystem simpleRenderSystem{m_pDevice, m_pRenderer->GetSwapChainRenderPass(), m_pDescriptorSetLayout->GetDescriptorSetLayout()};
	CPointLightSystem pointLightSystem{m_pDevice, m_pRenderer->GetSwapChainRenderPass(), m_pDescriptorSetLayout->GetDescriptorSetLayout()};
	
	while (!m_pWindow->GetWindowShouldClose())
	{
		m_pWindow->Update();
		m_dCurrentFrame = glfwGetTime();
		m_dDeltaTime = m_dCurrentFrame - m_dLastFrame;
		m_dLastFrame = m_dCurrentFrame;
		if (const auto commandBuffer = m_pRenderer->BeginFrame())
		{
			const auto frameIndex = m_pRenderer->GetFrameIndex();
			DrawInformation drawInfo{commandBuffer, simpleRenderSystem.GetLayout(), m_vGlobalDescriptorSets[frameIndex]};

			// Update uniform buffers
			UniformBufferObject ubo = m_pCurrScene->CreateUniformBuffer();
			// Switched from IndexedBuffer since each uniform data is stored in a different frame
			m_uboBuffers[frameIndex]->WriteToBuffer(&ubo);
			m_uboBuffers[frameIndex]->Flush();
			
			m_pRenderer->BeginSwapChainRenderPass(drawInfo);
			m_pCurrScene->Update(m_dDeltaTime);
			simpleRenderSystem.RenderGameObjects(drawInfo, m_pCurrScene);
			pointLightSystem.Render(drawInfo);
			m_pRenderer->EndSwapChainRenderPass(drawInfo);
			m_pRenderer->EndFrame();
			
			if (m_bSwitchScenes)
			{
				m_iCurrSceneNum++;
				m_iCurrSceneNum %= m_vScenes.size();
				m_pCurrScene->Finalize();
				m_pCurrScene = m_vScenes[m_iCurrSceneNum];
				m_pCurrScene->Initialize();
				EngineSetup();
				m_bSwitchScenes = false;
			}
			//std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}

	vkDeviceWaitIdle(m_pDevice->GetLogicalDevice());
}

void CEngine::Cleanup(void)
{
	m_pCurrScene->Finalize();
	//m_pPhysics->release();
	//m_pFoundation->release();
	m_pWindow->Finalize();
}

