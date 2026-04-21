#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include "Device.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class Queue {
public:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	VkQueue& getGraphicsQueue() { return graphicsQueue; }
	VkQueue& getPresentQueue() { return presentQueue; }
private:
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
};
