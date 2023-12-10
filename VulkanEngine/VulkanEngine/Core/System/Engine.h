#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
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
	~CEngine();

	void Run(void);

private:
	std::shared_ptr<CWindow> m_pWindow = nullptr;
	std::shared_ptr<CDevice> m_pDevice{nullptr};
	std::shared_ptr<CRenderer> m_pRenderer{nullptr};
	// Scenes
	std::shared_ptr<CDefaultScene> m_firstScene{nullptr};

	// Input
	std::shared_ptr<CPlayerController> m_playerController{nullptr};
	// Delta time
	double m_dDeltaTime{ 0 };
	double m_dLastFrame{ 0 };
	double m_dCurrentFrame{ 0 };
	
	void InitializeVulkan(void);
	void InitializeWindow(void);
	void CreateInput(void);
	void CreateScenes(void);
	void MainLoop(void);
	void Cleanup(void);
};

#endif