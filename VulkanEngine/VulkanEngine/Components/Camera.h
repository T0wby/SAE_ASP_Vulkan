#ifndef CAMERA_H
#define CAMERA_H
#include "Component.h"
#include <glm/glm/glm.hpp>

class CCamera : public IComponent
{
public:
	inline CCamera(int a_iWidth, int a_iHeight, glm::vec3 a_pos, glm::vec3 a_orient, glm::vec3 a_up)
		: m_iWidth(a_iWidth), m_iHeight(a_iHeight), m_pos(a_pos), m_orientation(a_orient), m_up(a_up) {}

	CCamera(const CCamera&) = default;
	CCamera(CCamera&&) = default;
	CCamera& operator= (const CCamera&) = default;
	CCamera& operator= (CCamera&&) = default;
	~CCamera() = default;

	// Inherited via IComponent
	int Initialize(void) override;
	int Initialize(VkCommandBuffer a_commandBuffer) override;
	int Update(void) override;
	void Draw(void) override;
	void Draw(const DrawInformation& a_drawInformation) override;
	void Finalize(void) override;

	auto GetViewMatrix(void) const -> const glm::mat4;
	auto GetViewMatrix(const glm::vec3& a_pos) const -> const glm::mat4;
	auto GetProjectionMatrix(void) const -> const glm::mat4;
	auto GetCamMatrix(void) const -> const glm::mat4;

	inline auto GetOrientation(void) const -> const glm::vec3 { return m_orientation; }
	inline auto GetUp(void) const -> const glm::vec3 { return m_up; }
	inline auto GetSpeed(void) const -> const float { return m_fSpeed; }
	void CalcOrientation(glm::vec3 a_front);
	void UpdateSizeValues(const int& a_iWidth, const int& a_iHeight);

private:
	// ScreenSize
	int m_iWidth{ 0 };
	int m_iHeight{ 0 };

	glm::mat4 m_view{};
	glm::mat4 m_projection{};

	glm::vec3 m_pos = {};
	glm::vec3 m_orientation = {};
	glm::vec3 m_up = {};
	float m_fSpeed = 5.0f;
};
#endif
