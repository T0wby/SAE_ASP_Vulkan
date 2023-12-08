#include "Transform.h"
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/transform.hpp>

int CTransform::Initialize(void)
{
	//update the positionmatrix of this class
	m_positionMatrix = glm::translate(m_position);

	//update the rotationmatrix of this class
	m_rotationMatrix = glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);

	//update the scalematrix of this class
	m_scaleMatrix = glm::scale(m_scale);

	//update the transformmatrix of this class
	m_transformMatrix = m_positionMatrix * m_rotationMatrix * m_scaleMatrix;
    return 0;
}

int CTransform::Initialize(VkCommandBuffer a_commandBuffer)
{
	return 0;
}

int CTransform::Update(void)
{
	//update the positionmatrix of this class
	m_positionMatrix = glm::translate(m_position);

	//update the rotationmatrix of this class
	m_rotationMatrix = glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);

	//update the scalematrix of this class
	m_scaleMatrix = glm::scale(m_scale);

	//update the transformmatrix of this class
	m_transformMatrix = m_positionMatrix * m_rotationMatrix * m_scaleMatrix;

    return 0;
}

void CTransform::Draw(void)
{
}

void CTransform::Draw(DrawInformation& a_drawInformation)
{
}

void CTransform::Finalize(void)
{
}
