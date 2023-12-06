#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../Utility/Variables.h"
#include "../Core/System/Device.h"

class CGameObject
{
public:
	inline CGameObject(const std::shared_ptr<CDevice>& a_pDevice)
	{
		m_pTransform = std::make_shared<CTransform>();
		m_pDevice = a_pDevice;
	}

	CGameObject(const CGameObject&) = default;
	CGameObject(CGameObject&&) = default;
	CGameObject& operator= (const CGameObject&) = default;
	CGameObject& operator= (CGameObject&&) = default;
	virtual ~CGameObject() = default;

	virtual void Initialize(void);
	virtual void Initialize(VkCommandBuffer a_commandBuffer);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Draw(VkCommandBuffer a_commandBuffer);
	virtual void Finalize(void);

	void AddComponent(std::shared_ptr<IComponent> a_component);
	void RemoveComponent(std::shared_ptr<IComponent> a_component);

	template <typename T>
	std::shared_ptr<T> GetComponent() const {
		// Iterate through components to find the one of the specified type
		for (const auto& component : m_components)
		{
			if (typeid(T) == typeid(*component))
			{
				// If found, cast and return the component
				return std::dynamic_pointer_cast<T>(component);
			}
		}
		// If no component of the specified type is found, return nullptr
		return nullptr;
	}

	inline auto GetPos(void) const -> const glm::vec3 { return m_pTransform->GetPosition(); }
	inline void AddPosition(const glm::vec3 a_pos) const
	{
		m_pTransform->AddPosition(a_pos);
	}
	inline void SetPosition(const glm::vec3 a_pos) const
	{
		m_pTransform->AddPosition(a_pos);
	}

	virtual std::vector<Vertex>& GetMeshVertexData(void);
	virtual std::vector<uint16_t>& GetMeshIndiceData(void);

protected:
	std::vector<std::shared_ptr<IComponent>> m_components{};
	std::shared_ptr<CTransform> m_pTransform{ nullptr };
	std::shared_ptr<CDevice> m_pDevice{nullptr};
};

#endif