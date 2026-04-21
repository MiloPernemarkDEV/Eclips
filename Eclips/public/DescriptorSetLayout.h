#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include "Device.h"

class DescriptorSetLayout {
public:
    DescriptorSetLayout(Device& device);
    ~DescriptorSetLayout();

    void create();

    VkDescriptorSetLayout get() { return layout; }

private:
    Device& device;
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
};