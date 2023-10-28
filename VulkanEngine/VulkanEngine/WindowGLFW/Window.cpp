#include "Window.h"

GLFWwindow* pWindow = nullptr;

void CWindow::Initialize(void)
{
	if (!glfwInit()) return;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (pWindow == nullptr) pWindow = glfwCreateWindow(m_iWidth, m_iHeight, m_sTitle.c_str(), nullptr, nullptr);

	if (!pWindow)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(pWindow);

	//glfwSetWindowSizeCallback(pWindow.get(), OnWindowSizeChange);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void CWindow::Update(void)
{
	glfwPollEvents();
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CWindow::Finalize(void)
{
	glfwDestroyWindow(pWindow);
	glfwTerminate();
}

auto CWindow::GetWindowShouldClose(void) const -> const bool
{
    return false;
}

void CWindow::SetWindowShouldClose(const bool& a_bShouldClose)
{
	if (a_bShouldClose)
	{
		glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
	}
	else
	{
		glfwSetWindowShouldClose(pWindow, GLFW_FALSE);
	}
}

GLFWwindow* CWindow::GetWindow(void)
{
    if (pWindow == nullptr) return nullptr;

    return pWindow;
}
