#include "pch.h"
#include <fstream>
#include "rendUtil.h"

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