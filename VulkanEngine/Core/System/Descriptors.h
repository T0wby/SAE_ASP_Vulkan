#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H
#include <memory>
#include <unordered_map>

#include "Device.h"

class CDescriptorSetLayout
{
 public:
  class Builder
  {
   public:
    Builder(const std::shared_ptr<CDevice>& a_pDevice) : m_pDevice{a_pDevice} {}
   
    /// <summary> Appends to the map of bindings </summary>
    /// <param name="a_binding">  </param>
    /// <param name="a_descriptorType"> The type to expect(uniform, image...) </param>
    /// <param name="a_stageFlags"> Which shader stages have access </param>
    /// <param name="a_count"> Count of descriptors </param>
    /// <returns> .... </returns>
    Builder& AddBinding( uint32_t a_binding, VkDescriptorType a_descriptorType, VkShaderStageFlags a_stageFlags, uint32_t a_count = 1);
   
    std::unique_ptr<CDescriptorSetLayout> Build() const;
 
   private:
    std::shared_ptr<CDevice> m_pDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
  };
 
  CDescriptorSetLayout(const std::shared_ptr<CDevice>& a_pDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> a_bindings);
  ~CDescriptorSetLayout();
  CDescriptorSetLayout(const CDescriptorSetLayout &) = delete;
  CDescriptorSetLayout &operator=(const CDescriptorSetLayout &) = delete;
 
  VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
 
 private:
  std::shared_ptr<CDevice> m_pDevice;
  VkDescriptorSetLayout m_descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;
 
  friend class CDescriptorWriter;
};
 
class CDescriptorPool
{
 public:
  class Builder
 {
   public:
    Builder(const std::shared_ptr<CDevice>& a_pDevice) : m_pDevice{a_pDevice} {}
    
    Builder &AddPoolSize(VkDescriptorType a_descriptorType, uint32_t a_count);
    Builder &SetPoolFlags(VkDescriptorPoolCreateFlags a_flags);
    Builder &SetMaxSets(uint32_t a_count);
    std::unique_ptr<CDescriptorPool> Build() const;
 
   private:
    std::shared_ptr<CDevice> m_pDevice{nullptr};
    std::vector<VkDescriptorPoolSize> m_poolSizes{};
    uint32_t m_maxSets = 1000;
    VkDescriptorPoolCreateFlags m_poolFlags = 0;
 };
 
  CDescriptorPool(const std::shared_ptr<CDevice>& a_pDevice, uint32_t a_maxSets, VkDescriptorPoolCreateFlags a_poolFlags, const std::vector<VkDescriptorPoolSize> &a_poolSizes);
  ~CDescriptorPool();
  CDescriptorPool(const CDescriptorPool &) = delete;
  CDescriptorPool &operator=(const CDescriptorPool &) = delete;
 
  bool AllocateDescriptorSet(
      const VkDescriptorSetLayout a_descriptorSetLayout, VkDescriptorSet &a_descriptor) const;
 
  void FreeDescriptors(std::vector<VkDescriptorSet> &a_descriptors) const;
 
  void ResetPool();
 
 private:
  std::shared_ptr<CDevice> m_pDevice{nullptr};
  VkDescriptorPool m_descriptorPool;
 
  friend class CDescriptorWriter;
};
 
class CDescriptorWriter
{
 public:
  CDescriptorWriter(CDescriptorSetLayout &a_setLayout, CDescriptorPool &a_pool);
 
  CDescriptorWriter &WriteBuffer(uint32_t a_binding, VkDescriptorBufferInfo *a_bufferInfo);
  CDescriptorWriter &WriteImage(uint32_t a_binding, VkDescriptorImageInfo *a_imageInfo);
 
  bool Build(VkDescriptorSet &a_set);
  void Overwrite(VkDescriptorSet &a_set);
 
 private:
  CDescriptorSetLayout& m_setLayout;
  CDescriptorPool& m_pool;
  std::vector<VkWriteDescriptorSet> m_vWrites;
};
#endif