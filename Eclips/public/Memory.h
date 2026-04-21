#pragma once

#include <vulkan/vulkan.h>

class Device;

class Memory {
public:
	uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties, Device& device);
private:
};