#include "Texture.h"
#include "../Utility/Utility.h"
#include <stb_image.h>
#include <stdexcept>

CTexture::~CTexture()
{
    
}

int CTexture::Initialize(void)
{
    return 0;
}

int CTexture::Initialize(const VkCommandBuffer& a_commandBuffer)
{
    
    return 0;
}

int CTexture::Update(const double& a_dDeltaTime)
{
    return 0;
}

void CTexture::Draw(void)
{
}

void CTexture::Draw(const DrawInformation& a_drawInformation)
{
}

void CTexture::Finalize(void)
{
	vkDestroySampler(m_pDevice->GetLogicalDevice(), m_textureSampler, nullptr);
	vkDestroyImageView(m_pDevice->GetLogicalDevice(), m_textureImageView, nullptr);
	vkDestroyImage(m_pDevice->GetLogicalDevice(), m_textureImage, nullptr);
	vkFreeMemory(m_pDevice->GetLogicalDevice(), m_textureImageMemory, nullptr);
}

void CTexture::CreateTextureImage(const std::string& a_texFilePath)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(a_texFilePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    const VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) 
    {
        throw std::runtime_error("failed to load texture image!");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    // The buffer should be in host visible memory so that we can map it and it should be usable as a transfer source so that we can copy it to an image later on:
    m_pDevice->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    // We can then directly copy the pixel values that we got from the image loading library to the buffer:
    void* data;
    vkMapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory);

    stbi_image_free(pixels);

    CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_textureImage, m_textureImageMemory);

    TransitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, m_textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

    TransitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(m_pDevice->GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void CTexture::CopyBufferToImage(const VkBuffer& a_buffer,const VkImage& a_image,const uint32_t& a_width,const uint32_t& a_height)
{
    const VkCommandBuffer commandBuffer = CUtility::BeginSingleTimeCommands(m_pDevice->GetLogicalDevice(), m_pDevice->GetCommandPool());

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        a_width,
        a_height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        a_buffer,
        a_image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    CUtility::EndSingleTimeCommands(commandBuffer, m_pDevice->GetGraphicsQueue(), m_pDevice->GetCommandPool(), m_pDevice->GetLogicalDevice());
}

void CTexture::CreateTextureImageView()
{
    m_textureImageView = CreateImageView(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void CTexture::CreateTextureSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;

    // Might be a good idea to save those in a variable
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(m_pDevice->GetPhysicalDevice(), &properties);

    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(m_pDevice->GetLogicalDevice(), &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void CTexture::CreateImage(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling,
    VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkImage& a_image, VkDeviceMemory& a_imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = a_width;
    imageInfo.extent.height = a_height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = a_format;
    imageInfo.tiling = a_tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = a_usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_pDevice->GetLogicalDevice(), &imageInfo, nullptr, &a_image) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_pDevice->GetLogicalDevice(), a_image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, a_properties);

    if (vkAllocateMemory(m_pDevice->GetLogicalDevice(), &allocInfo, nullptr, &a_imageMemory) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(m_pDevice->GetLogicalDevice(), a_image, a_imageMemory, 0);
}

VkImageView CTexture::CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = a_image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = a_format;
    viewInfo.subresourceRange.aspectMask = a_aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(m_pDevice->GetLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void CTexture::TransitionImageLayout(VkImage a_image, VkFormat a_format, VkImageLayout a_oldLayout,
    VkImageLayout a_newLayout)
{
    const VkCommandBuffer commandBuffer = CUtility::BeginSingleTimeCommands(m_pDevice->GetLogicalDevice(), m_pDevice->GetCommandPool());

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = a_oldLayout;
	barrier.newLayout = a_newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = a_image;
	if (a_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (HasStencilComponent(a_format)) 
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else 
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (a_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && a_newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (a_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && a_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (a_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && a_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else 
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	CUtility::EndSingleTimeCommands(commandBuffer, m_pDevice->GetGraphicsQueue(), m_pDevice->GetCommandPool(), m_pDevice->GetLogicalDevice());
}

uint32_t CTexture::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
    // query info about the available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_pDevice->GetPhysicalDevice(), &memProperties);

    // VkMemoryRequirements::memoryTypeBits is a bitfield that sets a bit for every memoryType that is
    // supported for the resource.Therefore we need to check if the bit at index i is set while also testing the
    // required memory property flags while iterating over the memory types.
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

bool CTexture::HasStencilComponent(VkFormat a_format)
{
	return a_format == VK_FORMAT_D32_SFLOAT_S8_UINT || a_format == VK_FORMAT_D24_UNORM_S8_UINT;
}
