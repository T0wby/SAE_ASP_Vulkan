#pragma once
#include "../Scene.h"
class CDefaultScene : public CScene
{
public:
    inline CDefaultScene(std::shared_ptr<CPlayerController> a_playerController, std::shared_ptr<CWindow> a_window, uint32_t& a_fWidth, uint32_t& a_fHeight)
        : CScene(a_playerController, a_window, a_fWidth, a_fHeight){}

    CDefaultScene(const CDefaultScene&) = default;
    CDefaultScene(CDefaultScene&&) = default;
    CDefaultScene& operator= (const CDefaultScene&) = default;
    CDefaultScene& operator= (CDefaultScene&&) = default;
    ~CDefaultScene() override = default;

    void Initialize() override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override;

private:
    std::shared_ptr<CCube> m_cube{ nullptr };
    //std::shared_ptr<CCube> m_cube2{ nullptr };

};

