#include "SimpleRenderSystem.h"

#include <stdexcept>

const std::string VERT_SHADER = "Shader/vert.spv";
const std::string FRAG_SHADER = "Shader/frag.spv";

CSimpleRenderSystem::~CSimpleRenderSystem()
{
    vkDestroyPipelineLayout(m_pDevice->GetLogicalDevice(), m_pipelineLayout, nullptr);
}

void CSimpleRenderSystem::RenderGameObjects(const DrawInformation& a_drawInfo, const std::shared_ptr<CScene>& a_pCurrentScene)
{
    m_pPipeline->Bind(a_drawInfo.commandBuffer);
    a_pCurrentScene->Initialize(a_drawInfo.commandBuffer);
    a_pCurrentScene->Draw(a_drawInfo);
}

void CSimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout a_descLayout)
{
    // Pipeline Layout
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);
	
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = &a_descLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional

    if (vkCreatePipelineLayout(m_pDevice->GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void CSimpleRenderSystem::CreatePipeline(const VkRenderPass& renderPass, VkDescriptorSetLayout a_descLayout)
{
    PipelineConfigInfo defaultPipelineConfigInfo{};
    CPipeline::DefaultPipelineConfigInfo(defaultPipelineConfigInfo);
    defaultPipelineConfigInfo.renderPass = renderPass;
    defaultPipelineConfigInfo.pipelineLayout = m_pipelineLayout;
    m_pPipeline = std::make_unique<CPipeline>(m_pDevice, &defaultPipelineConfigInfo, VERT_SHADER, FRAG_SHADER, a_descLayout);
}
