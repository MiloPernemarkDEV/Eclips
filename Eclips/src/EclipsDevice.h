#ifndef ECLIPS_DEVICE_H
#define ECLIPS_DEVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include "EclipsQueue.h"
#include "EclipsDebug.h"
#include "EclipsSurface.h"	

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class EclipsDevice {
public:
	EclipsDevice(EclipsQueue& eclipsQueue, EclipsSurface& eclipsSurface);
	EclipsDevice() = delete;
	void pickPhysicalDevice(VkInstance instance);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	void createLogicalDevice(EclipsDebug& eclipsDebug, VkSurfaceKHR surface);

	VkPhysicalDevice getPhysicalDevice() { return physicalDevice; };
	VkDevice getLogicalDevice() { return device; };

private:
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	EclipsQueue* eclipsQueue;
	EclipsSurface* eclipsSurface;
};

#endif // ECLIPS_DEVICE_H