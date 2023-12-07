#ifndef ENGINE_H
#define ENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "../../Input/PlayerController.h"
#include "Scene.h"
#include "Device.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "Scenes/CDefaultScene.h"


class CEngine
{
public:

	CEngine() = default;
	CEngine(const CEngine&) = default;
	CEngine(CEngine&&) = default;
	CEngine& operator= (const CEngine&) = default;
	CEngine& operator= (CEngine&&) = default;
	~CEngine() = default;

	void Run(void);

private:
	void InitializeVulkan(void);
	void InitializeWindow(void);
	void CreateInput(void);
	void CreateScenes(void);
	void MainLoop(void);
	void DrawFrame(void);
	void Cleanup(void);
	
	void CreateGLFWSurface(void);

	std::shared_ptr<CDevice> m_pDevice{nullptr};

	void CreateVulkanInstance(void);
	bool CheckValidationLayerSupport(const std::vector<const char*> a_enabled_layers);

	
	std::unique_ptr<CSwapChain> m_pSwapChain{nullptr};
	void RecreateSwapChain(void);
	
	std::shared_ptr<CPipeline> m_pPipeline{nullptr};
	VkPipelineLayout m_pipelineLayout{nullptr};
	void CreatePipeline(void);

	void CreateCommandBuffers(void);
	void FreeCommandBuffers(void);
	std::vector<VkCommandBuffer> m_vCommandBuffers{};

	bool HasStencilComponent(VkFormat a_format);

	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	bool m_bEnableValidationLayers{true};
	std::shared_ptr<VkInstance> m_vInstance{};
	
	VkSurfaceKHR m_surface{};

	VkDescriptorPool m_descriptorPool{};
	std::vector<VkDescriptorSet> m_vDescriptorSets{};

	// Scenes
	std::shared_ptr<CDefaultScene> m_firstScene{};

	// Input
	std::shared_ptr<CPlayerController> m_playerController{};
};

#endif