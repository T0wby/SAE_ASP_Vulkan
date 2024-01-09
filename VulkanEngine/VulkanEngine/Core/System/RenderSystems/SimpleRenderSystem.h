#ifndef SIMPLERENDERSYSTEM_H
#define SIMPLERENDERSYSTEM_H
#include <memory>
#include <vector>
#include <Vulkan/Include/vulkan/vulkan_core.h>
#include "../Pipeline.h"
#include "../../../GameObjects/GameObject.h"
#include "../Scene.h"

class CSimpleRenderSystem
{
public:
    inline CSimpleRenderSystem(const std::shared_ptr<CDevice>& a_pDevice, VkRenderPass a_renderPass, VkDescriptorSetLayout a_descLayout)
        : m_pDevice(a_pDevice)
    {
        CreatePipelineLayout(a_descLayout);
        CreatePipeline(a_renderPass, a_descLayout);
    }
    ~CSimpleRenderSystem();

    CSimpleRenderSystem(const CSimpleRenderSystem &) = delete;
    CSimpleRenderSystem &operator=(const CSimpleRenderSystem &) = delete;

    void RenderGameObjects(const DrawInformation& a_drawInfo, const std::shared_ptr<CScene>& a_pCurrentScene);
    inline VkPipelineLayout GetLayout(void) const { return m_pipelineLayout; }

private:
    void CreatePipelineLayout(VkDescriptorSetLayout a_descLayout);
    void CreatePipeline(const VkRenderPass& a_renderPass, VkDescriptorSetLayout a_descLayout);

    std::shared_ptr<CDevice> m_pDevice{nullptr};
    std::unique_ptr<CPipeline> m_pPipeline{nullptr};
    VkPipelineLayout m_pipelineLayout{};
};
    
#endif
