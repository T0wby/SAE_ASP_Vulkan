#include "Camera.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/vector_angle.hpp>

int CCamera::Initialize(void)
{
    return 0;
}

int CCamera::Initialize(VkCommandBuffer a_commandBuffer)
{
    return 0;
}

void CCamera::Draw(const DrawInformation& a_drawInformation)
{
}

int CCamera::Update(void)
{
    return 0;
}

void CCamera::Draw(void)
{
}

void CCamera::Finalize(void)
{
}

auto CCamera::GetViewMatrix() const -> const glm::mat4
{
    auto look = glm::lookAt(m_pos, m_pos + m_orientation, m_up);
    return look;
}

auto CCamera::GetViewMatrix(const glm::vec3& a_pos) const -> const glm::mat4
{
    auto look = glm::lookAt(a_pos, a_pos + m_orientation, m_up);
    return look;
}

auto CCamera::GetProjectionMatrix() const -> const glm::mat4
{
    return glm::perspective(glm::radians(45.0f), m_iWidth / (float)m_iHeight, 0.1f, 10.0f);
}

auto CCamera::GetCamMatrix() const -> const glm::mat4
{
    auto view = glm::mat4x4(1.0f);
    view = glm::lookAt(m_pos, m_pos + m_orientation, m_up);
    auto projection = glm::mat4x4(1.0f);
    projection = GetProjectionMatrix();
    return projection * view;
}

void CCamera::CalcOrientation(glm::vec3 a_front)
{
    m_orientation = glm::normalize(a_front);
}

void CCamera::UpdateSizeValues(const int& a_iWidth, const int& a_iHeight)
{
    m_iWidth = a_iWidth;
    m_iHeight = a_iHeight;
}
