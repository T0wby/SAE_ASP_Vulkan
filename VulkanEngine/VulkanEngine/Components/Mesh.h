#ifndef MESH_H
#define MESH_H
#include "Component.h"
#include "../Utility/Variables.h"
#include <vector>

class CMesh : public IComponent
{
public:
	inline CMesh(){	}
	CMesh(const CMesh&) = default;
	CMesh(CMesh&&) = default;
	CMesh& operator= (const CMesh&) = default;
	CMesh& operator= (CMesh&&) = default;
	virtual ~CMesh() = default;

	// Inherited via IComponent
	virtual int Initialize(void) override;
	virtual int Update(void) override;
	virtual void Draw(void) override;
	virtual void Finalize(void) override;

	void SetVertexData(const std::vector<Vertex>& a_vertices);

private:
	std::vector<Vertex> m_vertices{};
};
#endif
