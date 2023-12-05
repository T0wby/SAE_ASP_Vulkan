#ifndef DEVICE_H
#define DEVICE_H
#include "SwapChain.h"

#include <memory>

class CDevice
{
public:
	inline CDevice(const std::shared_ptr<VkInstance>& a_vulkanInstance, VkSurfaceKHR a_surface)
		: m_vulkanInstance(a_vulkanInstance), m_surface(a_surface) {}

	inline auto GetPhysicalDevice(void) const -> const VkPhysicalDevice& { return m_physicalDevice; }
	inline auto GetLogicalDevice(void) const -> const VkDevice& { return m_logicalDevice; }
	inline auto GetGraphicsQueue(void) const -> const VkQueue& { return m_graphicsQueue; }
	inline auto GetPresentationQueue(void) const -> const VkQueue& { return m_presentationQueue; }
	inline auto GetCommandPool(void) const -> const VkCommandPool& { return m_commandPool; }

	void Initialize(void);
	void Finalize(void);

private:

	void PickPhysicalDevice(void);
	void CreateLogicalDevice(void);
	void CreateCommandPool(void);

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
