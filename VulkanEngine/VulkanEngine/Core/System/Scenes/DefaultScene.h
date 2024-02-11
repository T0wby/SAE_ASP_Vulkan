#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H
#include "Scene.h"
#include "../../../GameObjects/Primitives/Cube.h"
#include "../../../GameObjects/Primitives/LoadedVase.h"
#include "../../../GameObjects/Primitives/Quad.h"

class CDefaultScene : public CScene
{
public:
    inline CDefaultScene(const std::shared_ptr<CPlayerController>& a_playerController, const std::shared_ptr<CWindow>& a_window,
        const std::shared_ptr<CDevice>& a_pDevice, const uint32_t& a_fWidth, const uint32_t& a_fHeight)
        : CScene(a_playerController, a_window, a_pDevice, a_fWidth, a_fHeight){}

    CDefaultScene(const CDefaultScene&) = default;
    CDefaultScene(CDefaultScene&&) = default;
    CDefaultScene& operator= (const CDefaultScene&) = default;
    CDefaultScene& operator= (CDefaultScene&&) = default;
    ~CDefaultScene() override = default;

    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(const double& a_dDeltaTime) override;
    void Draw(void) override;
    void Draw(const DrawInformation& a_drawInformation) override;
    void Finalize(void) override;

    UniformBufferObject& CreateUniformBuffer(void) override;

private:
    std::shared_ptr<CCube> m_pCube{ nullptr };
    std::shared_ptr<CCube> m_pCube2{ nullptr };
    std::shared_ptr<CQuad> m_pFloor{ nullptr };
    std::shared_ptr<CGameObject> m_pLightObject{ nullptr };
    std::shared_ptr<CLoadedVase> m_pVaseLoad{ nullptr };

    void InitGameObjects(void);

};
#endif
