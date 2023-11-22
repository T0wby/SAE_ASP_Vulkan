#include "Mesh.h"

int CMesh::Initialize(void)
{
    return 0;
}

int CMesh::Update(void)
{
    return 0;
}

void CMesh::Draw(void)
{
}

void CMesh::Finalize(void)
{
}

void CMesh::SetVertexData(const std::vector<Vertex>& a_vertices)
{
    m_vertices = a_vertices;
}

std::vector<Vertex>& CMesh::GetVertexData(void)
{
    return m_vertices;
}

void CMesh::SetIndiceData(const std::vector<uint16_t>& a_indices)
{
    m_indices = a_indices;
}

std::vector<uint16_t>& CMesh::GetIndiceData(void)
{
    return m_indices;
}
