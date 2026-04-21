#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class Instance {
public:
	Instance() = default;
	void createInstance(std::vector<const char*> requiredExtensions, uint32_t layerCount,
		const std::vector<const char*> validationLayers, bool enableVal);
	void destroyInstance();
	VkInstance getInstance() const { return instance; };
private:
	VkInstance instance;
};
