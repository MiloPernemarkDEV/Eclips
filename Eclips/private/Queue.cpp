#include "pch.h"
#include "Queue.h"

QueueFamilyIndices Queue::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
	QueueFamilyIndices familyIndices;
	uint32_t queueFamilyCount = 0;

	//@todo
	// Debug Exception thrown somewhere around here 
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) familyIndices.graphicsFamily = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
		if (presentSupport) familyIndices.presentFamily = i;

		if (familyIndices.isComplete()) break;
		i++;
	}
	return familyIndices;
}