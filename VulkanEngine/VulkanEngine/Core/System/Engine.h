#ifndef ENGINE_H
#define ENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "CoreSystemStructs.h"
#include "../../GameObjects/GameObject.h"


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
	void MainLoop(void);
	void DrawFrame(void);
	void Cleanup(void);
	void CleanupFrameBuffer(void);
	void CleanupSwapChain(void);
	void CleanupUniformBuffers(void);
	void PickPhysicalDevice(void);
	bool IsDeviceSuitable(VkPhysicalDevice a_device);
	void CreateVulkanInstance(void);
	bool CheckValidationLayerSupport(const std::vector<const char*> a_enabled_layers);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice a_device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice a_device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentationModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateLogicalDevice(void);
	void CreateGLFWSurface(void);
	void CreateSwapChain(void);
	void CreateImageViews(void);
	void DestroyImageViews(void);
	void CreateGraphicsPipeline(void);
	void CreateRenderPass(void);
	void CreateFrameBuffers(void);
	void CreateCommandPool(void);
	void CreateDescriptorPool(void);
	void CreateDescriptorSets(void);

	// Done in Mesh?
	void CreateVertexBuffer(void);
	void CreateIndexBuffer(void);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void CreateCommandBuffers(void);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	VkCommandBuffer BeginSingleTimeCommands(void);
	void EndSingleTimeCommands(VkCommandBuffer a_commandBuffer);
	void CreateSyncObjects(void);
	void RecreateSwapChain(void);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateDescriptorSetLayout(void);
	void CreateUniformBuffers(void);
	void UpdateUniformBuffer(uint32_t a_currentImage);
	void CreateTextureImage(void);
	void CreateImage(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkImage& a_image, VkDeviceMemory& a_imageMemory);
	void TransitionImageLayout(VkImage a_image, VkFormat a_format, VkImageLayout a_oldLayout, VkImageLayout a_newLayout);
	void CopyBufferToImage(VkBuffer a_buffer, VkImage a_image, uint32_t a_width, uint32_t a_height);
	void CreateTextureImageView(void);
	VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
	void CreateTextureSampler(void);
	void CreateDepthResources(void);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& a_candidates, VkImageTiling a_tiling, VkFormatFeatureFlags a_features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat a_format);

	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	bool m_bEnableValidationLayers{true};
	VkInstance m_vInstance;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_logicelDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentationQueue;
	VkSurfaceKHR m_surface;
	VkRenderPass m_renderPass;
	VkDescriptorSetLayout m_descriptorSetLayout;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;
	VkCommandPool m_commandPool;
	std::vector<VkCommandBuffer> m_vCommandBuffers;

	// SwapChain
	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_vSwapChainImages;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	// Image
	std::vector<VkImageView> m_vSwapChainImageViews;

	// Shader
	VkShaderModule CreateShaderModule(const std::vector<char>& a_vBytecode);

	// FrameBuffers
	std::vector<VkFramebuffer> m_vSwapChainFramebuffers;

	// semaphore and fence
	std::vector<VkSemaphore> m_vImageAvailableSemaphores;
	std::vector<VkSemaphore> m_vRenderFinishedSemaphores;
	std::vector<VkFence> m_vInFlightFences;

	uint32_t m_iCurrentFrame{ 0 };

	// VertexBuffer
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;
	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	std::vector<VkBuffer> m_vUniformBuffers;
	std::vector<VkDeviceMemory> m_vUniformBuffersMemory;
	std::vector<void*> m_vUniformBuffersMapped;

	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_vDescriptorSets;

	VkImage m_textureImage;
	VkDeviceMemory m_textureImageMemory;

	VkImageView m_textureImageView;
	VkSampler m_textureSampler;

	VkImage m_depthImage;
	VkDeviceMemory m_depthImageMemory;
	VkImageView m_depthImageView;

	// Temporary
	std::vector<std::shared_ptr<CGameObject>> m_vSceneObjects{};
};

#endif