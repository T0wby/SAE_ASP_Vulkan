#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <vector>
#include "../../Input/PlayerController.h"
#include "Device.h"
#include "Renderer.h"
#include "SwapChain.h"
#include "Scenes/CDefaultScene.h"


class CEngine
{
public:
	CEngine() = default;
	CEngine(const CEngine&) = delete;
	CEngine(CEngine&&) = default;
	CEngine& operator= (const CEngine&) = delete;
	CEngine& operator= (CEngine&&) = delete;
	~CEngine() = default;

	void Run(void);

private:
	std::shared_ptr<CWindow> m_pWindow = nullptr;
	std::shared_ptr<CDevice> m_pDevice{nullptr};
	std::shared_ptr<CRenderer> m_pRenderer{nullptr};
	const std::vector<const char*> m_EnabledLayers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> m_EnabledExtensions = { "VK_KHR_swapchain" };
	bool m_bEnableValidationLayers{true};
	std::shared_ptr<VkInstance> m_vInstance{};
	// Scenes
	std::shared_ptr<CDefaultScene> m_firstScene{};

	// Input
	std::shared_ptr<CPlayerController> m_playerController{};
	
	void InitializeVulkan(void);
	void InitializeWindow(void);
	void CreateInput(void);
	void CreateScenes(void);
	void MainLoop(void);
	void Cleanup(void);
	void CreateGLFWSurface(void);
	void CreateVulkanInstance(void);
	bool CheckValidationLayerSupport(const std::vector<const char*>& a_enabled_layers);
};

#endif