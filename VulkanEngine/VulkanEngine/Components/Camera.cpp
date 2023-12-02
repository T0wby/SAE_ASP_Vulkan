#include "Camera.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/vector_angle.hpp>

int CCamera::Initialize(void)
{
    return 0;
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
    return glm::lookAt(m_pos, m_pos + m_orientation, m_up);
}

auto CCamera::GetProjectionMatrix() const -> const glm::mat4
{
    return glm::perspective(glm::radians(90.0f), static_cast<float>(m_iWidth / m_iHeight), 0.0f, 1.0f);
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
