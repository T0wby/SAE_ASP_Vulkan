#ifndef COMPONENT_H
#define COMPONENT_H
#include "../Utility/CoreStructs.h"


class IComponent
{
public:
	virtual int Initialize(void) = 0;
	virtual int Initialize(const VkCommandBuffer& a_commandBuffer) = 0;
	virtual int Update(const double& a_dDeltaTime) = 0;
	virtual void Draw(void) = 0;
	virtual void Draw(const DrawInformation& a_drawInformation) = 0;
	virtual void Finalize(void) = 0;
};
#endif // !COMPONENT_H
