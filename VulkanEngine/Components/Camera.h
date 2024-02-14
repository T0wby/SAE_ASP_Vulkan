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
	int Initialize(const VkCommandBuffer& a_commandBuffer) override;
	int Update(const double& a_dDeltaTime) override;
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
	inline void SetSpeed(const float& a_fSpeed) { m_fSpeed = a_fSpeed; }
	inline void AddSpeed(const float& a_fSpeed)
	{
		if (m_fSpeed + a_fSpeed > m_fMaxSpeed || m_fSpeed + a_fSpeed < m_fMinSpeed) return;
		m_fSpeed += a_fSpeed;
	}
	inline auto GetMinSpeed(void) const -> const float { return m_fMinSpeed; }
	inline auto GetMaxSpeed(void) const -> const float { return m_fMaxSpeed; }

	inline void SetFOV(const float& a_fFieldOfView) { m_fFieldOfView = a_fFieldOfView; }
	inline void SetNearPlane(const float& a_fNearPlane) { m_fNearPlane = a_fNearPlane; }
	inline void SetFarPlane(const float& a_fFarPlane) { m_fFarPlane = a_fFarPlane; }
	void CalcOrientation(glm::vec3 a_front);
	void UpdateSizeValues(const int& a_iWidth, const int& a_iHeight);

private:
	// ScreenSize
	int m_iWidth{ 0 };
	int m_iHeight{ 0 };
	float m_fFieldOfView{90.0f};
	float m_fNearPlane{0.1f};
	float m_fFarPlane{20.0f};

	glm::vec3 m_pos = {};
	glm::vec3 m_orientation = {};
	glm::vec3 m_up = {};
	float m_fSpeed{2.0f};
	float m_fMaxSpeed{10.0f};
	float m_fMinSpeed{0.1f};
};
#endif
