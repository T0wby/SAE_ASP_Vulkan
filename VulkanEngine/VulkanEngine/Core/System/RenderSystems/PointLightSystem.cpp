#include "PointLightSystem.h"

#include <stdexcept>

const std::string VERT_SHADER = "Shader/point_light_vert.spv";
const std::string FRAG_SHADER = "Shader/point_light_frag.spv";

CPointLightSystem::~CPointLightSystem()
{
    vkDestroyPipelineLayout(m_pDevice->GetLogicalDevice(), m_pipelineLayout, nullptr);
}

void CPointLightSystem::Render(const DrawInformation& a_drawInfo)
{
    m_pPipeline->Bind(a_drawInfo.commandBuffer);

    vkCmdBindDescriptorSets(a_drawInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, a_drawInfo.pipelineLayout,
        0, 1, &a_drawInfo.globalDescriptorSet, 0, nullptr);
    
    vkCmdDraw(a_drawInfo.commandBuffer, 6, 1, 0, 0);
}

void CPointLightSystem::CreatePipelineLayout(VkDescriptorSetLayout a_descLayout)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = &a_descLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(m_pDevice->GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void CPointLightSystem::CreatePipeline(const VkRenderPass& renderPass, VkDescriptorSetLayout a_descLayout)
{
    PipelineConfigInfo defaultPipelineConfigInfo{};
    CPipeline::DefaultPipelineConfigInfo(defaultPipelineConfigInfo);
    defaultPipelineConfigInfo.renderPass = renderPass;
    defaultPipelineConfigInfo.pipelineLayout = m_pipelineLayout;
    m_pPipeline = std::make_unique<CPipeline>(m_pDevice, &defaultPipelineConfigInfo, VERT_SHADER, FRAG_SHADER, a_descLayout);
}
