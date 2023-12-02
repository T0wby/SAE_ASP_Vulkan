#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>
#include "../Components/Camera.h"
#include "../WindowGLFW/Window.h"

class CPlayerController
{
public:

	CPlayerController() = default;
	CPlayerController(const CPlayerController&) = default;
	CPlayerController(CPlayerController&&) = default;
	CPlayerController& operator= (const CPlayerController&) = default;
	CPlayerController& operator= (CPlayerController&&) = default;
	~CPlayerController() = default;

	int Initialize(std::shared_ptr<CWindow> a_pWindow, std::shared_ptr<CCamera> a_pCamera, float& a_fDeltaTime);
	int Update(float& a_fDeltaTime);
	void Finalize(void);

	void SetExitInput(std::function<void(void)> a_pExitInput);
	void SetForwardInput(std::function<void(void)> a_pForwardInput);
	void SetBackwardInput(std::function<void(void)> a_pBackwardInput);
	void SetRightInput(std::function<void(void)> a_pRightInput);
	void SetLeftInput(std::function<void(void)> a_pLeftInput);
	void SetUpInput(std::function<void(void)> a_pUpInput);
	void SetDownInput(std::function<void(void)> a_pDownInput);
	void SetMouseMode(std::function<void(void)> a_pMouseMode);

private:
	void SetDefaultInput(void);
	void CheckKeys(void);
	float m_fDeltaTime{ 0.0f };
};
#endif
