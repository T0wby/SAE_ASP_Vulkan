#ifndef MESH_H
#define MESH_H
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm/glm.hpp>
#include <memory>
#include "Component.h"
#include "../Utility/Variables.h"
#include "../Core/System/Device.h"
#include <vector>

class CMesh : public IComponent
{
public:
	inline CMesh(const std::shared_ptr<CDevice>& a_pDevice, const std::vector<Vertex>& a_vertices, const std::vector<uint16_t>& a_indices)
		: m_vertices(a_vertices), m_indices(a_indices), m_pDevice(a_pDevice)
	{
		CreateVertexBuffer(a_vertices);
		CreateIndexBuffer(a_indices);
	}
	CMesh(const CMesh&) = default;
	CMesh(CMesh&&) = default;
	CMesh& operator= (const CMesh&) = default;
	CMesh& operator= (CMesh&&) = default;
	~CMesh() = default;
	
	// Inherited via IComponent
	int Initialize(void) override;
	int Initialize(VkCommandBuffer a_commandBuffer) override;
	int Update(void) override;
	void Draw(void) override;
	void Draw(const DrawInformation& a_drawInformation) override;
	void Finalize(void) override;

	void SetVertexData(const std::vector<Vertex>& a_vertices);
	std::vector<Vertex>& GetVertexData(void);
	void SetIndiceData(const std::vector<uint16_t>& a_indices);
	std::vector<uint16_t>& GetIndiceData(void);

private:
	std::vector<Vertex> m_vertices{};
	std::vector<uint16_t> m_indices{};
	std::shared_ptr<CDevice> m_pDevice{nullptr};
	VkBuffer m_vertexBuffer{};
	VkDeviceMemory m_vertexBufferMemory{};
	VkBuffer m_indexBuffer{};
	VkDeviceMemory m_indexBufferMemory{};

	void CreateVertexBuffer(const std::vector<Vertex>& a_vertices);
	void CreateIndexBuffer(const std::vector<uint16_t>& a_indices);
};
#endif
