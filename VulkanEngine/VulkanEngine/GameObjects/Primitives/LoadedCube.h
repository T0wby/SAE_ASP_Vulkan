#ifndef LOADEDCUBE_H
#define LOADEDCUBE_H
#include "../GameObject.h"
#include "../../Components/Mesh.h"
#include "../../Utility/Variables.h"

class CLoadedCube : public CGameObject
{
public:
    static CLoadedCube CreateGameObject(const std::shared_ptr<CDevice>& a_pDevice)
    {
        static id_t currentId = 0;
        return CLoadedCube{a_pDevice, currentId++};
    }
    CLoadedCube(const CLoadedCube&) = delete;
    CLoadedCube(CLoadedCube&&) = default;
    CLoadedCube& operator= (const CLoadedCube&) = delete;
    CLoadedCube& operator= (CLoadedCube&&) = default;
    ~CLoadedCube() override;


    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(const double& a_dDeltaTime) override;
    void Draw(void) override;
    void Draw(const DrawInformation& a_drawInformation) override;
    void Finalize(void) override;

    virtual std::vector<Vertex>& GetMeshVertexData(void) override;
    virtual std::vector<uint16_t>& GetMeshIndiceData(void) override;

private:
    inline CLoadedCube(const std::shared_ptr<CDevice>& a_pDevice, id_t a_objId) : CGameObject(a_pDevice, a_objId){}
    std::shared_ptr<CMesh> m_pMesh{ nullptr };
    std::vector<Vertex> m_vertices{};

};
#endif
