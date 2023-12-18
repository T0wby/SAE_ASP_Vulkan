#include "Descriptors.h"

#include <cassert>
#include <stdexcept>

// *************** Descriptor Set Layout Builder *********************

CDescriptorSetLayout::Builder& CDescriptorSetLayout::Builder::AddBinding(
    uint32_t a_binding, VkDescriptorType a_descriptorType, VkShaderStageFlags a_stageFlags, uint32_t a_count)
{
    assert(m_bindings.count(a_binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = a_binding;
    layoutBinding.descriptorType = a_descriptorType;
    layoutBinding.descriptorCount = a_count;
    layoutBinding.stageFlags = a_stageFlags;
    m_bindings[a_binding] = layoutBinding;
    return *this;
}

std::unique_ptr<CDescriptorSetLayout> CDescriptorSetLayout::Builder::Build() const
{
    return std::make_unique<CDescriptorSetLayout>(m_pDevice, m_bindings);
}

// *************** Descriptor Set Layout *********************

CDescriptorSetLayout::CDescriptorSetLayout(const std::shared_ptr<CDevice>& a_pDevice,
                                           std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> a_bindings)
    : m_pDevice(a_pDevice), m_bindings{a_bindings}
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    for (auto kv : a_bindings)
    {
        setLayoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(
        m_pDevice->GetLogicalDevice(),
        &descriptorSetLayoutInfo,
        nullptr,
        &m_descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

CDescriptorSetLayout::~CDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(m_pDevice->GetLogicalDevice(), m_descriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

CDescriptorPool::Builder& CDescriptorPool::Builder::AddPoolSize(
    VkDescriptorType a_descriptorType, uint32_t a_count)
{
    m_poolSizes.push_back({a_descriptorType, a_count});
    return *this;
}

CDescriptorPool::Builder& CDescriptorPool::Builder::SetPoolFlags(
    VkDescriptorPoolCreateFlags a_flags)
{
    m_poolFlags = a_flags;
    return *this;
}

CDescriptorPool::Builder& CDescriptorPool::Builder::SetMaxSets(uint32_t a_count)
{
    m_maxSets = a_count;
    return *this;
}

std::unique_ptr<CDescriptorPool> CDescriptorPool::Builder::Build() const
{
    return std::make_unique<CDescriptorPool>(m_pDevice, m_maxSets, m_poolFlags, m_poolSizes);
}

// *************** Descriptor Pool *********************

CDescriptorPool::CDescriptorPool(const std::shared_ptr<CDevice>& a_pDevice, uint32_t a_maxSets,
                                 VkDescriptorPoolCreateFlags a_poolFlags,
                                 const std::vector<VkDescriptorPoolSize>& a_poolSizes)
    : m_pDevice(a_pDevice)
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(a_poolSizes.size());
    descriptorPoolInfo.pPoolSizes = a_poolSizes.data();
    descriptorPoolInfo.maxSets = a_maxSets;
    descriptorPoolInfo.flags = a_poolFlags;

    if (vkCreateDescriptorPool(m_pDevice->GetLogicalDevice(), &descriptorPoolInfo, nullptr, &m_descriptorPool) !=
        VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

CDescriptorPool::~CDescriptorPool()
{
    vkDestroyDescriptorPool(m_pDevice->GetLogicalDevice(), m_descriptorPool, nullptr);
}

bool CDescriptorPool::AllocateDescriptorSet(
    const VkDescriptorSetLayout a_descriptorSetLayout, VkDescriptorSet& a_descriptor) const
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.pSetLayouts = &a_descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
    // a new pool whenever an old pool fills up. But this is beyond our current scope
    if (vkAllocateDescriptorSets(m_pDevice->GetLogicalDevice(), &allocInfo, &a_descriptor) != VK_SUCCESS)
    {
        return false;
    }
    return true;
}

void CDescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet>& a_descriptors) const
{
    vkFreeDescriptorSets(
        m_pDevice->GetLogicalDevice(),
        m_descriptorPool,
        static_cast<uint32_t>(a_descriptors.size()),
        a_descriptors.data());
}

void CDescriptorPool::ResetPool()
{
    vkResetDescriptorPool(m_pDevice->GetLogicalDevice(), m_descriptorPool, 0);
}

// *************** Descriptor Writer *********************

CDescriptorWriter::CDescriptorWriter(CDescriptorSetLayout& a_setLayout, CDescriptorPool& a_pool)
    : m_setLayout{a_setLayout}, m_pool{a_pool}
{
}

CDescriptorWriter& CDescriptorWriter::WriteBuffer(
    uint32_t a_binding, VkDescriptorBufferInfo* a_bufferInfo)
{
    assert(m_setLayout.m_bindings.count(a_binding) == 1 && "Layout does not contain specified binding");

    auto& bindingDescription = m_setLayout.m_bindings[a_binding];

    assert(
        bindingDescription.descriptorCount == 1 &&
        "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = a_binding;
    write.pBufferInfo = a_bufferInfo;
    write.descriptorCount = 1;

    m_vWrites.push_back(write);
    return *this;
}

CDescriptorWriter& CDescriptorWriter::WriteImage(
    uint32_t a_binding, VkDescriptorImageInfo* a_imageInfo)
{
    assert(m_setLayout.m_bindings.count(a_binding) == 1 && "Layout does not contain specified binding");

    auto& bindingDescription = m_setLayout.m_bindings[a_binding];

    //assert(
    //    bindingDescription.descriptorCount == 1 &&
    //    "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = a_binding;
    write.pImageInfo = a_imageInfo;
    write.descriptorCount = 1;

    m_vWrites.push_back(write);
    return *this;
}

bool CDescriptorWriter::Build(VkDescriptorSet& a_set)
{
    bool success = m_pool.AllocateDescriptorSet(m_setLayout.GetDescriptorSetLayout(), a_set);
    if (!success)
    {
        return false;
    }
    Overwrite(a_set);
    return true;
}

void CDescriptorWriter::Overwrite(VkDescriptorSet& a_set)
{
    for (auto& write : m_vWrites)
    {
        write.dstSet = a_set;
    }
    vkUpdateDescriptorSets(m_pool.m_pDevice->GetLogicalDevice(), m_vWrites.size(), m_vWrites.data(), 0, nullptr);
}
