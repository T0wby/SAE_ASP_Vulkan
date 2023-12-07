#include "Pipeline.h"

#include <stdexcept>
#include "../../Utility/Utility.h"
#include "../../Utility/Variables.h"

void CPipeline::Finalize()
{
	vkDestroyShaderModule(m_device.GetLogicalDevice(), m_vertShaderModule, nullptr);
	vkDestroyShaderModule(m_device.GetLogicalDevice(), m_fragShaderModule, nullptr);

	vkDestroyPipeline(m_device.GetLogicalDevice(), m_graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_device.GetLogicalDevice(), m_pipelineConfig->pipelineLayout, nullptr);
	vkDestroyRenderPass(m_device.GetLogicalDevice(), m_pipelineConfig->renderPass, nullptr);
	
    vkDestroyPipeline(m_device.GetLogicalDevice(), m_graphicsPipeline, nullptr);
}

void CPipeline::Bind(VkCommandBuffer a_commandBuffer)
{
	vkCmdBindPipeline(a_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
}

PipelineConfigInfo CPipeline::DefaultPipelineConfigInfo(uint32_t a_iWidth, uint32_t a_iHeight)
{
	PipelineConfigInfo configInfo{};
	
	// The VkPipelineInputAssemblyStateCreateInfo struct describes two things: what kind of geometry will be drawn from the vertices and if primitive restart should be enabled.
	configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// Viewport
	configInfo.viewport.x = 0.0f;
	configInfo.viewport.y = 0.0f;
	configInfo.viewport.width = static_cast<float>(a_iWidth);
	configInfo.viewport.height = static_cast<float>(a_iHeight);
	configInfo.viewport.minDepth = 0.0f;
	configInfo.viewport.maxDepth = 1.0f;
	
	// Scissor(scissor rectangles define in which regions pixels will actually be stored. Any pixels outside the scissor rectangles will be discarded by the rasterizer.)
	configInfo.scissor.offset = { 0, 0 };
	configInfo.scissor.extent = {a_iWidth, a_iHeight};

	// Rasterizer (vertex data to fragments)
	configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; // VK_TRUE, then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them.
	configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; // VK_TRUE, then geometry never passes through the rasterizer stage. This basically disables any output to the framebuffer.
	configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	configInfo.rasterizationInfo.lineWidth = 1.0f;
	configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // Because of the Y-flip we did in the projection matrix this is set to counter clockwise but indices are still using clockwise
	configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
	configInfo.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
	configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

	// Multisampling (Disabled for now)
	configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	configInfo.multisampleInfo.minSampleShading = 1.0f; // Optional
	configInfo.multisampleInfo.pSampleMask = nullptr; // Optional
	configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

	// Depth and stencil testing
	configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
	configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
	configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.front = {}; // Optional
	configInfo.depthStencilInfo.back = {}; // Optional

	// Color blending
	configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	configInfo.colorBlendAttachment.blendEnable = VK_TRUE;
	configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
	configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	configInfo.colorBlendInfo.attachmentCount = 1;
	configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
	configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
	configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
	configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
	configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

	return configInfo;
}

void CPipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, PipelineConfigInfo* a_pipelineConfig, VkDescriptorSetLayout& a_descriptorSetLayout)
{
    //const auto vertShaderCode = CUtility::ReadFile("Shader/vert.spv");
    //const auto fragShaderCode = CUtility::ReadFile("Shader/frag.spv");
    const auto vertShaderCode = CUtility::ReadFile(vertFilepath);
    const auto fragShaderCode = CUtility::ReadFile(fragFilepath);

    // Wrapper for  SPIR-V bytecode
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

	// Dynamic state
	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	// This will cause the configuration of these values to be ignored and we need to specify the data at drawing time.
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();


	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptionPos = Vertex::GetAttributeDescriptionPos();
	auto attributeDescriptionColor = Vertex::GetAttributeDescriptionCol();
	auto attributeDescriptionUV = Vertex::GetAttributeDescriptionUV();

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = { attributeDescriptionPos, attributeDescriptionColor, attributeDescriptionUV };

	// Vertex Input(Data hard coded inside of the shader atm)
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount =  1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional


	// Pipeline Layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1; // Optional
	pipelineLayoutInfo.pSetLayouts = &a_descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(m_device.GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &a_pipelineConfig->pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
	
	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &a_pipelineConfig->viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &a_pipelineConfig->scissor;

	// Creating Pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &a_pipelineConfig->inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &a_pipelineConfig->rasterizationInfo;
	pipelineInfo.pMultisampleState = &a_pipelineConfig->multisampleInfo;
	pipelineInfo.pDepthStencilState = &a_pipelineConfig->depthStencilInfo;
	pipelineInfo.pColorBlendState = &a_pipelineConfig->colorBlendInfo;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = a_pipelineConfig->pipelineLayout;
	pipelineInfo.renderPass = a_pipelineConfig->renderPass;
	pipelineInfo.subpass = a_pipelineConfig->subpass;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(m_device.GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
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

    if (vkCreateShaderModule(m_device.GetLogicalDevice(), &createInfo, nullptr, a_vertShaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }
}
