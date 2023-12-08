#include "GameObject.h"
#include <iostream>

void CGameObject::Initialize(void)
{
	AddComponent(m_pTransform);
	for (std::shared_ptr<IComponent> component : m_components)
	{
		component->Initialize(); // calls the Initialize function of each component
	}
}

void CGameObject::Initialize(VkCommandBuffer a_commandBuffer)
{
	for (std::shared_ptr<IComponent> component : m_components)
	{
		component->Initialize(a_commandBuffer); // calls the Initialize function of each component
	}
}

void CGameObject::Update(void)
{
	for (std::shared_ptr<IComponent> component : m_components)
	{
		component->Update(); // calls the update function of each component
	}
}

void CGameObject::Draw(void)
{
	for (std::shared_ptr<IComponent> component : m_components)
	{
		component->Draw(); // calls the draw function of each component
	}
}

void CGameObject::Draw(DrawInformation& a_drawInformation)
{
	SimplePushConstantData push{};
	push.offset = m_pTransform->GetPosition();
	push.color = {0.0f, 0.0f, 0.2f + 0.2f * m_pTransform->GetPosition().x};

	vkCmdPushConstants(a_drawInformation.commandBuffer, a_drawInformation.pipelineLayout,
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
	
	for (std::shared_ptr<IComponent> component : m_components)
	{
		component->Draw(a_drawInformation); // calls the draw function of each component
	}
}

void CGameObject::Finalize()
{
	for (std::shared_ptr<IComponent> component : m_components)
	{
		component->Finalize();
	}
}

void CGameObject::AddComponent(std::shared_ptr<IComponent> a_component)
{
	if (a_component == nullptr) 
	{
		std::cout << "Failed to add Component, due to a nullptr!" << std::endl;
		return;
	}

	m_components.push_back(a_component);
}

void CGameObject::RemoveComponent(std::shared_ptr<IComponent> a_component)
{
	if (a_component == nullptr)
	{
		std::cout << "Failed to remove Component, due to a nullptr!" << std::endl;
		return;
	}

	for (int i = 0; i < m_components.size(); i++)
	{
		if (a_component == m_components[i])
		{
			m_components.erase(m_components.begin() + i);
			break;
		}
	}
}

std::vector<Vertex>& CGameObject::GetMeshVertexData(void)
{
	std::vector<Vertex> empty;
	return  empty;
}

std::vector<uint16_t>& CGameObject::GetMeshIndiceData(void)
{
	std::vector<uint16_t> empty;
	return empty;
}
