#pragma once 

#include <vector>
#include <string>

class CommandPool;
class Device;
class Queue;

class rendUtil {
public:
	static std::vector<char> readFile(const std::string& filename);
	static VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
	static void createBuffer(VkDevice device, VkPhysicalDevice pDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void copyBuffer(Queue& queue, Device& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, CommandPool& cmdPool);

	static uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties, VkPhysicalDevice device);
private:
};