#include "Engine.h"
#include "../../WindowGLFW/Window.h"

// System
std::shared_ptr<CWindow> pWindow = nullptr;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void CEngine::Run(void)
{
	InitializeWindow();
	InitializeVulkan();
	MainLoop();
	Cleanup();
}

void CEngine::InitializeVulkan(void)
{
}

void CEngine::InitializeWindow(void)
{
	// Create GLFW window
	pWindow = std::make_shared<CWindow>(WIDTH, HEIGHT, "SAE_Tobi_Engine");
	pWindow->Initialize();
}

void CEngine::MainLoop(void)
{
	while (!pWindow->GetWindowShouldClose()) 
	{
		pWindow->Update();
	}
}

void CEngine::Cleanup(void)
{
	pWindow->Finalize();
}
