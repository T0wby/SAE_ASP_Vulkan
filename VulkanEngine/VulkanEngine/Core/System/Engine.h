#ifndef ENGINE_H
#define ENGINE_H
#include <memory>

#include "Descriptors.h"
#include "../../Input/PlayerController.h"
#include "Device.h"
#include "Renderer.h"
#include "Scenes/DefaultScene.h"


class CEngine
{
public:
	CEngine() = default;
	CEngine(const CEngine&) = delete;
	CEngine(CEngine&&) = default;
	CEngine& operator= (const CEngine&) = delete;
	CEngine& operator= (CEngine&&) = delete;
	~CEngine();

	void Run(void);

private:
	std::shared_ptr<CWindow> m_pWindow = nullptr;
	std::shared_ptr<CDevice> m_pDevice{nullptr};
	std::shared_ptr<CRenderer> m_pRenderer{nullptr};
	std::unique_ptr<CDescriptorPool> m_pGlobalPool{nullptr};
	std::unique_ptr<CDescriptorSetLayout> m_pDescriptorSetLayout{nullptr};
	std::vector<VkDescriptorSet> m_vGlobalDescriptorSets{};
	std::vector<std::unique_ptr<CBuffer>> m_uboBuffers{};
	
	// Scenes
	std::vector<std::shared_ptr<CScene>> m_vScenes{};
	std::shared_ptr<CScene> m_pCurrScene{nullptr};
	int m_iCurrSceneNum{0};
	bool m_bSwitchScenes{false};

	// Input
	std::shared_ptr<CPlayerController> m_playerController{nullptr};
	std::shared_ptr<CPlayerController> m_playerController2{nullptr};
	// Delta time
	double m_dDeltaTime{ 0 };
	double m_dLastFrame{ 0 };
	double m_dCurrentFrame{ 0 };
	
	void InitializeVulkan(void);
	void EngineSetup(void);
	void InitializeWindow(void);
	void CreateInput(void);
	void CreateScenes(void);
	void MainLoop(void);
	void Cleanup(void);
};

#endif