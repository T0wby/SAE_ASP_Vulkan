#include "Transform.h"
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/transform.hpp>

int CTransform::Initialize(void)
{
	//update the position matrix
	m_positionMatrix = glm::translate(m_position);

	//update the rotation matrix of this class
	m_rotationMatrix = glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);

	//update the scale matrix
	m_scaleMatrix = glm::scale(m_scale);

	//update the transform matrix
	m_transformMatrix = m_positionMatrix * m_rotationMatrix * m_scaleMatrix;
    return 0;
}

int CTransform::Initialize(const VkCommandBuffer& a_commandBuffer)
{
	return 0;
}

int CTransform::Update(const double& a_dDeltaTime)
{
	//update the position matrix
	m_positionMatrix = glm::translate(m_position);

	//update the rotation matrix
	m_rotationMatrix = glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);

	//update the scale matrix
	m_scaleMatrix = glm::scale(m_scale);

	//update the transform matrix
	m_transformMatrix = m_positionMatrix * m_rotationMatrix * m_scaleMatrix;

    return 0;
}

void CTransform::Draw(void)
{
}

void CTransform::Draw(const DrawInformation& a_drawInformation)
{
}

void CTransform::Finalize(void)
{
}
