#include "pch.h"
#include "EclipsQueue.h"

QueueFamilyIndices EclipsQueue::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices family_indices;
	uint32_t queue_family_count = 0;

	//@todo
	// Debug Exception thrown somewhere around here 
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	int i = 0;
	for (const auto& queue_family : queue_families) {
		if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) family_indices.graphicsFamily = i;

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
		if (present_support) family_indices.presentFamily = i;

		if (family_indices.isComplete()) break;
		i++;
	}
	return family_indices;
}