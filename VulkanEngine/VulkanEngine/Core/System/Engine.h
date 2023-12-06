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
	
	void CleanupUniformBuffers(void);
	void CreateGLFWSurface(void);

	// Replaced with Device Class
	//void PickPhysicalDevice(void);
	//void CreateLogicalDevice(void);
	//bool IsDeviceSuitable(VkPhysicalDevice a_device);
	//bool CheckDeviceExtensionSupport(VkPhysicalDevice a_device);
	//QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_device);
	//SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice a_device);
	//void CreateCommandPool(void);
	/******************In Device Class******************/
	std::shared_ptr<CDevice> m_pDevice{nullptr};
	
	//VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	//VkDevice m_logicalDevice{};
	//VkQueue m_graphicsQueue{};
	//VkQueue m_presentationQueue{};
	//VkCommandPool m_commandPool{};
	/************************************/

	void CreateVulkanInstance(void);
	bool CheckValidationLayerSupport(const std::vector<const char*> a_enabled_layers);

	
	std::shared_ptr<CSwapChain> m_pSwapChain{nullptr};
	
	std::shared_ptr<CPipeline> m_pPipeline{nullptr};
	VkPipelineLayout m_pipelineLayout{nullptr};
	void CreatePipeline(void);

	void CreateCommandBuffers(void);
	std::vector<VkCommandBuffer> m_vCommandBuffers{};

	void CreateUniformBuffers(void);
	void CreateTextureSampler(void);
	bool HasStencilComponent(VkFormat a_format);

	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	bool m_bEnableValidationLayers{true};
	std::shared_ptr<VkInstance> m_vInstance{};
	
	VkSurfaceKHR m_surface{};

	std::vector<VkBuffer> m_vUniformBuffers{};
	std::vector<VkDeviceMemory> m_vUniformBuffersMemory{};
	std::vector<void*> m_vUniformBuffersMapped{};

	VkDescriptorPool m_descriptorPool{};
	std::vector<VkDescriptorSet> m_vDescriptorSets{};

	VkImage m_textureImage{};
	VkDeviceMemory m_textureImageMemory{};

	VkImageView m_textureImageView{};
	VkSampler m_textureSampler{};

	// Scenes
	std::shared_ptr<CDefaultScene> m_firstScene{};

	// Input
	std::shared_ptr<CPlayerController> m_playerController{};
};

#endif