#ifndef COMPONENT_H
#define COMPONENT_H
#include <Vulkan/Include/vulkan/vulkan_core.h>


class IComponent
{
public:
	virtual int Initialize(void) = 0;
	virtual int Initialize(VkCommandBuffer a_commandBuffer) = 0;
	virtual int Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Draw(VkCommandBuffer a_commandBuffer) = 0;
	virtual void Finalize(void) = 0;
};
#endif // !COMPONENT_H
