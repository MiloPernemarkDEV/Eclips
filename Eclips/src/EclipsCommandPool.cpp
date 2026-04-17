#include "pch.h"
#include "EclipsCommandPool.h"

void EclipsCommandPool::createCommandPool(EclipsQueue& eclipsQueue, EclipsDevice& eclipsDevice, EclipsSurface& eclipsSurface)
{
	QueueFamilyIndices queue_family_indices = eclipsQueue.findQueueFamilies(eclipsDevice.getPhysicalDevice(), eclipsSurface.getSurface());

	VkCommandPoolCreateInfo pool_info{};

	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	// allows any command buffer to be reset to initial state by calling VkResetCommandBuffer.
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = queue_family_indices.graphicsFamily.value();

	if (vkCreateCommandPool(eclipsDevice.getLogicalDevice(), &pool_info, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

VkCommandBuffer EclipsCommandPool::beginSingleTimeCommands(EclipsDevice& eclipsDevice)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer command_buffer{};
	vkAllocateCommandBuffers(eclipsDevice.getLogicalDevice(), &allocInfo, &command_buffer);

	VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(command_buffer, &begin_info);
	return command_buffer;
}

void EclipsCommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer, EclipsDevice& eclipsDevice, EclipsQueue& eclipsQueue) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	assert(eclipsQueue.getGraphicsQueue() != VK_NULL_HANDLE, "queue is null");
	vkQueueSubmit(eclipsQueue.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(eclipsQueue.getGraphicsQueue());

	vkFreeCommandBuffers(eclipsDevice.getLogicalDevice(), commandPool, 1, &commandBuffer);
}
