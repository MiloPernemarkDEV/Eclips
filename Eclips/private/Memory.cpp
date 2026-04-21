#include "pch.h"
#include "Memory.h"
#include "Device.h"

uint32_t Memory::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties, Device& device)
{
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevice(), &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
		if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	std::cout << "type_filter = " << type_filter << std::endl;
	std::cout << "requested properties = " << properties << std::endl;

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
		std::cout << "Memory type " << i
			<< " flags = " << mem_properties.memoryTypes[i].propertyFlags
			<< std::endl;
	}
	throw std::runtime_error("failed to find suitable memory type!");
}
