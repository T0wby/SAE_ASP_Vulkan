#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <Vulkan/Include/vulkan/vulkan_core.h>
#include "SwapChain.h"
#include "Scene.h"

class CRenderer
{
public:
    inline CRenderer(const std::shared_ptr<CDevice>& a_pDevice,
        const std::shared_ptr<CWindow>& a_pWindow,
        const std::shared_ptr<CScene>& a_pCurrentScene)
            : m_pDevice(a_pDevice), m_pWindow(a_pWindow), m_pCurrentScene(a_pCurrentScene)
    {
        RecreateSwapChain();
        CreateCommandBuffers();
    }
    CRenderer(const CRenderer&) = delete;
    CRenderer(CRenderer&&) = default;
    CRenderer& operator= (const CRenderer&) = delete;
    CRenderer& operator= (CRenderer&&) = default;
    ~CRenderer();

    VkCommandBuffer BeginFrame(void);
    void EndFrame(void);
    void BeginSwapChainRenderPass(const DrawInformation& a_drawInfo);
    void EndSwapChainRenderPass(const DrawInformation& a_drawInfo);
    void RecreateSwapChain(void);

    inline auto IsFrameInProgress(void) const -> const bool { return m_bIsFrameStarted; }
    inline auto GetCurrentCommandBuffer(void) const -> const VkCommandBuffer&{return m_vCommandBuffers[m_currentFrameIndex];}
    inline auto GetSwapChainRenderPass(void) const -> const VkRenderPass { return m_pSwapChain->GetRenderPass(); }
    inline int GetFrameIndex() const
    {
        assert(m_bIsFrameStarted && "Cannot get frame index when frame not in progress");
        return m_currentFrameIndex;
    }

    inline VkDescriptorImageInfo GetDescriptorImageInfo() const
    {
        return m_pSwapChain->GetDescriptorImageInfo();
    }

private:
    std::shared_ptr<CDevice> m_pDevice{nullptr};
    std::shared_ptr<CWindow> m_pWindow = nullptr;
    std::unique_ptr<CSwapChain> m_pSwapChain{nullptr};
    std::shared_ptr<CScene> m_pCurrentScene{nullptr};
    std::vector<VkCommandBuffer> m_vCommandBuffers{};
    uint32_t m_currentImageIndex{0};
    int m_currentFrameIndex{0};
    bool m_bIsFrameStarted{false};
    
    void CreateCommandBuffers(void);
    void FreeCommandBuffers(void);
};
#endif