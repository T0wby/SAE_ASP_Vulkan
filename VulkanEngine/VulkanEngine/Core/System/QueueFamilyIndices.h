#ifndef QUEUEFAMILYINDICES_H
#define QUEUEFAMILYINDICES_H
#include <cstdint>
#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool IsComplete() 
	{
		return graphicsFamily.has_value();
	}
};

#endif