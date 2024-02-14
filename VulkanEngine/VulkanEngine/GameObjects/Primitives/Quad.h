#ifndef QUAD_H
#define QUAD_H
#include "../GameObject.h"
#include "../../Components/Mesh.h"
#include "../../Utility/CoreStructs.h"

class CQuad : public CGameObject
{
public:
    static CQuad CreateGameObject(const std::shared_ptr<CDevice>& a_pDevice)
    {
        static id_t currentId = 0;
        return CQuad{a_pDevice, currentId++};
    }
    CQuad(const CQuad&) = default;
    CQuad(CQuad&&) = default;
    CQuad& operator= (const CQuad&) = default;
    CQuad& operator= (CQuad&&) = default;
    ~CQuad() override;


    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(const double& a_dDeltaTime) override;
    void Draw(void) override;
    void Draw(const DrawInformation& a_drawInformation) override;
    void Finalize(void) override;

    virtual std::vector<Vertex>& GetMeshVertexData(void) override;
    virtual std::vector<uint16_t>& GetMeshIndiceData(void) override;

private:
    inline CQuad(const std::shared_ptr<CDevice>& a_pDevice, id_t a_objId) : CGameObject(a_pDevice, a_objId){}
    std::shared_ptr<CMesh> m_pMesh{ nullptr };
    std::vector<Vertex> m_vertices{};
};
#endif