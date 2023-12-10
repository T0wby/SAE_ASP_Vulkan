#ifndef DEVICE_H
#define DEVICE_H
#define GLFW_INCLUDE_VULKAN
#include <memory>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "../../WindowGLFW/Window.h"

class CDevice
{
public:
	inline CDevice(const std::shared_ptr<CWindow>& a_pWindow)
		: m_pWindow(a_pWindow)
	{
		CreateVulkanInstance();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateCommandPool();
	}
	~CDevice();

	inline auto GetPhysicalDevice(void) const -> const VkPhysicalDevice& { return m_physicalDevice; }
	inline auto GetLogicalDevice(void) const -> const VkDevice& { return m_logicalDevice; }
	inline auto GetGraphicsQueue(void) const -> const VkQueue& { return m_graphicsQueue; }
	inline auto GetPresentationQueue(void) const -> const VkQueue& { return m_presentationQueue; }
	inline auto GetCommandPool(void) const -> const VkCommandPool& { return m_commandPool; }
	inline std::shared_ptr<VkInstance> GetVulkanInstance(void) const { return m_vulkanInstance; }
	inline VkSurfaceKHR GetSurface(void) const { return m_surface; }


	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
	void CreateVulkanInstance(void);
	void CreateSurface(void);
	void PickPhysicalDevice(void);
	void CreateLogicalDevice(void);
	void CreateCommandPool(void);
	bool CheckValidationLayerSupport(const std::vector<const char*>& a_enabled_layers);

	std::shared_ptr<CWindow> m_pWindow{nullptr};

	std::shared_ptr<VkInstance> m_vulkanInstance{ nullptr };

	VkSurfaceKHR m_surface{};
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	bool m_bEnableValidationLayers{true};
	
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_logicalDevice{};
	VkQueue m_graphicsQueue{};
	VkQueue m_presentationQueue{};
	VkCommandPool m_commandPool{};
};
#endif
