#ifndef ENGINE_H
#define ENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "CoreSystemStructs.h"

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
	void Cleanup(void);
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

	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	bool m_bEnableValidationLayers{true};
	VkInstance m_vInstance;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_logicelDevice;
	VkQueue m_graphicsQueue;
	VkQueue m_presentationQueue;
	VkSurfaceKHR m_surface;

	// SwapChain
	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_vSwapChainImages;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	// Image
	std::vector<VkImageView> m_vSwapChainImageViews;

	// Shader
	VkShaderModule CreateShaderModule(const std::vector<char>& a_vBytecode);
};

#endif