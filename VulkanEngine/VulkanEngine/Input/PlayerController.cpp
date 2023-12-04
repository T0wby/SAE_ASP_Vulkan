#include "PlayerController.h"
const int I_SUCCESS = 0;
const int I_USERINPUT_INIT_FAILED = -5;

std::shared_ptr<CWindow> pCurrWindow = nullptr;
std::shared_ptr<CCamera> pCCamera = nullptr;

// Key Input
std::function<void(void)> pExitInput{ []() {} };
std::function<void(void)> pForwardInput{ []() {} };
std::function<void(void)> pBackwardInput{ []() {} };
std::function<void(void)> pRightInput{ []() {} };
std::function<void(void)> pLeftInput{ []() {} };
std::function<void(void)> pUpInput{ []() {} };
std::function<void(void)> pDownInput{ []() {} };
std::function<void(void)> pMouseMode{ []() {} };

// Mouse Input
float fPitch{ 0.0f };
float fYaw{ -90.0f };
float fLastPosX{ 400.0f };
float fLastPosY{ 400.0f };
float fOffsetX{ 0.0f };
float fOffsetY{ 0.0f };
const float F_SENSE{ 0.01f };

void MouseInput(GLFWwindow* a_pWindow, double a_dXPos, double a_dYPos)
{
	fOffsetX = static_cast<float>(a_dXPos) - fLastPosX;
	fOffsetY = fLastPosY - static_cast<float>(a_dYPos); // reversed since y-coordinates range from bottom to top
	fLastPosX = static_cast<float>(a_dXPos);
	fLastPosY = static_cast<float>(a_dYPos);

	fOffsetX *= F_SENSE;
	fOffsetY *= F_SENSE;

	fYaw += fOffsetX;
	fPitch += fOffsetY;

	if (fPitch > 89.0f)
		fPitch = 89.0f;
	if (fPitch < -89.0f)
		fPitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	direction.y = sin(glm::radians(fPitch));
	direction.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	pCCamera->CalcOrientation(direction);
}

int CPlayerController::Initialize(std::shared_ptr<CWindow> a_pWindow, std::shared_ptr<CCamera> a_pCamera, float& a_fDeltaTime)
{
	m_fDeltaTime = a_fDeltaTime;
	pCCamera = a_pCamera;

	pCurrWindow = a_pWindow;

	SetDefaultInput();

	glfwSetCursorPosCallback(pCurrWindow->GetWindow().get(), MouseInput);
	return I_SUCCESS;
}

int CPlayerController::Update(float& a_fDeltaTime)
{
	m_fDeltaTime = a_fDeltaTime;
	CheckKeys();
	return I_SUCCESS;
}

void CPlayerController::Finalize(void)
{
}

void CPlayerController::SetExitInput(std::function<void(void)> a_pExitInput)
{
	pExitInput = a_pExitInput;
}

void CPlayerController::SetForwardInput(std::function<void(void)> a_pForwardInput)
{
	pForwardInput = a_pForwardInput;
}

void CPlayerController::SetBackwardInput(std::function<void(void)> a_pBackwardInput)
{
	pBackwardInput = a_pBackwardInput;
}

void CPlayerController::SetRightInput(std::function<void(void)> a_pRightInput)
{
	pRightInput = a_pRightInput;
}

void CPlayerController::SetLeftInput(std::function<void(void)> a_pLeftInput)
{
	pLeftInput = a_pLeftInput;
}

void CPlayerController::SetUpInput(std::function<void(void)> a_pUpInput)
{
	pUpInput = a_pUpInput;
}

void CPlayerController::SetDownInput(std::function<void(void)> a_pDownInput)
{
	pDownInput = a_pDownInput;
}

void CPlayerController::SetMouseMode(std::function<void(void)> a_pMouseMode)
{
	pMouseMode = a_pMouseMode;
}

void CPlayerController::SetDefaultInput(void)
{
	pExitInput = ([]() { pCurrWindow->SetWindowShouldClose(true); });
	pForwardInput = ([this]() { pCCamera->SetPosition(glm::vec3(pCCamera->GetSpeed() * m_fDeltaTime * pCCamera->GetOrientation())); });
	pBackwardInput = ([this]() { pCCamera->SetPosition(glm::vec3(pCCamera->GetSpeed() * m_fDeltaTime * -pCCamera->GetOrientation())); });
	pRightInput = ([this]() { pCCamera->SetPosition(glm::vec3(pCCamera->GetSpeed() * m_fDeltaTime * glm::normalize(glm::cross(pCCamera->GetOrientation(), pCCamera->GetUp())))); });
	pLeftInput = ([this]() { pCCamera->SetPosition(glm::vec3(pCCamera->GetSpeed() * m_fDeltaTime * -glm::normalize(glm::cross(pCCamera->GetOrientation(), pCCamera->GetUp())))); });
	pUpInput = ([this]() { pCCamera->SetPosition(glm::vec3(pCCamera->GetSpeed() * m_fDeltaTime * pCCamera->GetUp())); });
	pDownInput = ([this]() { pCCamera->SetPosition(glm::vec3(pCCamera->GetSpeed() * m_fDeltaTime * -pCCamera->GetUp())); });
}

void CPlayerController::CheckKeys(void)
{
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		pExitInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		pMouseMode();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_W) == GLFW_PRESS)
	{
		pForwardInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_S) == GLFW_PRESS)
	{
		pBackwardInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_D) == GLFW_PRESS)
	{
		pRightInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_A) == GLFW_PRESS)
	{
		pLeftInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_E) == GLFW_PRESS)
	{
		pUpInput();
	}
	if (glfwGetKey(pCurrWindow->GetWindow().get(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		pDownInput();
	}
}
