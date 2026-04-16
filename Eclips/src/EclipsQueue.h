#ifndef ECLIPS_QUEUE_H
#define ECLIPS_QUEUE_H

#include <vulkan/vulkan.h>
#include <optional>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class EclipsQueue {
public:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
	VkQueue getGraphicsQueue()  { return graphicsQueue; };
	VkQueue getPresentQueue()  { return presentQueue; };
private:
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
};

#endif // ECLIPS_QUEUE_H