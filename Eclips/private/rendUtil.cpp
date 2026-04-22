#include "pch.h"
#include <fstream>
#include "rendUtil.h"
#include "CommandPool.h"
#include "Device.h"

std::vector<char> rendUtil::readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file");
	}

	size_t file_size = (size_t)file.tellg();
	std::vector<char> buffer(file_size);

	file.seekg(0);
	file.read(buffer.data(), file_size);

	file.close();
	return buffer;
}

VkShaderModule rendUtil::createShaderModule(const std::vector<char>& code, VkDevice device) {
	assert(device == VK_NULL_HANDLE, "Device is VK_NULL_HANDLE");

	VkShaderModuleCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code.size();
	create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shader_module;
	if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shader_module;
}

void rendUtil::createBuffer(VkDevice device, VkPhysicalDevice pDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
	VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("createBuffer(); failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, pDevice);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t rendUtil::findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties, VkPhysicalDevice device)
{
	
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(device, &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
		if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	std::cout << "type_filter = " << type_filter << std::endl;
	std::cout << "requested properties = " << properties << std::endl;

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
		std::cout << "Memory type " << i
			<< " flags = " << mem_properties.memoryTypes[i].propertyFlags
			<< std::endl;
	}
	throw std::runtime_error("failed to find suitable memory type!");
}


void rendUtil::copyBuffer(Queue& queue, Device& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, CommandPool& commandPool) {
	VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands(device);

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	commandPool.endSingleTimeCommands(commandBuffer, device, queue);
}