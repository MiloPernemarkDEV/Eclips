#ifndef ECLIPS_DEVICE_H
#define ECLIPS_DEVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include "EclipsDebug.h"
#include "EclipsSurface.h"

class EclipsSwapchain;
class EclipsQueue;

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class EclipsDevice {
public:
    EclipsDevice(EclipsSurface& eclipsSurface);
    EclipsDevice() = delete;

    void pickPhysicalDevice(VkInstance instance, EclipsQueue& eclipsQueue, EclipsSwapchain& eclipsSwapchain);
    bool isDeviceSuitable(VkPhysicalDevice device, EclipsQueue& eclipsQueue, EclipsSwapchain& eclipsSwapchain);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    // Note: Passing EclipsDevice& into a member of EclipsDevice might be redundant
    void createLogicalDevice(EclipsDebug& eclipsDebug, VkSurfaceKHR surface, EclipsDevice& eclipsDevice, EclipsQueue& eclipsQueue);

    VkPhysicalDevice& getPhysicalDevice() { return physicalDevice; };
    VkDevice& getLogicalDevice() { return device; };

private:
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    EclipsSurface* eclipsSurface;
};

#endif // ECLIPS_DEVICE_H