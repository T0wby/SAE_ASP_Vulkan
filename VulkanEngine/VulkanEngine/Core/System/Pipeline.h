#ifndef PIPELINE_H
#define PIPELINE_H
#include <string>

#include "Device.h"

class CPipeline
{
public:
    inline CPipeline(const std::shared_ptr<CDevice>& a_pDevice, const PipelineConfigInfo& a_pipelineConfig, const std::string& a_vertFilepath, const std::string& a_fragFilepath)
        : m_pDevice(a_pDevice), m_pipelineConfig(a_pipelineConfig)
    {
        CreateGraphicsPipeline(a_vertFilepath, a_fragFilepath, m_pipelineConfig);
    }
    
    CPipeline(const CPipeline&) = delete;
    CPipeline(CPipeline&&) = delete;
    CPipeline& operator= (const CPipeline&) = delete;
    CPipeline& operator= (CPipeline&&) = delete;
    ~CPipeline() = default;

    void Finalize(void);

    static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t a_iWidth, uint32_t a_iHeight);
    
private:
    std::shared_ptr<CDevice> m_pDevice{nullptr};
    VkDescriptorSetLayout m_descriptorSetLayout{};
    VkPipeline m_graphicsPipeline{};
    PipelineConfigInfo m_pipelineConfig{};
    VkShaderModule m_vertShaderModule{};
    VkShaderModule m_fragShaderModule{};
    constexpr uint32_t m_WIDTH = 800;
    constexpr uint32_t m_HEIGHT = 600;
    
    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo& a_pipelineConfig);
    void CreateShaderModule(const std::vector<char>& a_vBytecode, VkShaderModule* a_vertShaderModule);
    void CreateDescriptorSetLayout(void);
};
#endif
