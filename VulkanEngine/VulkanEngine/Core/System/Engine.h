#ifndef ENGINE_H
#define ENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "QueueFamilyIndices.h"

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
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice a_device);

	VkInstance m_vInstance;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

};

#endif