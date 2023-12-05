#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <memory>
#include <GLFW/glfw3.h>

#include "Device.h"
#include "../../WindowGLFW/Window.h"
#include "../../Core/System/CoreSystemStructs.h"

class CSwapChain
{
public:
	inline CSwapChain(const CDevice& a_device, const std::shared_ptr<CWindow>& a_pWindow, VkSurfaceKHR a_surface)
			: m_device(a_device), m_pWindow(a_pWindow), m_surface(a_surface)
	{
		CreateSwapChain();
		CreateImageViews();
		CreateRenderPass();
		CreateDepthResources();
		CreateFrameBuffers();
		CreateSyncObjects();
	}
    
	CSwapChain(const CSwapChain&) = delete;
	CSwapChain(CSwapChain&&) = delete;
	CSwapChain& operator= (const CSwapChain&) = delete;
	CSwapChain& operator= (CSwapChain&&) = delete;
	~CSwapChain() = default;

	void Finalize(void);
	void RecreateSwapChain(void);

	inline VkFramebuffer GetFrameBuffer(const int& a_iIndex) const { return m_vSwapChainFramebuffers[a_iIndex]; }
	inline VkRenderPass GetRenderPass() const { return m_renderPass; }
	inline VkImageView GetImageView(const int& a_iIndex) const { return m_vSwapChainImageViews[a_iIndex]; }
	inline size_t GetImageCount() const { return m_vSwapChainImages.size(); }
	inline VkFormat GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
	inline VkExtent2D GetSwapChainExtent() const { return m_swapChainExtent; }
	inline uint32_t GetWidth() const { return m_swapChainExtent.width; }
	inline uint32_t GetHeight() const { return m_swapChainExtent.height; }
	
	static bool IsDeviceSuitable(VkPhysicalDevice a_device, VkSurfaceKHR a_surface, const std::vector<const char*> a_enabledExtensions);
	static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_device, VkSurfaceKHR a_surface);
	static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice a_device, VkSurfaceKHR a_surface);
	static bool CheckDeviceExtensionSupport(VkPhysicalDevice a_device, const std::vector<const char*> a_EnabledExtensions);

private:
	CDevice m_device;
	std::shared_ptr<CWindow> m_pWindow{nullptr};
	VkSurfaceKHR m_surface{};
	VkSwapchainKHR m_swapChain{};
	std::vector<VkImage> m_vSwapChainImages{};
	VkFormat m_swapChainImageFormat{};
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
	VkFormat FindDepthFormat();
	void CreateImage(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkImage& a_image, VkDeviceMemory& a_imageMemory);
	VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
	void TransitionImageLayout(VkImage a_image, VkFormat a_format, VkImageLayout a_oldLayout, VkImageLayout a_newLayout);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& a_candidates, VkImageTiling a_tiling, VkFormatFeatureFlags a_features) const;
};

#endif
