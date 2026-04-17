#ifndef ECLIPS_COMMAND_POOL_H
#define ECLIPS_COMMAND_POOL_H

#include <vulkan/vulkan.h>
#include "EclipsQueue.h"
#include "EclipsDevice.h"
#include "EclipsSurface.h"

// The cmd pool is used per frame 
class EclipsCommandPool {
public:
	void createCommandPool(EclipsQueue& eclipsQueue, EclipsDevice& eclipsDevice, EclipsSurface& eclipsSurface);

	VkCommandBuffer beginSingleTimeCommands(EclipsDevice& eclipsDevice);
	void endSingleTimeCommands(VkCommandBuffer commandBuffer, EclipsDevice& eclipsDevice, EclipsQueue& eclipsQueue);
	VkCommandPool& getCommandPool() { return commandPool; };

private:
	VkCommandPool commandPool = VK_NULL_HANDLE;
};

#endif // ECLIPS_COMMAND_POOL_H