#include "Variables.h"
#include <stdexcept>
#include <unordered_map>
#include "Utility.h"

const float F_ZERO = 0.0f;
const float F_ONE = 1.0f;


void Vertex::Position::operator+=(Position a_pos)
{
    x += a_pos.x;
    y += a_pos.y;
    z += a_pos.z;
}

Vertex::Position Vertex::Position::Zero()
{
    return Position{ F_ZERO, F_ZERO, F_ZERO };
}

Vertex::Position Vertex::Position::Forward()
{
    return Position{ F_ZERO, F_ZERO, F_ONE };
}

Vertex::Color Vertex::Color::Black(void)
{
    return Color{ F_ZERO, F_ZERO, F_ZERO };
}

Vertex::Color Vertex::Color::White(void)
{
    return Color{ F_ONE, F_ONE, F_ONE };
}

Vertex::Color Vertex::Color::Red(void)
{
    return Color{ F_ONE, F_ZERO, F_ZERO };
}

Vertex::Color Vertex::Color::Green(void)
{
    return Color{ F_ZERO, F_ONE, F_ZERO };
}

Vertex::Color Vertex::Color::Blue(void)
{
    return Color{ F_ZERO, F_ZERO, F_ONE };
}

Vertex::Normal Vertex::Normal::Zero()
{
    return Normal{ F_ZERO, F_ZERO, F_ZERO };
}
Vertex::Normal Vertex::Normal::One()
{
    return Normal{ F_ONE, F_ONE, F_ONE };
}

void MeshData::LoadModel(const std::string& a_filePath, MeshData& a_meshData)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, a_filePath.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    a_meshData.vertices.clear();
    a_meshData.indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                auto colorIndex = 3 * index.vertex_index + 2;
                if (colorIndex < attrib.colors.size())
                {
                    vertex.color = {
                        attrib.colors[colorIndex - 2],
                        attrib.colors[colorIndex - 1],
                        attrib.colors[colorIndex - 0],
                    };
                }
                else
                {
                    vertex.color = {1.f, 1.f, 1.f};  // set default color
                }
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                a_meshData.vertices.push_back(vertex);
            }
            a_meshData.indices.push_back(uniqueVertices[vertex]);
        }
    }
}
