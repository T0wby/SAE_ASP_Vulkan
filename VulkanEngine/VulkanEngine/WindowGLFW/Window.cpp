#include "Window.h"

GLFWwindow* pWindow = nullptr;

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) 
{
	auto app = reinterpret_cast<CWindow*>(glfwGetWindowUserPointer(window));
	app->SetIsFrameBufferResized(true);
}

void CWindow::Initialize(void)
{
	if (!glfwInit()) return;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	if (pWindow == nullptr) pWindow = glfwCreateWindow(m_iWidth, m_iHeight, m_sTitle.c_str(), nullptr, nullptr);

	glfwSetWindowUserPointer(pWindow, this);
	glfwSetFramebufferSizeCallback(pWindow, framebufferResizeCallback);

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

void CWindow::GetWindowFrameBufferSize(int& a_iWidth, int& a_iHeight)
{
	glfwGetFramebufferSize(pWindow, &a_iWidth, &a_iHeight);
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

auto CWindow::IsFrameBufferResized(void) const -> const bool
{
	return m_bFrameBufferResized;
}

void CWindow::SetIsFrameBufferResized(const bool& a_bFrameBufferResized)
{
	m_bFrameBufferResized = a_bFrameBufferResized;
}

void CWindow::CheckIfWindowMinimized(void)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(pWindow, &width, &height);
	while (width == 0 || height == 0) 
	{
		glfwGetFramebufferSize(pWindow, &width, &height);
		glfwWaitEvents();
	}
}

GLFWwindow* CWindow::GetWindow(void)
{
    if (pWindow == nullptr) return nullptr;

    return pWindow;
}
