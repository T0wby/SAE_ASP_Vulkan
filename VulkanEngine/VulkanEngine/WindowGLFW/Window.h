#ifndef WINDOW_H
#define WINDOW_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

class CWindow
{
public:
	inline CWindow(int a_iWidth, int a_iHeight, const std::string& a_sTitle)
		: m_iWidth(a_iWidth), m_iHeight(a_iHeight), m_sTitle(a_sTitle) {};
	CWindow(const CWindow&) = default;
	CWindow(CWindow&&) = default;
	CWindow& operator= (const CWindow&) = default;
	CWindow& operator= (CWindow&&) = default;
	~CWindow() = default;

	void Initialize(void);
	void Update(void);
	void Finalize(void);

	auto GetWindowShouldClose(void) const->const bool;
	void GetWindowFrameBufferSize(int& a_iWidth, int& a_iHeight);
	void SetWindowShouldClose(const bool& a_bShouldClose);
	auto IsFrameBufferResized(void) const->const bool;
	void SetIsFrameBufferResized(const bool& a_bFrameBufferResized);
	void CheckIfWindowMinimized(void);

	GLFWwindow* GetWindow(void);

private:

	int m_iWidth{ 0 };
	int m_iHeight{ 0 };
	std::string m_sTitle{};
	bool m_bFrameBufferResized{ false };

};

#endif