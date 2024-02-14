#ifndef CUBE_H
#define CUBE_H
#include "../GameObject.h"
#include "../../Components/Mesh.h"
#include "../../Utility/CoreStructs.h"

class CCube : public CGameObject
{
public:
    static CCube CreateGameObject(const std::shared_ptr<CDevice>& a_pDevice)
    {
        static id_t currentId = 0;
        
        return CCube{a_pDevice, currentId++};
    }
    CCube(const CCube&) = default;
    CCube(CCube&&) = default;
    CCube& operator= (const CCube&) = default;
    CCube& operator= (CCube&&) = default;
    ~CCube() override;


    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(const double& a_dDeltaTime) override;
    void Draw(void) override;
    void Draw(const DrawInformation& a_drawInformation) override;
    void Finalize(void) override;

    virtual std::vector<Vertex>& GetMeshVertexData(void) override;
    virtual std::vector<uint16_t>& GetMeshIndiceData(void) override;

private:
    inline CCube(const std::shared_ptr<CDevice>& a_pDevice, id_t a_objId) : CGameObject(a_pDevice, a_objId){}
    std::shared_ptr<CMesh> m_pMesh{ nullptr };
    std::vector<Vertex> m_vertices{};

};
#endif
