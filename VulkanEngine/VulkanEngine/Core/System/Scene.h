#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <physx/PxPhysics.h>

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
        const std::shared_ptr<CDevice>& a_pDevice, const uint32_t& a_fWidth, const uint32_t& a_fHeight, physx::PxPhysics& a_physics)
        : m_pPlayerController(a_playerController), m_pWindow(a_window), m_pDevice(a_pDevice),
            m_fWidth(a_fWidth), m_fHeight(a_fHeight), m_physics(a_physics) {}

    CScene(const CScene&) = default;
    CScene(CScene&&) = default;
    CScene& operator= (const CScene&) = default;
    CScene& operator= (CScene&&) = default;
    virtual ~CScene() = default;

    void AddGameObject(std::shared_ptr<CGameObject>& a_gameObject);
    void RemoveGameObject(const std::shared_ptr<CGameObject>& a_gameObject);

    std::shared_ptr<CGameObject> GetGameObject(const int& a_iIndex);

    virtual UniformBufferObject& CreateUniformBuffer(void);
    void UpdateSizeValues(const int& a_iWidth, const int& a_iHeight);

    virtual void Initialize(void);
    virtual void Initialize(VkCommandBuffer a_commandBuffer);
    virtual void Update(const double& a_dDeltaTime);
    virtual void Draw(void);
    virtual void Draw(const DrawInformation& a_drawInformation);
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
    
    physx::PxPhysics& m_physics;

};
#endif
