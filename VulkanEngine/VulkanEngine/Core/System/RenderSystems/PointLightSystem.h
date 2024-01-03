#ifndef POINTLIGHTSYSTEM_H
#define POINTLIGHTSYSTEM_H
#include <memory>
#include <Vulkan/Include/vulkan/vulkan_core.h>
#include "../Pipeline.h"
#include "../../../GameObjects/GameObject.h"
#include "../Scene.h"

class CPointLightSystem
{
public:
    inline CPointLightSystem(const std::shared_ptr<CDevice>& a_pDevice, VkRenderPass a_renderPass, VkDescriptorSetLayout a_descLayout)
        : m_pDevice(a_pDevice)
    {
        CreatePipelineLayout(a_descLayout);
        CreatePipeline(a_renderPass, a_descLayout);
    }
    ~CPointLightSystem();

    CPointLightSystem(const CPointLightSystem &) = delete;
    CPointLightSystem &operator=(const CPointLightSystem &) = delete;

    void Render(const DrawInformation& a_drawInfo);
    inline VkPipelineLayout GetLayout(void) const { return m_pipelineLayout; }

private:
    void CreatePipelineLayout(VkDescriptorSetLayout a_descLayout);
    void CreatePipeline(const VkRenderPass& renderPass, VkDescriptorSetLayout a_descLayout);

    std::shared_ptr<CDevice> m_pDevice{nullptr};
    std::unique_ptr<CPipeline> m_pPipeline{nullptr};
    VkPipelineLayout m_pipelineLayout{};
};
    
#endif
