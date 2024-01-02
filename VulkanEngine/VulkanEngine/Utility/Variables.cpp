#include "Variables.h"
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
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

auto MeshData::LoadMesh(const std::string& a_filePath) -> const aiScene*
{
    Assimp::Importer imp;
    const auto pScene = imp.ReadFile(a_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!pScene | pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
    {
        throw std::runtime_error(imp.GetErrorString());
    }
    
    return pScene;
}

void MeshData::ProcessMesh(const aiMesh* a_pMesh, const aiScene* a_pScene, MeshData& a_data)
{
    for (int i = 0; i < a_pMesh->mNumVertices; ++i)
    {
        
        a_data.vertices.push_back(Vertex{Vertex::Position{a_pMesh->mVertices[i].x, a_pMesh->mVertices[i].y, a_pMesh->mVertices[i].z},
                                            //Vertex::Color{a_pMesh->mColors[0][i].r, a_pMesh->mColors[0][i].g, a_pMesh->mColors[0][i].b},
                                                Vertex::Color::White(),
                                                Vertex::Normal{a_pMesh->mNormals[i].x, a_pMesh->mNormals[i].y, a_pMesh->mNormals[i].z},
                                                {a_pMesh->mTextureCoords[0][i].x, a_pMesh->mTextureCoords[0][i].y}});
    }

    for (int i = 0; i < a_pMesh->mNumFaces; ++i)
    {
        const auto face = a_pMesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) a_data.indices.push_back(face.mIndices[j]);
    }
}

void MeshData::ProcessNode(const aiNode* a_pNode, const aiScene* a_pScene, MeshData& a_data)
{
    for (int i = 0; i < a_pNode->mNumMeshes; ++i)
    {
        const auto mesh = a_pScene->mMeshes[a_pNode->mMeshes[i]];
        ProcessMesh(mesh, a_pScene, a_data);
    }

    for (int i = 0; i < a_pNode->mNumChildren; ++i)
    {
        ProcessNode(a_pNode->mChildren[i], a_pScene, a_data);
    }
}
