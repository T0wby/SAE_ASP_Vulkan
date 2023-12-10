#ifndef MESH_H
#define MESH_H


#include <memory>
#include "Component.h"
#include "../Utility/Variables.h"
#include "../Core/System/Device.h"
#include "../Core/System/CoreSystemStructs.h"
#include <vector>

class CMesh : public IComponent
{
public:
	inline CMesh(const std::shared_ptr<CDevice>& a_pDevice, const MeshData& a_meshData)
		: m_vertices(a_meshData.vertices), m_indices(a_meshData.indices), m_pDevice(a_pDevice)
	{
		CreateVertexBuffer(a_meshData.vertices);
		CreateIndexBuffer(a_meshData.indices);
	}
	CMesh(const CMesh&) = default;
	CMesh(CMesh&&) = default;
	CMesh& operator= (const CMesh&) = default;
	CMesh& operator= (CMesh&&) = default;
	~CMesh();

	static std::unique_ptr<CMesh> CreateMeshFromFile(const std::shared_ptr<CDevice>& a_pDevice, const std::string& a_filePath, MeshData& a_meshData);
	
	// Inherited via IComponent
	int Initialize(void) override;
	int Initialize(const VkCommandBuffer& a_commandBuffer) override;
	int Update(const double& a_dDeltaTime) override;
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
