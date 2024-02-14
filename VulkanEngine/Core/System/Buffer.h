#ifndef BUFFER_H
#define BUFFER_H
#include "Device.h"


class CBuffer
{
public:
    CBuffer(const std::shared_ptr<CDevice>& a_pDevice, VkDeviceSize a_instanceSize, uint32_t a_instanceCount, VkBufferUsageFlags a_usageFlags, VkMemoryPropertyFlags a_memoryPropertyFlags, VkDeviceSize a_minOffsetAlignment = 1);
    ~CBuffer();
 
    CBuffer(const CBuffer&) = delete;
    CBuffer& operator=(const CBuffer&) = delete;
 
    VkResult Map(VkDeviceSize a_size = VK_WHOLE_SIZE, VkDeviceSize a_offset = 0);
    void Unmap();
 
    void WriteToBuffer(void* a_data, VkDeviceSize a_size = VK_WHOLE_SIZE, VkDeviceSize a_offset = 0);
    VkResult Flush(VkDeviceSize a_size = VK_WHOLE_SIZE, VkDeviceSize a_offset = 0);
    VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize a_size = VK_WHOLE_SIZE, VkDeviceSize a_offset = 0);
    VkResult Invalidate(VkDeviceSize a_size = VK_WHOLE_SIZE, VkDeviceSize a_offset = 0);
 
    void WriteToIndex(void* a_data, int a_index);
    VkResult FlushIndex(int a_index);
    VkDescriptorBufferInfo DescriptorInfoForIndex(int a_index);
    VkResult InvalidateIndex(int a_index);
 
    VkBuffer GetBuffer() const { return m_buffer; }
    void* GetMappedMemory() const { return m_mapped; }
    uint32_t GetInstanceCount() const { return m_instanceCount; }
    VkDeviceSize GetInstanceSize() const { return m_instanceSize; }
    VkDeviceSize GetAlignmentSize() const { return m_instanceSize; }
    VkBufferUsageFlags GetUsageFlags() const { return m_usageFlags; }
    VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return m_memoryPropertyFlags; }
    VkDeviceSize GetBufferSize() const { return m_bufferSize; }
 
private:
    static VkDeviceSize GetAlignment(VkDeviceSize a_instanceSize, VkDeviceSize a_minOffsetAlignment);
 
    std::shared_ptr<CDevice> m_pDevice{nullptr};
    void* m_mapped = nullptr;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
 
    VkDeviceSize m_bufferSize;
    uint32_t m_instanceCount;
    VkDeviceSize m_instanceSize;
    VkDeviceSize m_alignmentSize;
    VkBufferUsageFlags m_usageFlags;
    VkMemoryPropertyFlags m_memoryPropertyFlags;
    
};
#endif
