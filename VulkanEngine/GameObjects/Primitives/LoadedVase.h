#ifndef LOADEDCUBE_H
#define LOADEDCUBE_H
#include "../GameObject.h"
#include "../../Components/Mesh.h"
#include "../../Utility/CoreStructs.h"

class CLoadedVase : public CGameObject
{
public:
    static CLoadedVase CreateGameObject(const std::shared_ptr<CDevice>& a_pDevice)
    {
        static id_t currentId = 0;
        return CLoadedVase{a_pDevice, currentId++};
    }
    CLoadedVase(const CLoadedVase&) = delete;
    CLoadedVase(CLoadedVase&&) = default;
    CLoadedVase& operator= (const CLoadedVase&) = delete;
    CLoadedVase& operator= (CLoadedVase&&) = default;
    ~CLoadedVase() override;


    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(const double& a_dDeltaTime) override;
    void Draw(void) override;
    void Draw(const DrawInformation& a_drawInformation) override;
    void Finalize(void) override;

    virtual std::vector<Vertex>& GetMeshVertexData(void) override;
    virtual std::vector<uint16_t>& GetMeshIndiceData(void) override;

private:
    inline CLoadedVase(const std::shared_ptr<CDevice>& a_pDevice, id_t a_objId) : CGameObject(a_pDevice, a_objId){}
    std::shared_ptr<CMesh> m_pMesh{ nullptr };
    std::vector<Vertex> m_vertices{};

};
#endif
