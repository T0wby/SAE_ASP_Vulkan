#ifndef ENGINE_H
#define ENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "CoreSystemStructs.h"
#include "../../Input/PlayerController.h"
#include "Scene.h"
#include "Device.h"
#include "Pipeline.h"
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

	
	std::unique_ptr<CSwapChain> m_pSwapChain{nullptr};
	// Replaced with SwapChain Class
	void CreateSwapChain(void);
	void CreateImageViews(void);
	void DestroyImageViews(void);
	void CreateRenderPass(void);
	void CreateFrameBuffers(void);
	void CreateSyncObjects(void);
	void RecreateSwapChain(void);
	void CleanupSwapChain(void);
	void CleanupFrameBuffer(void);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentationModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateDepthResources(void);
	VkFormat FindDepthFormat();
	void CreateImage(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkImage& a_image, VkDeviceMemory& a_imageMemory);
	VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
	void TransitionImageLayout(VkImage a_image, VkFormat a_format, VkImageLayout a_oldLayout, VkImageLayout a_newLayout);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& a_candidates, VkImageTiling a_tiling, VkFormatFeatureFlags a_features);
	// SwapChain
	VkSwapchainKHR m_swapChain{};
	std::vector<VkImage> m_vSwapChainImages{};
	VkFormat m_swapChainImageFormat{};
	VkExtent2D m_swapChainExtent{};
	std::vector<VkImageView> m_vSwapChainImageViews{};
	std::vector<VkFramebuffer> m_vSwapChainFramebuffers{};
	std::vector<VkSemaphore> m_vImageAvailableSemaphores{};
	std::vector<VkSemaphore> m_vRenderFinishedSemaphores{};
	std::vector<VkFence> m_vInFlightFences{};
	uint32_t m_iCurrentFrame{ 0 };
	
	
	// Replaced with Pipeline Class
	//void CreateDescriptorSetLayout(void);
	//void CreateGraphicsPipeline(void);
	//VkShaderModule CreateShaderModule(const std::vector<char>& a_vBytecode);
	/******************In Pipeline Class******************/
	std::unique_ptr<CPipeline> m_pPipeline{nullptr};
	VkPipelineLayout m_pipelineLayout{nullptr};
	std::vector<VkCommandBuffer> m_vCommandBuffers{};

	void CreatePipeline(void);
	void CreateCommandBuffers(void);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	VkCommandBuffer BeginSingleTimeCommands(void);
	void EndSingleTimeCommands(VkCommandBuffer a_commandBuffer);

	//VkRenderPass m_renderPass{};
	//VkDescriptorSetLayout m_descriptorSetLayout{};
	//VkPipelineLayout m_pipelineLayout{};
	//VkPipeline m_graphicsPipeline{};
	/************************************/
	
	void CreateDescriptorPool(void);
	void CreateDescriptorSets(void);

	// Done in Mesh?
	void CreateVertexBuffer(void);
	void CreateIndexBuffer(void);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	
	
	void CreateUniformBuffers(void);
	void UpdateUniformBuffer(uint32_t a_currentImage);
	void CreateTextureImage(void);
	void CopyBufferToImage(VkBuffer a_buffer, VkImage a_image, uint32_t a_width, uint32_t a_height);
	void CreateTextureImageView(void);
	void CreateTextureSampler(void);
	bool HasStencilComponent(VkFormat a_format);

	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	bool m_bEnableValidationLayers{true};
	std::shared_ptr<VkInstance> m_vInstance{};
	
	VkSurfaceKHR m_surface{};
	


	// VertexBuffer
	VkBuffer m_vertexBuffer{};
	VkDeviceMemory m_vertexBufferMemory{};
	VkBuffer m_indexBuffer{};
	VkDeviceMemory m_indexBufferMemory{};

	std::vector<VkBuffer> m_vUniformBuffers{};
	std::vector<VkDeviceMemory> m_vUniformBuffersMemory{};
	std::vector<void*> m_vUniformBuffersMapped{};

	VkDescriptorPool m_descriptorPool{};
	std::vector<VkDescriptorSet> m_vDescriptorSets{};

	VkImage m_textureImage{};
	VkDeviceMemory m_textureImageMemory{};

	VkImageView m_textureImageView{};
	VkSampler m_textureSampler{};

	VkImage m_depthImage{};
	VkDeviceMemory m_depthImageMemory{};
	VkImageView m_depthImageView{};

	// Scenes
	std::shared_ptr<CDefaultScene> m_firstScene{};

	// Input
	std::shared_ptr<CPlayerController> m_playerController{};
};

#endif