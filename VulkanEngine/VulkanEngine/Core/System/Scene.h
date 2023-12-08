#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include "../../GameObjects/GameObject.h"
#include "../../Input/PlayerController.h"
#include "../../Utility/Variables.h"
#include "Device.h"
#include "CoreSystemStructs.h"

class CScene
{
public:

    CScene() = default;
    inline CScene(const std::shared_ptr<CPlayerController>& a_playerController, const std::shared_ptr<CWindow>& a_window,
        const std::shared_ptr<CDevice>& a_pDevice, const uint32_t& a_fWidth, const uint32_t& a_fHeight)
        : m_pPlayerController(a_playerController), m_pWindow(a_window), m_pDevice(a_pDevice),
            m_fWidth(a_fWidth), m_fHeight(a_fHeight) {}

    CScene(const CScene&) = default;
    CScene(CScene&&) = default;
    CScene& operator= (const CScene&) = default;
    CScene& operator= (CScene&&) = default;
    virtual ~CScene() = default;

    void AddGameObject(std::shared_ptr<CGameObject> a_gameObject);
    void RemoveGameObject(std::shared_ptr<CGameObject> a_gameObject);

    std::shared_ptr<CGameObject> GetGameObject(const int& a_iIndex);
    auto GetSceneVertexCount(void) const -> const int;
    auto GetSceneFirstVertex(void) const -> const std::vector<Vertex>;
    auto GetSceneIndicesCount(void) const -> const int;
    auto GetSceneFirstIndice(void) const -> const std::vector<uint16_t>;

    virtual UniformBufferObject& CreateUniformBuffer(void);
    void UpdateSizeValues(const int& a_iWidth, const int& a_iHeight);

    virtual void Initialize(void);
    virtual void Initialize(VkCommandBuffer a_commandBuffer);
    virtual void Update(void);
    virtual void Draw(void);
    virtual void Draw(DrawInformation& a_drawInformation);
    virtual void Finalize(void);

protected:
    void CreateGameObjects(void);
    void SetupSceneInput(void);
    std::shared_ptr<CCamera> m_pCamera{ nullptr };
    std::shared_ptr<CGameObject> m_pCameraObject{ nullptr };
    std::shared_ptr<CPlayerController> m_pPlayerController{ nullptr };
    std::shared_ptr<CWindow> m_pWindow{ nullptr };
    std::shared_ptr<CDevice> m_pDevice{ nullptr };
    std::vector<std::shared_ptr<CGameObject>> m_vGameObjects{};

    uint32_t m_fWidth{ 0 };
    uint32_t m_fHeight{ 0 };

    // Delta time
    float m_fDeltaTime{ 0 };
    float m_fLastFrame{ 0 };
    float m_fCurrentFrame{ 0 };

};
#endif
