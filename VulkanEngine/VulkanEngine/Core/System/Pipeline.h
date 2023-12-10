#ifndef PIPELINE_H
#define PIPELINE_H
#include <string>

#include "CoreSystemStructs.h"
#include "Device.h"

class CPipeline
{
public:
    inline CPipeline(const std::shared_ptr<CDevice>& a_pDevice, PipelineConfigInfo* a_pipelineConfig,
        const std::string& a_vertFilepath, const std::string& a_fragFilepath,
        VkDescriptorSetLayout& a_descriptorSetLayout)
        : m_pDevice(a_pDevice), m_pipelineConfig(a_pipelineConfig)
    {
        
        CreateGraphicsPipeline(a_vertFilepath, a_fragFilepath, m_pipelineConfig, a_descriptorSetLayout);
    }
    
    CPipeline(const CPipeline&) = delete;
    CPipeline(CPipeline&&) = delete;
    CPipeline& operator= (const CPipeline&) = delete;
    CPipeline& operator= (CPipeline&&) = delete;
    ~CPipeline();

    void Bind(VkCommandBuffer a_commandBuffer);
    static void DefaultPipelineConfigInfo(PipelineConfigInfo& a_configInfo);
    
private:
    std::shared_ptr<CDevice> m_pDevice{nullptr};
    VkPipeline m_graphicsPipeline{};
    PipelineConfigInfo* m_pipelineConfig{};
    VkShaderModule m_vertShaderModule{};
    VkShaderModule m_fragShaderModule{};
    uint32_t m_WIDTH = 800;
    uint32_t m_HEIGHT = 600;
    
    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo* a_pipelineConfig, VkDescriptorSetLayout& a_descriptorSetLayout);
    void CreateShaderModule(const std::vector<char>& a_vBytecode, VkShaderModule* a_vertShaderModule);
    
};
#endif
