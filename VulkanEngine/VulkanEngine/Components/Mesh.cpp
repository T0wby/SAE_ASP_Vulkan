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
