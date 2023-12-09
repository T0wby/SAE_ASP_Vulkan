#ifndef CORESYSTEMSTRUCTS_H
#define CORESYSTEMSTRUCTS_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>
#include <optional>
#include <glm/glm/glm.hpp>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() 
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 transform;
};

struct PipelineConfigInfo
{
	PipelineConfigInfo(const PipelineConfigInfo&) = delete;
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
	
	VkPipelineViewportStateCreateInfo viewportInfo{};
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
	VkPipelineMultisampleStateCreateInfo multisampleInfo{};
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
	std::vector<VkDynamicState> dynamicStateEnables{};
	VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
};

struct SimplePushConstantData
{
	glm::mat4 transform;
};

struct DrawInformation
{
	VkCommandBuffer commandBuffer;
	VkPipelineLayout pipelineLayout;
};

#endif