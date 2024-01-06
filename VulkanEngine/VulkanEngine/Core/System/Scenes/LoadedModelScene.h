#ifndef LOADEDSCENE_H
#define LOADEDSCENE_H
#include "../Scene.h"
#include "../../../GameObjects/Primitives/Cube.h"
#include "../../../GameObjects/Primitives/LoadedCube.h"

class CLoadedModelScene : public CScene
{
public:
    inline CLoadedModelScene(const std::shared_ptr<CPlayerController>& a_playerController, const std::shared_ptr<CWindow>& a_window, const std::shared_ptr<CDevice>& a_pDevice, const uint32_t& a_fWidth, const uint32_t& a_fHeight)
        : CScene(a_playerController, a_window, a_pDevice, a_fWidth, a_fHeight){}

    CLoadedModelScene(const CLoadedModelScene&) = default;
    CLoadedModelScene(CLoadedModelScene&&) = default;
    CLoadedModelScene& operator= (const CLoadedModelScene&) = default;
    CLoadedModelScene& operator= (CLoadedModelScene&&) = default;
    ~CLoadedModelScene() override = default;

    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(const double& a_dDeltaTime) override;
    void Draw(void) override;
    void Draw(const DrawInformation& a_drawInformation) override;
    void Finalize(void) override;

    UniformBufferObject& CreateUniformBuffer(void) override;

private:
    std::shared_ptr<CGameObject> m_pLightObject{ nullptr };
    std::shared_ptr<CLoadedCube> m_pVaseLoad{ nullptr };

    void InitGameObjects(void);

};
#endif
