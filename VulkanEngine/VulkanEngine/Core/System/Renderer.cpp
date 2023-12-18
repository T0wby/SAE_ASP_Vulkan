#include "Renderer.h"

#include <stdexcept>

CRenderer::~CRenderer()
{
    FreeCommandBuffers();
}

VkCommandBuffer CRenderer::BeginFrame()
{
    assert(!m_bIsFrameStarted && "Frame already started");
    const VkResult result = m_pSwapChain->AquireNextImage(m_currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || m_pWindow->IsFrameBufferResized())
    {
        m_pWindow->SetIsFrameBufferResized(false);
        RecreateSwapChain();
        if (m_pCurrentScene != nullptr)
            m_pCurrentScene->UpdateSizeValues(static_cast<uint32_t>(m_pSwapChain->GetWidth()), static_cast<uint32_t>(m_pSwapChain->GetHeight()));
        return nullptr;
    }
    m_bIsFrameStarted = true;


    const auto commandBuffer = GetCurrentCommandBuffer();
    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Only relevant for secondary command buffers

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    return commandBuffer;
}

void CRenderer::EndFrame()
{
    assert(m_bIsFrameStarted && "Frame still in progress!");

    const auto commandBuffer = m_vCommandBuffers[m_currentFrameIndex];
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    const auto result = m_pSwapChain->SubmitCommandBuffers(commandBuffer, m_currentImageIndex, m_pCurrentScene);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||  m_pWindow->IsFrameBufferResized())
    {
        m_pWindow->SetIsFrameBufferResized(false);
        RecreateSwapChain();
        if (m_pCurrentScene != nullptr)
            m_pCurrentScene->UpdateSizeValues(static_cast<uint32_t>(m_pSwapChain->GetWidth()), static_cast<uint32_t>(m_pSwapChain->GetHeight()));
    }
    else if (result != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    m_bIsFrameStarted = false;
    m_currentFrameIndex = (m_currentFrameIndex + 1) % CSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void CRenderer::BeginSwapChainRenderPass(const DrawInformation& a_drawInfo)
{
    assert(m_bIsFrameStarted && "Frame still in progress!");
    assert(a_drawInfo.commandBuffer == GetCurrentCommandBuffer() && "Can't begin render pass on commandbuffer from a different frame!");

    // Starting a render pass
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_pSwapChain->GetRenderPass();
    renderPassInfo.framebuffer = m_pSwapChain->GetFrameBuffer(m_currentImageIndex);
    // size of the render area
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_pSwapChain->GetSwapChainExtent();

    constexpr VkClearValue clearColor = { {{0.1f, 0.1f, 0.1f, 1.0f}} };
    constexpr VkClearValue depthStencil = { {1.0f, 0.0f} };
    const std::vector<VkClearValue> clearValues{ clearColor , depthStencil };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    /* The final parameter controls how the drawing commands within the render pass will be provided. It can have one of two values:
    * VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
    * VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
    */
    vkCmdBeginRenderPass(a_drawInfo.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    //vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_pSwapChain->GetWidth());
    viewport.height = static_cast<float>(m_pSwapChain->GetHeight());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(a_drawInfo.commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_pSwapChain->GetSwapChainExtent();
    vkCmdSetScissor(a_drawInfo.commandBuffer, 0, 1, &scissor);

    //vkCmdBindDescriptorSets(a_drawInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, a_drawInfo.pipelineLayout, 0, 1, &m_pSwapChain->GetDescriptorSets()[m_currentFrameIndex], 0, nullptr);
}

void CRenderer::EndSwapChainRenderPass(const DrawInformation& a_drawInfo)
{
    assert(m_bIsFrameStarted && "Frame still in progress!");
    assert(a_drawInfo.commandBuffer == GetCurrentCommandBuffer() && "Can't end render pass on commandbuffer from a different frame!");

    vkCmdEndRenderPass(a_drawInfo.commandBuffer);
}

void CRenderer::CreateCommandBuffers()
{
    m_vCommandBuffers.resize(CSwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_pDevice->GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_vCommandBuffers.size());

    if (vkAllocateCommandBuffers(m_pDevice->GetLogicalDevice(), &allocInfo, m_vCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void CRenderer::FreeCommandBuffers()
{
    vkFreeCommandBuffers(m_pDevice->GetLogicalDevice(), m_pDevice->GetCommandPool(),
        static_cast<uint32_t>(m_vCommandBuffers.size()), m_vCommandBuffers.data());
    m_vCommandBuffers.clear();
}

void CRenderer::RecreateSwapChain()
{
    m_pWindow->CheckIfWindowMinimized();
    vkDeviceWaitIdle(m_pDevice->GetLogicalDevice());
    if (m_pSwapChain == nullptr)
    {
        m_pSwapChain = std::make_unique<CSwapChain>(m_pDevice, m_pWindow);
    }
    else
    {
        std::shared_ptr<CSwapChain> oldSwapChain = std::move(m_pSwapChain);
        m_pSwapChain = std::make_unique<CSwapChain>(m_pDevice, m_pWindow, oldSwapChain);
        if (!oldSwapChain->CompareSwapFormats(*m_pSwapChain))
            throw std::runtime_error("SwapChain Image or depth format has changed!");
    }
}
