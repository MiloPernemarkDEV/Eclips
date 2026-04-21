#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "Debug.h"
#include "Surface.h"

class Swapchain;
class Queue;

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class Device {
public:
    Device(Surface& Surface);
    Device() = delete;

    void pickPhysicalDevice(VkInstance instance, Queue& queue, Swapchain& swapchain);
    bool isDeviceSuitable(VkPhysicalDevice device, Queue& queue, Swapchain& swapchain);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    void createLogicalDevice(Debug& debug, Surface& surface, Device& device, Queue& queue);

    VkPhysicalDevice& getPhysicalDevice() { return physdevice; };
    VkDevice& getLogicalDevice() { return logDevice; };

private:
    VkPhysicalDevice physdevice;
    VkDevice logDevice;
    Surface* surface;
};