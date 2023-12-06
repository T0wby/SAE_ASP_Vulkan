#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H
#include "../Scene.h"
#include "../../../GameObjects/Primitives/Cube.h"

class CDefaultScene : public CScene
{
public:
    inline CDefaultScene(const std::shared_ptr<CPlayerController>& a_playerController, const std::shared_ptr<CWindow>& a_window, const std::shared_ptr<CDevice>& a_pDevice, const uint32_t& a_fWidth, const uint32_t& a_fHeight)
        : CScene(a_playerController, a_window, a_pDevice, a_fWidth, a_fHeight){}

    CDefaultScene(const CDefaultScene&) = default;
    CDefaultScene(CDefaultScene&&) = default;
    CDefaultScene& operator= (const CDefaultScene&) = default;
    CDefaultScene& operator= (CDefaultScene&&) = default;
    ~CDefaultScene() override = default;

    void Initialize(void) override;
    void Initialize(VkCommandBuffer a_commandBuffer) override;
    void Update(void) override;
    void Draw(void) override;
    void Draw(VkCommandBuffer a_commandBuffer) override;
    void Finalize(void) override;

private:
    std::shared_ptr<CCube> m_pCube{ nullptr };
    //std::shared_ptr<CCube> m_cube2{ nullptr };

};
#endif
