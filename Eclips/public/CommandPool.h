#pragma once

#include <vulkan/vulkan.h>
#include "Queue.h"
#include "Device.h"
#include "Surface.h"

class CommandPool {
public:
	void createCommandPool(Queue& Queue, Device& Device, Surface&Surface);

	VkCommandBuffer beginSingleTimeCommands(Device& eclipsDevice);
	void endSingleTimeCommands(VkCommandBuffer commandBuffer, Device& eclipsDevice, Queue& Queue);
	VkCommandPool& getCommandPool() { return commandPool; };

private:
	VkCommandPool commandPool = VK_NULL_HANDLE;
};
