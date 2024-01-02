#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../Utility/Variables.h"
#include "../Core/System/Device.h"

class CGameObject
{
public:
	using id_t = unsigned int;
	using Map = std::unordered_map<id_t, CGameObject>;
	static CGameObject CreateGameObject(const std::shared_ptr<CDevice>& a_pDevice)
	{
		static id_t currentId = 0;
		return CGameObject{a_pDevice, currentId++};
	}

	CGameObject(const CGameObject&) = delete;
	CGameObject(CGameObject&&) = default;
	CGameObject& operator= (const CGameObject&) = delete;
	CGameObject& operator= (CGameObject&&) = default;
	virtual ~CGameObject() = default;


	virtual void Initialize(void);
	virtual void Initialize(VkCommandBuffer a_commandBuffer);
	virtual void Update(const double& a_dDeltaTime);
	virtual void Draw(void);
	virtual void Draw(const DrawInformation& a_drawInformation);
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

	
	inline auto GetID(void) const -> const id_t { return m_id; }
	inline auto GetPosition(void) const -> const glm::vec3 { return m_pTransform->GetPosition(); }
	inline void AddPosition(const glm::vec3 a_pos) const { m_pTransform->AddPosition(a_pos); }
	inline void SetPosition(const glm::vec3 a_pos) const { m_pTransform->SetPosition(a_pos); }
	inline void AddRotation(const glm::vec3 a_rotation) const {	m_pTransform->AddRotation(a_rotation); }
	inline void SetRotation(const glm::vec3 a_rotation) const {	m_pTransform->SetRotation(a_rotation); }
	inline void AddScale(const glm::vec3 a_scale) const {	m_pTransform->AddScale(a_scale); }
	inline void SetScale(const glm::vec3 a_scale) const {	m_pTransform->SetScale(a_scale); }

	virtual std::vector<Vertex>& GetMeshVertexData(void);
	virtual std::vector<uint16_t>& GetMeshIndiceData(void);

protected:
	inline CGameObject(const std::shared_ptr<CDevice>& a_pDevice, id_t a_objId)
	{
		m_pTransform = std::make_shared<CTransform>();
		m_pDevice = a_pDevice;
		m_id = a_objId;
	}
	id_t m_id;
	std::vector<std::shared_ptr<IComponent>> m_components{};
	std::shared_ptr<CTransform> m_pTransform{ nullptr };
	std::shared_ptr<CDevice> m_pDevice{nullptr};
};

#endif