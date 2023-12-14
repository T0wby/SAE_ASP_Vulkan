/*
 * Encapsulates a vulkan buffer
 *
 * Initially based off VulkanBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h
 */
#include "Buffer.h"

#include <cassert>

VkDeviceSize CBuffer::GetAlignment(VkDeviceSize a_instanceSize, VkDeviceSize a_minOffsetAlignment)
{
  if (a_minOffsetAlignment > 0) {
    return (a_instanceSize + a_minOffsetAlignment - 1) & ~(a_minOffsetAlignment - 1);
  }
  return a_instanceSize;
}
 
CBuffer::CBuffer(const std::shared_ptr<CDevice>& a_pDevice, VkDeviceSize a_instanceSize, uint32_t a_instanceCount, VkBufferUsageFlags a_usageFlags, VkMemoryPropertyFlags a_memoryPropertyFlags, VkDeviceSize a_minOffsetAlignment)
    : m_pDevice(a_pDevice),
      m_instanceCount{a_instanceCount},
      m_instanceSize{a_instanceSize},
      m_usageFlags{a_usageFlags},
      m_memoryPropertyFlags{a_memoryPropertyFlags}
{
   m_alignmentSize = GetAlignment(a_instanceSize, a_minOffsetAlignment);
   m_bufferSize =  m_alignmentSize * a_instanceCount;
  m_pDevice->CreateBuffer( m_bufferSize, a_usageFlags, a_memoryPropertyFlags,  m_buffer,  m_memory);
}
 
CBuffer::~CBuffer()
{
  Unmap();
  vkDestroyBuffer(m_pDevice->GetLogicalDevice(), m_buffer, nullptr);
  vkFreeMemory(m_pDevice->GetLogicalDevice(), m_memory, nullptr);
}
 
/**
 * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
 *
 * @param a_size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
 * buffer range.
 * @param a_offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the buffer mapping call
 */
VkResult CBuffer::Map(VkDeviceSize a_size, VkDeviceSize a_offset)
{
  assert(m_buffer && m_memory && "Called map on buffer before create");
  return vkMapMemory(m_pDevice->GetLogicalDevice(), m_memory, a_offset, a_size, 0, &m_mapped);
}
 
/**
 * Unmap a mapped memory range
 *
 * @note Does not return a result as vkUnmapMemory can't fail
 */
void CBuffer::Unmap()
{
  if (m_mapped) {
    vkUnmapMemory(m_pDevice->GetLogicalDevice(), m_memory);
    m_mapped = nullptr;
  }
}
 
/**
 * Copies the specified data to the mapped buffer. Default value writes whole buffer range
 *
 * @param a_data Pointer to the data to copy
 * @param a_size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
 * range.
 * @param a_offset (Optional) Byte offset from beginning of mapped region
 *
 */
void CBuffer::WriteToBuffer(void *a_data, VkDeviceSize a_size, VkDeviceSize a_offset)
{
  assert(m_mapped && "Cannot copy to unmapped buffer");
 
  if (a_size == VK_WHOLE_SIZE) {
    memcpy(m_mapped, a_data, m_bufferSize);
  } else {
    char *memOffset = (char *)m_mapped;
    memOffset += a_offset;
    memcpy(memOffset, a_data, a_size);
  }
}
 
/**
 * Flush a memory range of the buffer to make it visible to the device
 *
 * @note Only required for non-coherent memory
 *
 * @param a_size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
 * complete buffer range.
 * @param a_offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the flush call
 */
VkResult CBuffer::Flush(VkDeviceSize a_size, VkDeviceSize a_offset)
{
  VkMappedMemoryRange mappedRange = {};
  mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mappedRange.memory = m_memory;
  mappedRange.offset = a_offset;
  mappedRange.size = a_size;
  return vkFlushMappedMemoryRanges(m_pDevice->GetLogicalDevice(), 1, &mappedRange);
}
 
/**
 * Invalidate a memory range of the buffer to make it visible to the host
 *
 * @note Only required for non-coherent memory
 *
 * @param a_size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
 * the complete buffer range.
 * @param a_offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the invalidate call
 */
VkResult CBuffer::Invalidate(VkDeviceSize a_size, VkDeviceSize a_offset) {
  VkMappedMemoryRange mappedRange = {};
  mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mappedRange.memory = m_memory;
  mappedRange.offset = a_offset;
  mappedRange.size = a_size;
  return vkInvalidateMappedMemoryRanges(m_pDevice->GetLogicalDevice(), 1, &mappedRange);
}
 
/**
 * Create a buffer info descriptor
 *
 * @param a_size (Optional) Size of the memory range of the descriptor
 * @param a_offset (Optional) Byte offset from beginning
 *
 * @return VkDescriptorBufferInfo of specified offset and range
 */
VkDescriptorBufferInfo CBuffer::DescriptorInfo(VkDeviceSize a_size, VkDeviceSize a_offset)
{
  return VkDescriptorBufferInfo{
      m_buffer,
      a_offset,
      a_size,
  };
}
 
/**
 * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
 *
 * @param a_data Pointer to the data to copy
 * @param a_index Used in offset calculation
 *
 */
void CBuffer::WriteToIndex(void *a_data, int a_index)
{
  WriteToBuffer(a_data, m_instanceSize, a_index * m_alignmentSize);
}
 
/**
 *  Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
 *
 * @param a_index Used in offset calculation
 *
 */
VkResult CBuffer::FlushIndex(int a_index) { return Flush(m_alignmentSize, a_index * m_alignmentSize); }
 
/**
 * Create a buffer info descriptor
 *
 * @param a_index Specifies the region given by index * alignmentSize
 *
 * @return VkDescriptorBufferInfo for instance at index
 */
VkDescriptorBufferInfo CBuffer::DescriptorInfoForIndex(int a_index)
{
  return DescriptorInfo(m_alignmentSize, a_index * m_alignmentSize);
}
 
/**
 * Invalidate a memory range of the buffer to make it visible to the host
 *
 * @note Only required for non-coherent memory
 *
 * @param a_index Specifies the region to invalidate: index * alignmentSize
 *
 * @return VkResult of the invalidate call
 */
VkResult CBuffer::InvalidateIndex(int a_index)
{
  return Invalidate(m_alignmentSize, a_index * m_alignmentSize);
}