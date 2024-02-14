#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>
#include "../Components/Camera.h"
#include "../WindowGLFW/Window.h"
#include "../GameObjects/GameObject.h"

class CPlayerController
{
public:

	CPlayerController() = default;
	CPlayerController(const CPlayerController&) = default;
	CPlayerController(CPlayerController&&) = default;
	CPlayerController& operator= (const CPlayerController&) = default;
	CPlayerController& operator= (CPlayerController&&) = default;
	~CPlayerController() = default;

	int Initialize(const std::shared_ptr<CWindow>& a_pWindow, const std::shared_ptr<CGameObject>& a_pGameObject, const float& a_fDeltaTime);
	int Update(const double& a_dDeltaTime);
	void Finalize(void);

	void SetExitInput(const std::function<void(void)>& a_pExitInput);
	void SetForwardInput(const std::function<void(void)>& a_pForwardInput);
	void SetBackwardInput(const std::function<void(void)>& a_pBackwardInput);
	void SetRightInput(const std::function<void(void)>& a_pRightInput);
	void SetLeftInput(const std::function<void(void)>& a_pLeftInput);
	void SetUpInput(const std::function<void(void)>& a_pUpInput);
	void SetDownInput(const std::function<void(void)>& a_pDownInput);
	void SetMouseMode(const std::function<void(void)>& a_pMouseMode);
	void SetSceneSwitch(const std::function<void(void)>& a_pSceneSwitch);

private:
	void SetDefaultInputGO(void);
	void CheckKeys(void);
	float m_fDeltaTime{ 0.0f };
	bool m_bMouseOn {true};
};
#endif
