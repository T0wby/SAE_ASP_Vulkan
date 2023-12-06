#ifndef CUBE_H
#define CUBE_H
#include "../GameObject.h"
#include "../../Components/Mesh.h"
#include "../../Utility/Variables.h"

class CCube : public CGameObject
{
public:
    inline CCube(const std::shared_ptr<CDevice>& a_pDevice) : CGameObject(a_pDevice){}
    CCube(const CCube&) = default;
    CCube(CCube&&) = default;
    CCube& operator= (const CCube&) = default;
    CCube& operator= (CCube&&) = default;
    ~CCube() override;


    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(void) override;
    void Draw(void) override;
    void Draw(VkCommandBuffer a_commandBuffer) override;
    void Finalize(void) override;

    virtual std::vector<Vertex>& GetMeshVertexData(void) override;
    virtual std::vector<uint16_t>& GetMeshIndiceData(void) override;

private:
    std::shared_ptr<CMesh> m_pMesh{ nullptr };
    std::vector<Vertex> m_vertices{};

};
#endif
