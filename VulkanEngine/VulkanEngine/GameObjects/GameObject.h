#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <memory>
#include "../Components/Component.h"
#include "../Components/Transform.h"


class CGameObject
{
public:
	inline CGameObject()
	{
		m_pTransform = std::make_shared<CTransform>();
	}

	CGameObject(const CGameObject&) = default;
	CGameObject(CGameObject&&) = default;
	CGameObject& operator= (const CGameObject&) = default;
	CGameObject& operator= (CGameObject&&) = default;
	virtual ~CGameObject() = default;

	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Draw(void);

	void AddComponent(std::shared_ptr<IComponent> a_component);
	void RemoveComponent(std::shared_ptr<IComponent> a_component);

	std::shared_ptr<CTransform> GetTransform(void);

protected:
	std::vector<std::shared_ptr<IComponent>> m_components{};
	std::shared_ptr<CTransform> m_pTransform{ nullptr };
};

#endif