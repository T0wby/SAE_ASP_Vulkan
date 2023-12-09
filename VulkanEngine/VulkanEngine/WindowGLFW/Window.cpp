#include "Window.h"
#include <stdexcept>


static void framebufferResizeCallback(GLFWwindow* window, int width, int height) 
{
	auto app = reinterpret_cast<CWindow*>(glfwGetWindowUserPointer(window));
	app->SetIsFrameBufferResized(true);
	app->SetSize(height, width);
}

void CWindow::Initialize(void)
{
	if (!glfwInit()) return;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);


	if (m_pWindow == nullptr) m_pWindow.reset(glfwCreateWindow(m_iWidth, m_iHeight, m_sTitle.c_str(), nullptr, nullptr));

	glfwSetWindowUserPointer(m_pWindow.get(), this);
	glfwSetFramebufferSizeCallback(m_pWindow.get(), framebufferResizeCallback);

	if (!m_pWindow)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_pWindow.get());

	glfwSetInputMode(m_pWindow.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void CWindow::Update(void)
{
	glfwPollEvents();
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CWindow::Finalize(void)
{
	glfwDestroyWindow(m_pWindow.get());
	glfwTerminate();
}

void CWindow::CreateWindowSurface(VkInstance a_vulkanInstance)
{
	if (glfwCreateWindowSurface(a_vulkanInstance, m_pWindow.get(), nullptr, &m_surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

auto CWindow::GetWindowShouldClose(void) const -> const bool
{
    return glfwWindowShouldClose(m_pWindow.get());
}

auto CWindow::GetExtent() const -> VkExtent2D
{
	return {static_cast<uint32_t>(m_iWidth), static_cast<uint32_t>(m_iHeight)};
}

auto CWindow::GetSurface() const -> VkSurfaceKHR
{
	return m_surface;
}

void CWindow::GetWindowFrameBufferSize(int& a_iWidth, int& a_iHeight)
{
	glfwGetFramebufferSize(m_pWindow.get(), &a_iWidth, &a_iHeight);
}

void CWindow::SetWindowShouldClose(const bool& a_bShouldClose)
{
	if (a_bShouldClose)
	{
		glfwSetWindowShouldClose(m_pWindow.get(), GLFW_TRUE);
	}
	else
	{
		glfwSetWindowShouldClose(m_pWindow.get(), GLFW_FALSE);
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

void CWindow::SetSize(const int& a_iHeight, const int& a_iWidth)
{
	m_iHeight = a_iHeight;
	m_iWidth = a_iWidth;
}

void CWindow::CheckIfWindowMinimized(void)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(m_pWindow.get(), &width, &height);
	while (width == 0 || height == 0) 
	{
		glfwGetFramebufferSize(m_pWindow.get(), &width, &height);
		glfwWaitEvents();
	}
}

std::shared_ptr<GLFWwindow> CWindow::GetWindow(void)
{
    if (m_pWindow == nullptr) return nullptr;

    return m_pWindow;
}
