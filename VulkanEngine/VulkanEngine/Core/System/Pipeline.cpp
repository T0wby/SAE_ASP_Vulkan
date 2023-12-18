#include "Pipeline.h"

#include <stdexcept>
#include "../../Utility/Utility.h"
#include "../../Utility/Variables.h"

CPipeline::~CPipeline()
{
	vkDestroyShaderModule(m_pDevice->GetLogicalDevice(), m_vertShaderModule, nullptr);
	vkDestroyShaderModule(m_pDevice->GetLogicalDevice(), m_fragShaderModule, nullptr);

	vkDestroyPipeline(m_pDevice->GetLogicalDevice(), m_graphicsPipeline, nullptr);
}

void CPipeline::Bind(VkCommandBuffer a_commandBuffer)
{
	vkCmdBindPipeline(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
}

void CPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& a_configInfo)
{
	// The VkPipelineInputAssemblyStateCreateInfo struct describes two things: what kind of geometry will be drawn from the vertices and if primitive restart should be enabled.
	a_configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	a_configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	a_configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	a_configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	a_configInfo.viewportInfo.viewportCount = 1;
	a_configInfo.viewportInfo.pViewports = nullptr;
	a_configInfo.viewportInfo.scissorCount = 1;
	a_configInfo.viewportInfo.pScissors = nullptr;

	// Rasterizer (vertex data to fragments)
	a_configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	a_configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; // VK_TRUE, then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them.
	a_configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; // VK_TRUE, then geometry never passes through the rasterizer stage. This basically disables any output to the framebuffer.
	a_configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	a_configInfo.rasterizationInfo.lineWidth = 1.0f;
	a_configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	a_configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // Because of the Y-flip we did in the projection matrix this is set to counter clockwise but indices are still using clockwise
	a_configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	a_configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
	a_configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
	a_configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

	// Multisampling (Disabled for now)
	a_configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	a_configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	a_configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	a_configInfo.multisampleInfo.minSampleShading = 1.0f; // Optional
	a_configInfo.multisampleInfo.pSampleMask = nullptr; // Optional
	a_configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	a_configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

	// Depth and stencil testing
	a_configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	a_configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	a_configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	a_configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	a_configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	a_configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
	a_configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
	a_configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	a_configInfo.depthStencilInfo.front = {}; // Optional
	a_configInfo.depthStencilInfo.back = {}; // Optional

	// Color blending
	a_configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	a_configInfo.colorBlendAttachment.blendEnable = VK_TRUE;
	a_configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	a_configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	a_configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	a_configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	a_configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	a_configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	a_configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	a_configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
	a_configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	a_configInfo.colorBlendInfo.attachmentCount = 1;
	a_configInfo.colorBlendInfo.pAttachments = &a_configInfo.colorBlendAttachment;
	a_configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
	a_configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
	a_configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
	a_configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

	a_configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	// This will cause the configuration of these values to be ignored and we need to specify the data at drawing time.
	a_configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	a_configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(a_configInfo.dynamicStateEnables.size());
	a_configInfo.dynamicStateInfo.pDynamicStates = a_configInfo.dynamicStateEnables.data();
	a_configInfo.dynamicStateInfo.flags = 0;

	a_configInfo.uboLayoutBinding.binding = 0;
	a_configInfo.uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	a_configInfo.uboLayoutBinding.descriptorCount = 1;
	a_configInfo.uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // only used in Vertex shader
	a_configInfo.uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	a_configInfo.samplerLayoutBinding.binding = 1;
	a_configInfo.samplerLayoutBinding.descriptorCount = 1;
	a_configInfo.samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	a_configInfo.samplerLayoutBinding.pImmutableSamplers = nullptr;
	a_configInfo.samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	const std::vector<VkDescriptorSetLayoutBinding> bindings = { a_configInfo.uboLayoutBinding, a_configInfo.samplerLayoutBinding };
	a_configInfo.layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	a_configInfo.layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	a_configInfo.layoutInfo.pBindings = bindings.data();
}

void CPipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo* a_pipelineConfig, VkDescriptorSetLayout& a_descriptorSetLayout)
{
    const auto vertShaderCode = CUtility::ReadFile(vertFilepath);
    const auto fragShaderCode = CUtility::ReadFile(fragFilepath);

    // Wrapper for SPIR-V bytecode
    CreateShaderModule(vertShaderCode, &m_vertShaderModule);
    CreateShaderModule(fragShaderCode, &m_fragShaderModule);

    // Shader Stage
	VkPipelineShaderStageCreateInfo shaderStages[2];
	shaderStages[0] = {};
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = m_vertShaderModule;
	shaderStages[0].pName = "main";

	shaderStages[1] = {};
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = m_fragShaderModule;
	shaderStages[1].pName = "main";

    const auto bindingDescription = Vertex::GetBindingDescription();
	const auto attributeDescriptionPos = Vertex::GetAttributeDescriptionPos();
	const auto attributeDescriptionColor = Vertex::GetAttributeDescriptionCol();
	const auto attributeDescriptionNormal = Vertex::GetAttributeDescriptionNormal();
	const auto attributeDescriptionUV = Vertex::GetAttributeDescriptionUV();

	const std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
		attributeDescriptionPos, attributeDescriptionColor, attributeDescriptionNormal, attributeDescriptionUV
	};

	// Vertex Input
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount =  1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional


	// Pipeline Layout
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{a_descriptorSetLayout};
	
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size()); // Optional
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional

	if (vkCreatePipelineLayout(m_pDevice->GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &a_pipelineConfig->pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	// Creating Pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &a_pipelineConfig->inputAssemblyInfo;
	pipelineInfo.pViewportState = &a_pipelineConfig->viewportInfo;
	pipelineInfo.pRasterizationState = &a_pipelineConfig->rasterizationInfo;
	pipelineInfo.pMultisampleState = &a_pipelineConfig->multisampleInfo;
	pipelineInfo.pDepthStencilState = &a_pipelineConfig->depthStencilInfo;
	pipelineInfo.pColorBlendState = &a_pipelineConfig->colorBlendInfo;
	pipelineInfo.pDynamicState = &a_pipelineConfig->dynamicStateInfo;
	pipelineInfo.layout = a_pipelineConfig->pipelineLayout;
	pipelineInfo.renderPass = a_pipelineConfig->renderPass;
	pipelineInfo.subpass = a_pipelineConfig->subpass;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(m_pDevice->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}
}

void CPipeline::CreateShaderModule(const std::vector<char>& a_vBytecode, VkShaderModule* a_vertShaderModule)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = a_vBytecode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(a_vBytecode.data());

    if (vkCreateShaderModule(m_pDevice->GetLogicalDevice(), &createInfo, nullptr, a_vertShaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }
}
