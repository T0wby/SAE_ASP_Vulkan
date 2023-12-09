#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <memory>
#include <GLFW/glfw3.h>
#include "Device.h"
#include "Scene.h"
#include "../../WindowGLFW/Window.h"
#include "../../Core/System/CoreSystemStructs.h"

class CSwapChain
{
public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
	inline CSwapChain(const CDevice& a_device, const std::shared_ptr<CWindow>& a_pWindow)
			: m_device(a_device), m_pWindow(a_pWindow)
	{
		Init();
	}
	
	inline CSwapChain(const CDevice& a_device, const std::shared_ptr<CWindow>& a_pWindow, const std::shared_ptr<CSwapChain>& a_pSwapChainPrevious)
			: m_device(a_device), m_pWindow(a_pWindow), m_pSwapChainOld(a_pSwapChainPrevious)
	{
		Init();
		m_pSwapChainOld = nullptr;
	}
    
	CSwapChain(const CSwapChain&) = delete;
	CSwapChain(CSwapChain&&) = delete;
	CSwapChain& operator= (const CSwapChain&) = delete;
	CSwapChain& operator= (CSwapChain&&) = delete;
	~CSwapChain() = default;

	void Finalize(void);
	void CreateTextures(void);
	VkResult AquireNextImage(uint32_t& a_imageIndex);
	VkResult SubmitCommandBuffers(const VkCommandBuffer& buffers, const uint32_t& a_imageIndex,
		const std::shared_ptr<CScene>& a_pScene);
	VkFormat FindDepthFormat();
	void CreateDescriptorSetLayout(void);
	void CreateDescriptorPool(void);
	void CreateDescriptorSets(void);
	void CreateUniformBuffers(void);

	inline VkFramebuffer GetFrameBuffer(const int& a_iIndex) const { return m_vSwapChainFramebuffers[a_iIndex]; }
	inline VkRenderPass GetRenderPass() const { return m_renderPass; }
	inline VkImageView GetImageView(const int& a_iIndex) const { return m_vSwapChainImageViews[a_iIndex]; }
	inline size_t GetImageCount() const { return m_vSwapChainImages.size(); }
	inline VkFormat GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
	inline VkFormat GetSwapChainDepthFormat() const { return m_swapChainDepthFormat; }
	inline VkExtent2D GetSwapChainExtent() const { return m_swapChainExtent; }
	inline uint32_t GetWidth() const { return m_swapChainExtent.width; }
	inline uint32_t GetHeight() const { return m_swapChainExtent.height; }
	inline uint32_t GetCurrentFrame() const { return m_iCurrentFrame; }
	inline std::vector<VkDescriptorSet> GetDescriptorSets() const { return m_vDescriptorSets; }
	inline VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
	inline void SetDescriptorSetLayout(VkDescriptorSetLayout a_layout){ m_descriptorSetLayout = a_layout; }
	inline bool CompareSwapFormats(const CSwapChain& a_swapChain) const
	{
		return a_swapChain.GetSwapChainDepthFormat() == m_swapChainDepthFormat &&
			a_swapChain.GetSwapChainImageFormat() == m_swapChainImageFormat;
	}
	
	static bool IsDeviceSuitable(VkPhysicalDevice a_device, VkSurfaceKHR a_surface, const std::vector<const char*> a_enabledExtensions);
	static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_device, VkSurfaceKHR a_surface);
	static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice a_device, VkSurfaceKHR a_surface);
	static bool CheckDeviceExtensionSupport(VkPhysicalDevice a_device, const std::vector<const char*> a_EnabledExtensions);

private:
	CDevice m_device;
	std::shared_ptr<CWindow> m_pWindow{nullptr};
	std::shared_ptr<CSwapChain> m_pSwapChainOld{nullptr};
	VkSwapchainKHR m_swapChain{};
	std::vector<VkImage> m_vSwapChainImages{};
	VkFormat m_swapChainImageFormat{};
	VkFormat m_swapChainDepthFormat{};
	VkExtent2D m_swapChainExtent{};
	std::vector<VkImageView> m_vSwapChainImageViews{};
	std::vector<VkFramebuffer> m_vSwapChainFramebuffers{};
	VkRenderPass m_renderPass{};
	std::vector<VkSemaphore> m_vImageAvailableSemaphores{};
	std::vector<VkSemaphore> m_vRenderFinishedSemaphores{};
	std::vector<VkFence> m_vInFlightFences{};
	uint32_t m_iCurrentFrame{ 0 };
	VkImage m_depthImage{};
	VkDeviceMemory m_depthImageMemory{};
	VkImageView m_depthImageView{};
	std::vector<VkBuffer> m_vUniformBuffers{};
	std::vector<VkDeviceMemory> m_vUniformBuffersMemory{};
	std::vector<void*> m_vUniformBuffersMapped{};
	VkDescriptorSetLayout m_descriptorSetLayout{};
	VkDescriptorPool m_descriptorPool{};
    std::vector<VkDescriptorSet> m_vDescriptorSets{};
	VkImage m_textureImage{};
	VkDeviceMemory m_textureImageMemory{};
	VkImageView m_textureImageView{};
	VkSampler m_textureSampler{};
	
	void Init(void);
	void CreateSwapChain(void);
	void CreateImageViews(void);
	void CreateRenderPass(void);
	void CreateFrameBuffers(void);
	void CreateSyncObjects(void);
	void CreateDepthResources(void);
	void CleanupSwapChain(void);
	void DestroyImageViews(void);
	void CleanupFrameBuffer(void);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentationModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	void CreateImage(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkImage& a_image, VkDeviceMemory& a_imageMemory);
	VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
	void TransitionImageLayout(VkImage a_image, VkFormat a_format, VkImageLayout a_oldLayout, VkImageLayout a_newLayout);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& a_candidates, VkImageTiling a_tiling, VkFormatFeatureFlags a_features) const;
	bool HasStencilComponent(VkFormat a_format);
	void UpdateUniformBuffer(const std::shared_ptr<CScene>& a_pScene);

	void CreateTextureImage(void);
	void CopyBufferToImage(VkBuffer a_buffer, VkImage a_image, uint32_t a_width, uint32_t a_height);
	void CreateTextureImageView(void);
	void CreateTextureSampler(void);
};

#endif
