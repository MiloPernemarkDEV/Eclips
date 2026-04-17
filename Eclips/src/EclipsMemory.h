#ifndef ECLIPS_MEMORY_H
#define ECLIPS_MEMORY_H

#include <vulkan/vulkan.h>

class EclipsDevice;

class EclipsMemory {
public:
	uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties, EclipsDevice& eclipsDevice);
private:
};

#endif // ECLIPS_MEMORY_H