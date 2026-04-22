#include "pch.h"
#include "VBO.h"
#include "Device.h"
#include "Queue.h"
#include "CommandPool.h"
#include "rendUtil.h"

void VBO::create(Queue& queue, Device& device, CommandPool& commandPool, std::vector<Vertex>& vertices) {
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	rendUtil::createBuffer(device.getLogicalDevice(), device.getPhysicalDevice(), bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device.getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device.getLogicalDevice(), stagingBufferMemory);

	rendUtil::createBuffer(device.getLogicalDevice(), device.getPhysicalDevice(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	rendUtil::copyBuffer(queue, device ,stagingBuffer, vertexBuffer, bufferSize, commandPool);
	vkDestroyBuffer(device.getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device.getLogicalDevice(), stagingBufferMemory, nullptr);
}