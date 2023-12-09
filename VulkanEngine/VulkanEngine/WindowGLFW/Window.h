#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Vulkan/Include/vulkan/vulkan.h>

#include <string>
#include <memory>

class CWindow
{
public:
	inline CWindow(int a_iWidth, int a_iHeight, const std::string& a_sTitle)
		: m_iWidth(a_iWidth), m_iHeight(a_iHeight), m_sTitle(a_sTitle) {};
	CWindow(const CWindow&) = delete;
	CWindow(CWindow&&) = default;
	CWindow& operator= (const CWindow&) = delete;
	CWindow& operator= (CWindow&&) = default;
	~CWindow() = default;

	void Initialize(void);
	void Update(void);
	void Finalize(void);

	void CreateWindowSurface(VkInstance a_vulkanInstance);

	auto GetWindowShouldClose(void) const->const bool;
	auto GetExtent(void) const -> VkExtent2D;
	auto GetSurface(void) const -> VkSurfaceKHR;
	void GetWindowFrameBufferSize(int& a_iWidth, int& a_iHeight);
	void SetWindowShouldClose(const bool& a_bShouldClose);
	auto IsFrameBufferResized(void) const->const bool;
	void SetIsFrameBufferResized(const bool& a_bFrameBufferResized);
	void SetSize(const int& a_iHeight, const int& a_iWidth);
	void CheckIfWindowMinimized(void);

	std::shared_ptr<GLFWwindow> GetWindow(void);

private:
	std::shared_ptr<GLFWwindow> m_pWindow{ nullptr };
	VkSurfaceKHR m_surface{};

	int m_iWidth{ 0 };
	int m_iHeight{ 0 };
	std::string m_sTitle{};
	bool m_bFrameBufferResized{ false };

};
