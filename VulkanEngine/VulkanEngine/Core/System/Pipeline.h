#ifndef PIPELINE_H
#define PIPELINE_H
#include <string>

#include "CoreSystemStructs.h"
#include "Device.h"

class CPipeline
{
public:
    inline CPipeline(CDevice a_device, const PipelineConfigInfo& a_pipelineConfig, const std::string& a_vertFilepath, const std::string& a_fragFilepath, VkDescriptorSetLayout& a_descriptorSetLayout)
        : m_device(std::move(a_device)), m_pipelineConfig(a_pipelineConfig)
    {
        CreateGraphicsPipeline(a_vertFilepath, a_fragFilepath, m_pipelineConfig, a_descriptorSetLayout);
    }
    
    CPipeline(const CPipeline&) = delete;
    CPipeline(CPipeline&&) = delete;
    CPipeline& operator= (const CPipeline&) = delete;
    CPipeline& operator= (CPipeline&&) = delete;
    ~CPipeline() = default;

    void Finalize(void);

    void Bind(VkCommandBuffer a_commandBuffer);
    static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t a_iWidth, uint32_t a_iHeight);
    
private:
    CDevice m_device;
    VkPipeline m_graphicsPipeline{};
    PipelineConfigInfo m_pipelineConfig{};
    VkShaderModule m_vertShaderModule{};
    VkShaderModule m_fragShaderModule{};
    uint32_t m_WIDTH = 800;
    uint32_t m_HEIGHT = 600;
    
    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo& a_pipelineConfig, VkDescriptorSetLayout& a_descriptorSetLayout);
    void CreateShaderModule(const std::vector<char>& a_vBytecode, VkShaderModule* a_vertShaderModule);
    
};
#endif
